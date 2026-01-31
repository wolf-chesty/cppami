// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/net/SocketReader.hpp"

#include "c++ami/net/TcpSocket.hpp"
#include <cassert>
#include <utility>

using namespace cpp_ami::net;

SocketReader::SocketReader(socket_ptr_t socket, handler_t callback)
    : callback_(std::move(callback))
    , socket_(std::move(socket))
{
    assert(socket_);
    start_work_thread();
}

SocketReader::~SocketReader()
{
    stop_work_thread();
}

void SocketReader::start_work_thread()
{
    thread_spin_ = true;
    thread_ = std::thread(&SocketReader::work_thread, this);

    std::string_view thread_name("ami_reader");
    assert(thread_name.length() <= 16);
    pthread_setname_np(thread_.native_handle(), thread_name.data());
}

void SocketReader::stop_work_thread()
{
    thread_spin_ = false;

    assert(thread_.joinable());
    thread_.join();
}

void SocketReader::work_thread() const
{
    while (thread_spin_) {
        if (auto buf = socket_->read(); !buf.empty()) {
            callback_(std::move(buf));
        }
    }
}