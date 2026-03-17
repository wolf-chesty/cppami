// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include <gtest/gtest.h>

#include "c++ami/net/SocketReader.hpp"
#include "c++ami/net/TcpSocket.hpp"
#include "c++ami/util/ScopeGuard.hpp"
#include <future>
#include <memory>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>

TEST(socket_reader, read_test)
{
    using namespace std::chrono_literals;
    using namespace cpp_ami;

    std::promise<void> server_promise;
    auto server_future = server_promise.get_future();

    std::string const hostname{"127.0.0.1"};
    uint16_t const port{9999};
    std::string const pong{"Pong 0123456789"};

    std::thread server_thread([server_promise = std::move(server_promise), &pong]() mutable -> void {
        auto server_fd = socket(AF_INET, SOCK_STREAM, 0);
        EXPECT_GT(server_fd, 0);
        util::ScopeGuard scope_fd_guard([server_fd]() -> void { close(server_fd); });

        int opt = 1;
        auto ret = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
        EXPECT_EQ(ret, 0);

        sockaddr_in address{0};
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        ret = bind(server_fd, reinterpret_cast<sockaddr *>(&address), sizeof(address));
        EXPECT_GE(ret, 0);

        ret = listen(server_fd, 3);
        EXPECT_GE(ret, 0);

        int addrlen = sizeof(address);
        server_promise.set_value();
        auto new_socket =
            accept(server_fd, reinterpret_cast<sockaddr *>(&address), reinterpret_cast<socklen_t *>(&addrlen));
        EXPECT_GE(new_socket, 0);
        util::ScopeGuard socket_guard([new_socket]() -> void { close(new_socket); });

        send(new_socket, pong.c_str(), pong.size(), 0);
    });

    server_future.get();

    auto sock = std::make_shared<net::TcpSocket>(hostname, port);
    net::SocketReader reader(sock, [&pong](std::string buf) { EXPECT_EQ(buf, pong); });

    server_thread.join();
}
