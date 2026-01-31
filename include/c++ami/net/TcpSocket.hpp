// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#ifndef NET_TCPSOCKET_HPP
#define NET_TCPSOCKET_HPP

#include <chrono>
#include <cstdint>
#include <mutex>
#include <string>
#include <string_view>

namespace cpp_ami::net {

///
/// @class TcpSocket
///
/// @brief Controls the lifetime of a socket file descriptor and allows thread-safe reading and writing to the managed
///        socket.
///
class TcpSocket {
public:
    using timeout_t = std::chrono::milliseconds;

public:
    TcpSocket() = delete;
    TcpSocket(TcpSocket const &) = delete;
    TcpSocket(TcpSocket &&) = delete;

    /// \brief Creates a new socket file descriptor attached to port \c of \c hostname.
    ///
    /// \param hostname Hostname to connect socket to.
    /// \param port Port to attach socket to on remote host.
    explicit TcpSocket(std::string_view hostname, uint16_t port = 5038);

    /// \brief dtor.
    ///
    /// Closes the socket file descriptor.
    virtual ~TcpSocket();

    TcpSocket& operator=(TcpSocket const &) = delete;
    TcpSocket& operator=(TcpSocket &&) = delete;

    /// @brief Reads data from the socket.
    ///
    /// @param buf_size Number of bytes to try and read from the socket. Range is [1024, 65535].
    /// @param timeout Period of time before read stops waiting for data.
    ///
    /// @return Data read from the socket.
    ///
    /// This function will wait for \c timeout milliseconds for input on the socket. If the timeout lapses and there
    /// isn't data available on the socket the function will return an empty buffer. If there is data available on the
    /// socket the function will read all available bytes (up to \c buf_size) from the socket and return the read data
    /// in a string.
    std::string read(uint16_t buf_size = 4096, timeout_t timeout = timeout_t{500});

    /// @brief Writes data to the socket.
    ///
    /// @param data Data to write to the socket.
    ///
    /// Provides a thread-safe write to the underlying socket file descriptor. This function will block until the socket
    /// is free for write.
    void write(std::string_view data);

private:
    /// @brief Closes socket \c sock_fd.
    ///
    /// @param sock_fd File descriptor for socket.
    static void close(int sock_fd);

    /// @brief Opens a new socket and returns the new socket file descriptor.
    ///
    /// @return Socket file descriptor.
    ///
    /// @param hostname Hostname to attach socket to.
    /// @param port Port of hostname to attach socket to.
    int open(std::string_view hostname, uint16_t port);

    std::mutex read_mutex_;     ///< Mutex used to control read access.
    std::mutex write_mutex_;    ///< Mutex used to control write access..

    int sock_fd_{-1};           ///< Socket file descriptor.
};

}

#endif
