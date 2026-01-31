// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#ifndef NET_SOCKETWRITER_HPP
#define NET_SOCKETWRITER_HPP

#include <memory>
#include <string_view>

namespace cpp_ami::net {

class TcpSocket;

///
/// @class SocketWriter
///
/// @brief Provides an interface for writing to a socket.
///
class SocketWriter {
public:
    using socket_t = net::TcpSocket;
    using socket_ptr_t = std::shared_ptr<socket_t>;

public:
    SocketWriter() = delete;
    SocketWriter(SocketWriter const &) = delete;
    SocketWriter(SocketWriter &&) = delete;

    /// @brief Constructs an object that will write to \c socket.
    ///
    /// @param socket Socket to write data to.
    explicit SocketWriter(socket_ptr_t socket);

    virtual ~SocketWriter() = default;

    SocketWriter& operator=(SocketWriter const &) = delete;
    SocketWriter& operator=(SocketWriter &&)  = delete;

    /// @brief Writes \c buf to the socket.
    ///
    /// @param buf Data to write to socket.
    void write(std::string_view buf) const;

private:
    socket_ptr_t socket_;   ///< Socket to write to.
};

}

#endif
