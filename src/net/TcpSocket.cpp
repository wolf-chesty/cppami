// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/net/TcpSocket.hpp"

#include "c++ami/CppAmiDefs.h"
#include "c++ami/util/ScopeGuard.hpp"
#include <arpa/inet.h>
#include <cassert>
#include <fmt/core.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace cpp_ami::net;

TcpSocket::TcpSocket(std::string_view hostname, uint16_t port)
    : sock_fd_(open(hostname, port))
{
    if (hostname.empty()) {
        throw std::invalid_argument("Invalid hostname");
    }

    if (port == 0) {
        throw std::out_of_range("Invalid port");
    }
}

TcpSocket::~TcpSocket()
{
    std::scoped_lock const lock(read_mutex_, write_mutex_);
    TcpSocket::close(std::exchange(sock_fd_, -1));
}

int TcpSocket::open(std::string_view hostname, uint16_t port)
{
    auto sock_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock_fd == -1) {
        throw std::runtime_error(fmt::format("Error opening socket: {}", strerror(errno)));
    }

    util::ScopeGuard sock_fd_scope([&sock_fd]() -> void {
        if (sock_fd != -1) {
            ::close(sock_fd);
        }
    });

    constexpr int reuse_on = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_on, sizeof(reuse_on)) == -1) {
        // TODO: issue a warning
    }

    addrinfo hints = {
        .ai_flags = AI_ADDRCONFIG,
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM
    };
    addrinfo *res{};
    if (getaddrinfo(hostname.data(), nullptr, &hints, &res) != 0) {
        throw std::runtime_error(fmt::format("Invalid hostname {}", hostname.data()));
    }

    util::ScopeGuard addrinfo_scope([res]() -> void { freeaddrinfo(res); });

    if (res->ai_addr == nullptr) {
        throw std::runtime_error(fmt::format("Invalid hostname {}", hostname.data()));
    }

    switch (res->ai_addr->sa_family) {
    case AF_INET:
        reinterpret_cast<sockaddr_in *>(res->ai_addr)->sin_port = htons(port);
        break;
    case AF_INET6:
        reinterpret_cast<sockaddr_in6 *>(res->ai_addr)->sin6_port = htons(port);
        break;
    default:
        throw std::runtime_error(fmt::format("Invalid address for hostname {}", hostname.data()));
    }

    if (connect(sock_fd, res->ai_addr, res->ai_addrlen) == -1) {
        throw std::runtime_error(fmt::format("Unable to connect socket: {}", strerror(errno)));
    }

    return std::exchange(sock_fd, -1);
}

void TcpSocket::close(int sock_fd)
{
    if (sock_fd != -1) {
        ::close(sock_fd);
    }
}

std::string TcpSocket::read(uint16_t buf_size, timeout_t timeout)
{
    std::unique_lock const lock(read_mutex_);

    if (sock_fd_ == -1) {
        return "";
    }

    // Wait for incoming message
    pollfd fds{};
    fds.fd = sock_fd_;
    fds.events = POLLIN;
    // Clamp timeout
    auto const timeout_ms = static_cast<int>(std::max(timeout.count(), {0}));
    // Wait for data; avoid waiting indefinitely
    auto const ret = poll(&fds, 1, timeout_ms);
    if (ret == -1) {
        throw std::runtime_error(fmt::format("Error reading socket: {}", strerror(errno)));
    }
    else if (ret == 0) {            // poll timeout
        return "";
    }
    // fds.revents == POLLIN means data is available for read
    if (fds.revents != POLLIN) {
        return "";
    }

    // Rather than a read/copy just read the message directly into the string
    buf_size = std::clamp(buf_size, {1024}, {65535});
    std::string buffer(buf_size, '\0');

    // Read incoming message
    auto const bytes_received = recv(sock_fd_, buffer.data(), buffer.size(), 0);
    if (bytes_received == -1) {
        throw std::runtime_error(fmt::format("Error reading socket: {}", strerror(errno)));
    }
    // Shrink the string to fit the message
    buffer.resize(bytes_received);

    return buffer;
}

void TcpSocket::write(std::string_view data)
{
    if (data.empty()) {
        return;
    }

    std::unique_lock const lock(write_mutex_);

    if (sock_fd_ == -1) {
        return;
    }

    auto const write_size = send(sock_fd_, data.data(), data.size(), 0);
    if (write_size == -1) {
        throw std::runtime_error(fmt::format("Error writing socket: {}", strerror(errno)));
    }
}
