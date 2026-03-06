// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include <boost/test/unit_test.hpp>

#include <sys/socket.h>
#include <netinet/in.h>
#include <memory>
#include "c++ami/net/TcpSocket.hpp"
#include "c++ami/util/ScopeGuard.hpp"
#include <future>
#include <thread>

BOOST_AUTO_TEST_SUITE(cpp_ami_tests)

BOOST_AUTO_TEST_SUITE(socket_tests)

BOOST_AUTO_TEST_CASE(tcp_write_test)
{
    using namespace std::chrono_literals;
    using namespace cpp_ami;

    std::promise<void> server_promise;
    auto server_future = server_promise.get_future();

    uint16_t const port = 9999;
    std::thread server_thread([server_promise = std::move(server_promise), port]() mutable -> void {
        auto server_fd = socket(AF_INET, SOCK_STREAM, 0);
        BOOST_CHECK(server_fd > 0);
        util::ScopeGuard scope_fd_guard([server_fd]() -> void { close(server_fd); });

        int opt = 1;
        auto ret = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
        BOOST_CHECK(ret == 0);

        sockaddr_in address{0};
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        ret = bind(server_fd, reinterpret_cast<sockaddr*>(&address), sizeof(address));
        BOOST_CHECK(ret >= 0);

        ret = listen(server_fd, 3);
        BOOST_CHECK(ret >= 0);

        int addrlen = sizeof(address);
        server_promise.set_value();
        auto new_socket = accept(server_fd, reinterpret_cast<sockaddr*>(&address), reinterpret_cast<socklen_t*>(&addrlen));
        BOOST_CHECK(new_socket >= 0);
        util::ScopeGuard socket_guard([new_socket]() -> void { close(new_socket); });

        char ping[1024] = {0};
        read(new_socket, ping,  sizeof(ping));
        BOOST_CHECK((std::string("Ping 9876543210") == ping));
    });

    server_future.get();
    net::TcpSocket sock("127.0.0.1", port);
    sock.write("Ping 9876543210");

    server_thread.join();
}

BOOST_AUTO_TEST_CASE(tcp_read_test)
{
    using namespace std::chrono_literals;
    using namespace cpp_ami;

    std::promise<void> server_promise;
    auto server_future = server_promise.get_future();

    uint16_t const port = 9999;
    std::thread server_thread([server_promise = std::move(server_promise), port]() mutable -> void {
        auto server_fd = socket(AF_INET, SOCK_STREAM, 0);
        BOOST_CHECK(server_fd > 0);
        util::ScopeGuard scope_fd_guard([server_fd]() -> void { close(server_fd); });

        int opt = 1;
        auto ret = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
        BOOST_CHECK(ret == 0);

        sockaddr_in address{0};
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        ret = bind(server_fd, reinterpret_cast<sockaddr*>(&address), sizeof(address));
        BOOST_CHECK(ret >= 0);

        ret = listen(server_fd, 3);
        BOOST_CHECK(ret >= 0);

        int addrlen = sizeof(address);
        server_promise.set_value();
        auto new_socket = accept(server_fd, reinterpret_cast<sockaddr*>(&address), reinterpret_cast<socklen_t*>(&addrlen));
        BOOST_CHECK(new_socket >= 0);
        util::ScopeGuard socket_guard([new_socket]() -> void { close(new_socket); });

        char ping[] = "Pong 0123456789";
        send(new_socket, ping,  strlen(ping), 0);
    });

    server_future.get();
    net::TcpSocket sock("127.0.0.1", port);
    BOOST_CHECK(sock.read() == "Pong 0123456789");

    server_thread.join();
}

BOOST_AUTO_TEST_CASE(tcp_read_write_test)
{
    using namespace std::chrono_literals;
    using namespace cpp_ami;

    std::promise<void> server_promise;
    auto server_future = server_promise.get_future();

    uint16_t const port = 9999;
    std::thread server_thread([server_promise = std::move(server_promise), port]() mutable -> void {
        auto server_fd = socket(AF_INET, SOCK_STREAM, 0);
        BOOST_CHECK(server_fd > 0);
        util::ScopeGuard scope_fd_guard([server_fd]() -> void { close(server_fd); });

        int opt = 1;
        auto ret = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
        BOOST_CHECK(ret == 0);

        sockaddr_in address{0};
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        ret = bind(server_fd, reinterpret_cast<sockaddr*>(&address), sizeof(address));
        BOOST_CHECK(ret >= 0);

        ret = listen(server_fd, 3);
        BOOST_CHECK(ret >= 0);

        int addrlen = sizeof(address);
        server_promise.set_value();
        auto new_socket = accept(server_fd, reinterpret_cast<sockaddr*>(&address), reinterpret_cast<socklen_t*>(&addrlen));
        BOOST_CHECK(new_socket >= 0);
        util::ScopeGuard socket_guard([new_socket]() -> void { close(new_socket); });

        char ping[1024] = {0};
        read(new_socket, ping,  sizeof(ping));
        BOOST_CHECK((std::string("Ping 9876543210") == ping));

        char pong[] = "Pong 0123456789";
        send(new_socket, pong,  strlen(pong), 0);
    });

    server_future.get();
    net::TcpSocket sock("127.0.0.1", port);
    sock.write("Ping 9876543210");
    BOOST_CHECK(sock.read() == "Pong 0123456789");

    server_thread.join();
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
