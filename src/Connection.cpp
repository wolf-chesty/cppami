// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/Connection.hpp"

#include "c++ami/action/Action.hpp"
#include "c++ami/net/SocketReader.hpp"
#include "c++ami/net/SocketWriter.hpp"
#include "c++ami/net/TcpSocket.hpp"
#include "c++ami/StreamParser.hpp"
#include <fmt/core.h>

#include <quill/Backend.h>

using namespace cpp_ami;

Connection::Connection(quill::Logger *logger, std::string_view hostname, uint16_t port)
    : logger_(logger)
{
    auto sock = std::make_shared<net::TcpSocket>(hostname, port);

    dispatcher_ = std::make_unique<EventDispatcher>(
        [this](EventDispatcher::event_ptr_t dict) -> void {
            dispatch_handler(std::move(dict));
        });

    stream_parser_ = std::make_unique<StreamParser>(
        [this](std::string ami_version) -> void {
            ami_version_ = std::move(ami_version);
        },
        [this](std::string event) -> void {
            dispatcher_->add_event(std::move(event));
        });

    reader_ = std::make_unique<net::SocketReader>(sock,
        [this](std::string buf) -> void {
            stream_parser_->add_buf(std::move(buf));
    });

    writer_ = std::make_unique<net::SocketWriter>(sock);
}

Connection::~Connection()
{
    // Make sure objects get deleted in correct order
    reader_.reset();
    writer_.reset();
    stream_parser_.reset();
    dispatcher_.reset();
}

std::string Connection::get_ami_version() const
{
    return ami_version_;
}

void Connection::dispatch_handler(EventDispatcher::event_ptr_t dict)
{
    std::unique_lock const lock(callbacks_mutex_);
    for (auto const &[_, callback] : callbacks_) {
        callback(dict.get());
    }
}

Connection::event_callback_key_t Connection::add_callback(event_callback_t callback)
{
    auto const id = action::Action::create_uuid();
    std::unique_lock const lock(callbacks_mutex_);
    callbacks_.emplace(id, std::move(callback));
    return id;
}

void Connection::remove_callback(event_callback_key_t const &key)
{
    std::unique_lock const lock(callbacks_mutex_);
    callbacks_.erase(key);
}

void Connection::async_invoke(action::Action const &action) const
{
    writer_->write(action.to_string());
}

Connection::reaction_ptr_t Connection::invoke(action::Action const &action) const
{
    auto reaction = dispatcher_->get_event_pipe(action.get_action_id());

    // Send action to AMI; this will kick off creation of reaction pipe result
    writer_->write(action.to_string());

    // Wait for and return event
    return reaction.get();
}

Connection::reaction_ptr_t Connection::invoke(action::Action const &action, std::chrono::milliseconds const &timeout) const
{
    auto reaction = dispatcher_->get_event_pipe(action.get_action_id());

    // Send action to AMI; this will kick off creation of reaction pipe result
    writer_->write(action.to_string());

    // If response isn't complete before timeout then raise an exception, however we can't raise an exception here
    // otherwise the future will freak out causing an additional exceptions to be raised at the time of program
    // termination (an std::broken_promise exception when the application terminates and
    // promise::set_value()/future::get() wasn't invoked). In order to avoid that poke the exception into the promise,
    // this will cause future::get() to raise the exception resulting in only one exception being raised.
    std::runtime_error const err(fmt::format("Event timeout: Timeout waiting for event; ActionID={}", action.get_action_id()));
    if (auto const status = reaction.wait_for(timeout); status == std::future_status::timeout) {
        dispatcher_->set_exception_on_pipe(action.get_action_id(), std::make_exception_ptr(err));
    }

    // Return event
    return reaction.get();
}
