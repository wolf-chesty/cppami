// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#ifndef NET_SOCKETWRITER_HPP
#define NET_SOCKETWRITER_HPP

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace cpp_ami::net {

class TcpSocket;

/// @class SocketWriter
///
/// @brief Provides an interface for writing to a socket.
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

    ~SocketWriter();

    SocketWriter &operator=(SocketWriter const &) = delete;
    SocketWriter &operator=(SocketWriter &&) = delete;

    /// @brief Writes \c buf to the socket.
    ///
    /// @param buf Data to write to socket.
    void write(std::string buf);

protected:
    /// @brief Starts the write thread.
    void startWriteThread();
    /// @brief Stops the write thread;
    void stopWriteThread();
    /// @brief Write thread.
    void writeThread();

private:
    socket_ptr_t socket_; ///< Socket to write to.

    std::vector<std::string> write_queue_;      ///< Collection of items to write.
    std::mutex write_queue_mut_;                ///< Mutex controlling access to buf queue.
    std::condition_variable write_thread_cv_;   ///< Condition to wake write thread.
    std::atomic<bool> write_thread_run_{false}; ///< Flag controlling write thread.
    std::thread write_thread_;                  ///< Handle to write thread.
};

} // namespace cpp_ami::net

#endif
