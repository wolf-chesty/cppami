// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#ifndef NET_SOCKETREADER_HPP
#define NET_SOCKETREADER_HPP

#include <atomic>
#include <functional>
#include <memory>
#include <string>
#include <thread>

namespace cpp_ami::net {

class TcpSocket;

///
/// @class SocketReader
///
/// @brief Starts a thread that reads data from a socket object.
///
class SocketReader {
public:
    using handler_t = std::function<void(std::string)>;

    using socket_t = net::TcpSocket;
    using socket_ptr_t = std::shared_ptr<socket_t>;

public:
    SocketReader() = delete;
    SocketReader(SocketReader const &) = delete;
    SocketReader(SocketReader &&) = delete;

    /// @brief Constructs a new object that will read data from \c socket, when data is received callback
    ///        \c handler will be invoked with the received data.
    ///
    /// @param socket Socket to read data from.
    /// @param callback Callback function to invoke on received data.
    explicit SocketReader(socket_ptr_t socket, handler_t callback);

    /// @brief Stops the thread.
    virtual ~SocketReader();

    SocketReader& operator=(SocketReader const &) = delete;
    SocketReader& operator=(SocketReader &&) = delete;

private:
    /// @brief Starts the data reader work thread.
    void start_work_thread();

    /// @brief Stops the data reader work thread.
    void stop_work_thread();

    /// @brief Implements the work thread responsible for reading data from the socket and invoking the
    ///        data callback on received data.
    void work_thread() const;

    handler_t callback_{ [](std::string) -> void { } }; ///< Callback that will be invoked whenever new data as been read from \c m_socket.

    std::thread thread_;                        ///< Handle to data read thread.
    std::atomic<bool> thread_spin_{ false };    ///< Flag indicating if flag is still running.

    socket_ptr_t socket_;       ///< Socket to read data from.
};

}

#endif
