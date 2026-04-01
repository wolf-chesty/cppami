// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/net/SocketWriter.hpp"

#include "c++ami/net/TcpSocket.hpp"
#include <cassert>
#include <utility>

using namespace cpp_ami::net;

SocketWriter::SocketWriter(socket_ptr_t socket)
    : socket_(std::move(socket))
{
    assert(socket_);

    write_queue_.reserve(100);
    startWriteThread();
}

SocketWriter::~SocketWriter()
{
    stopWriteThread();
}

void SocketWriter::write(std::string buf)
{
    if (write_thread_run_) {
        std::lock_guard const lock(write_queue_mut_);
        write_queue_.emplace_back(std::move(buf));
        socket_->write(buf);
        write_thread_cv_.notify_one();
    }
}

void SocketWriter::startWriteThread()
{
    write_thread_run_ = true;
    write_thread_ = std::thread(&SocketWriter::writeThread, this);

    std::string_view thread_name("write_thread");
    assert(thread_name.length() <= 16);
    pthread_setname_np(write_thread_.native_handle(), thread_name.data());
}

void SocketWriter::stopWriteThread()
{
    write_thread_run_ = false;
    write_thread_cv_.notify_one();

    assert(write_thread_.joinable());
    write_thread_.join();
}

void SocketWriter::writeThread()
{
    decltype(write_queue_) write_queue;
    write_queue.reserve(write_queue_.size());

    while (write_thread_run_) {
        std::unique_lock lock(write_queue_mut_);
        write_thread_cv_.wait(lock, [this]() -> bool { return !write_thread_run_ || !write_queue_.empty(); });

        std::swap(write_queue_, write_queue);
        lock.unlock();

        for (auto const &buf : write_queue) {
            socket_->write(buf);
        }
        write_queue.clear();
    }

    std::unique_lock lock(write_queue_mut_);
    for (auto const &buf : write_queue_) {
        socket_->write(buf);
    }
}
