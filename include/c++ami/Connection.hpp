// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#ifndef AMI_CONNECTION_HPP
#define AMI_CONNECTION_HPP

#include "c++ami/EventDispatcher.hpp"
#include <chrono>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

#include <quill/Logger.h>

namespace cpp_ami {

namespace action {
class Action;
}

namespace net {
class SocketReader;
class SocketWriter;
} // namespace net

class EventDispatcher;
class StreamParser;

///
/// @class Connection
///
/// @brief Manages the individual threads that manage communication with the AMI server and provides an interface for an
/// application to communicate with the AMI server.
///
/// This object constructs and manages the lifetime of objects that communicate with the AMI server. AMI client
/// applications will construct one of these objects to use as a handle to communicate to the AMI server.
///
class Connection {
public:
    using reaction_ptr_t = EventDispatcher::reaction_ptr_t;

    using event_callback_t = std::function<void(EventDispatcher::event_t const *)>;
    using event_callback_key_t = std::string;

public:
    Connection() = delete;
    Connection(Connection const &) = delete;
    Connection(Connection &&) noexcept = delete;

    /// @brief Constructs an object that is connected to \c port on the \c hostname machine.
    ///
    /// @param hostname Hostname of the AMI server to attach to.
    /// @param port Port number on the AMI server to attach to.
    explicit Connection(quill::Logger *logger, std::string_view hostname, uint16_t port = 5038);

    virtual ~Connection();

    Connection &operator=(Connection const &) = delete;
    Connection &operator=(Connection &&) noexcept = delete;

    /// @brief Returns the AMI server version.
    ///
    /// @return String containing AMI server version.
    std::string get_ami_version() const;

    /// @brief Sends \c action to the AMI server and immediately returns. The client application will need
    ///        to add callbacks to handle the responding events.
    ///
    /// @param action Action to send to the AMI server.
    void async_invoke(action::Action const &action) const;

    /// @brief Sends \c action to the AMI server and returns the resulting Event object. This call will block
    ///        indefinitely until all of the event stream is read back from the socket.
    ///
    /// @param action Action to send to the AMI server.
    reaction_ptr_t invoke(action::Action const &action) const;

    /// @brief Sends \c action to the AMI server and returns the resulting Event object. This call will block until
    ///        \c timeout has elapsed (at which point an exception is raised) or the event stream was read back from the
    ///        socket.
    ///
    /// @param action Action to send to the AMI server.
    /// @param timeout Amount of time to wait for the AMI server to fulfill the event request.
    reaction_ptr_t invoke(action::Action const &action, std::chrono::milliseconds const &timeout) const;

    /// @brief Adds an event callback to the collection of callbacks. These callbacks are invoked whenever async_invoke
    ///        is invoked or the AMI server is sending events out.
    ///
    /// @return Callback ID.
    ///
    /// @param callback Callback to invoke.
    event_callback_key_t add_callback(event_callback_t callback);

    /// @brief Removes an event callback from the collection of callbacks.
    ///
    /// @param id ID of callback to remove from the collection of event callbacks.
    void remove_callback(event_callback_key_t const &id);

private:
    /// @brief Invokes all event callbacks on \c dict.
    ///
    /// @param dict Event values.
    void dispatch_handler(EventDispatcher::event_ptr_t dict);

    std::string ami_version_;   ///< AMI version.

    std::unordered_map<event_callback_key_t, event_callback_t> callbacks_;  ///< Collection of event callbacks.
    std::mutex callbacks_mutex_;                                            ///< Mutex to guard \c m_callbacks collection.

    std::unique_ptr<EventDispatcher> dispatcher_;   ///< Object responsible for dispatching AMI events.
    std::unique_ptr<net::SocketReader> reader_;     ///< Object responsible for pulling messages from the AMI socket.
    std::unique_ptr<net::SocketWriter> writer_;     ///< Object responsible for writing messages to the AMI socket.
    std::unique_ptr<StreamParser> stream_parser_;   ///< Object responsible for parsing the socket stream into individual AMI messages.

    quill::Logger *logger_{nullptr};    ///< Pointer to the logger.
};

} // namespace cpp_ami

#endif
