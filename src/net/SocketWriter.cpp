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
}

void SocketWriter::write(std::string_view buf) const
{
    socket_->write(buf);
}
