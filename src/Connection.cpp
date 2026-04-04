// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/Connection.hpp"

#include "c++ami/action/Action.hpp"
#include "c++ami/net/SocketReader.hpp"
#include "c++ami/net/SocketWriter.hpp"
#include "c++ami/net/TcpSocket.hpp"
#include "c++ami/StreamParser.hpp"
#include <algorithm>
#include <cassert>
#include <execution>
#include <fmt/core.h>
#include <string_view>

using namespace cpp_ami;

Connection::Connection(std::string_view hostname, uint16_t port)
{
    auto sock = std::make_shared<net::TcpSocket>(hostname, port);

    dispatcher_ = std::make_unique<EventDispatcher>(
        [this](EventDispatcher::event_ptr_t dict) -> void { dispatchHandler(std::move(dict)); });

    stream_parser_ = std::make_unique<StreamParser>(
        [this](std::string ami_version) -> void { ami_version_ = std::move(ami_version); },
        [this](std::string event) -> void { dispatcher_->addEvent(std::move(event)); });

    reader_ = std::make_unique<net::SocketReader>(
        sock, [this](std::string buf) -> void { stream_parser_->addBuf(std::move(buf)); });

    writer_ = std::make_unique<net::SocketWriter>(sock);

    events_.reserve(1000);
    startDispatchThread();
}

Connection::~Connection()
{
    // Make sure objects get deleted in correct order
    reader_.reset();
    writer_.reset();
    stream_parser_.reset();
    dispatcher_.reset();
    stopDispatchThread();
}

std::string Connection::getAmiVersion() const
{
    return ami_version_;
}

void Connection::dispatchHandler(EventDispatcher::event_ptr_t dict)
{
    std::lock_guard const lock(events_mut_);
    events_.push_back(std::move(dict));
    events_cv_.notify_one();
}

Connection::event_callback_key_t Connection::addCallback(event_callback_t callback)
{
    auto const id = action::Action::createUuid();
    std::lock_guard const lock(callbacks_mutex_);
    callbacks_.emplace(id, std::move(callback));
    return id;
}

void Connection::removeCallback(event_callback_key_t const &key)
{
    std::lock_guard const lock(callbacks_mutex_);
    callbacks_.erase(key);
}

void Connection::asyncInvoke(action::Action const &action) const
{
    writer_->write(action.toString());
}

Connection::reaction_ptr_t Connection::invoke(action::Action const &action) const
{
    auto reaction = dispatcher_->getEventPipe(action.getActionId());

    // Send action to AMI; this will kick off creation of reaction pipe result
    writer_->write(action.toString());

    // Wait for and return event
    return reaction.get();
}

Connection::reaction_ptr_t Connection::invoke(action::Action const &action,
                                              std::chrono::milliseconds const &timeout) const
{
    auto reaction = dispatcher_->getEventPipe(action.getActionId());

    // Send action to AMI; this will kick off creation of reaction pipe result
    writer_->write(action.toString());

    // If response isn't complete before timeout then raise an exception, however we can't raise an exception here
    // otherwise the future will freak out causing an additional exceptions to be raised at the time of program
    // termination (an std::broken_promise exception when the application terminates and
    // promise::setValue()/future::get() wasn't invoked). In order to avoid that poke the exception into the promise,
    // this will cause future::get() to raise the exception resulting in only one exception being raised.
    std::runtime_error const err(
        fmt::format("Event timeout: Timeout waiting for event; ActionID={}", action.getActionId()));
    if (auto const status = reaction.wait_for(timeout); status == std::future_status::timeout) {
        dispatcher_->setExceptionOnPipe(action.getActionId(), std::make_exception_ptr(err));
    }

    // Return event
    return reaction.get();
}

void Connection::startDispatchThread()
{
    event_dispatch_thread_run_ = true;
    event_dispatch_thread_ = std::thread(&Connection::dispatchThread, this);

    pthread_setname_np(event_dispatch_thread_.native_handle(), "conn_dispatch");
}

void Connection::stopDispatchThread()
{
    event_dispatch_thread_run_ = false;
    events_cv_.notify_one();

    assert(event_dispatch_thread_.joinable());
    event_dispatch_thread_.join();
}

void Connection::dispatchThread()
{
    decltype(events_) events;
    events.reserve(events_.capacity());

    while (event_dispatch_thread_run_) {
        std::unique_lock lock(events_mut_);
        events_cv_.wait(lock, [this]() -> bool { return !event_dispatch_thread_run_ || !events_.empty(); });
        std::swap(events_, events);
        lock.unlock();

        std::lock_guard const callback_lock(callbacks_mutex_);
        for (auto const &event : events) {
            std::for_each(std::execution::par, callbacks_.begin(), callbacks_.end(), [&event](auto const &it) {
                if (event) {
                    it.second(*event);
                }
            });
        }
        events.clear();
    }

    // Finish up dispatching events
    std::lock_guard const events_lock(events_mut_);
    std::lock_guard const callback_lock(callbacks_mutex_);
    for (auto const &event : events_) {
        std::for_each(std::execution::par, callbacks_.begin(), callbacks_.end(), [&event](auto const &it) {
            if (event) {
                it.second(*event);
            }
        });
    }
    events_.clear();
}
