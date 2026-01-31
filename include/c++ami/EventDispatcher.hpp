// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#ifndef AMI_EVENT_DISPATCHER_HPP
#define AMI_EVENT_DISPATCHER_HPP

#include "c++ami/reaction/Reaction.hpp"
#include "c++ami/event/Event.hpp"
#include <atomic>
#include <condition_variable>
#include <exception>
#include <functional>
#include <future>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

namespace cpp_ami {

namespace reaction {
    class Reaction;
    class EventList;
}

///
/// @class EventDispatcher
///
/// @brief Dispatches event messages received from the AMI server.
///
/// There are two types of AMI event messages that the AMI server can publish to AMI client applications:
///
///   - Notification Events : events raised by other components on the Asterisk server and
///   - Response Events :  events raised by this process sending an Action to the Asterisk server
///
/// This class is responsible for identifying notification events and dispatching those to dispatch functions that can
/// process the received event.
///
/// This class is also responsible for returning response events to callers of the invoke functions. Response events can
/// be bundled together into more complex system event messages to be processed by the client application.
///
class EventDispatcher {
public:
    // The following are typedefs for objects used by the function based event handler. Events of this type
    // will be dispatched via the dispatch function.
    using event_t = util::KeyValDict;
    using event_ptr_t = std::unique_ptr<event_t const>;
    using event_callback_t = std::function<void(event_ptr_t)>;

    // The following are typedefs for used by the promise/future interface when returning AMI events in a
    // synchronous manner.
    using reaction_t = reaction::Reaction;
    using reaction_ptr_t = std::unique_ptr<reaction_t const>;
    using pipe_t = std::promise<reaction_ptr_t>;

public:
    EventDispatcher() = delete;
    EventDispatcher(EventDispatcher const &) = delete;
    EventDispatcher(EventDispatcher &&) noexcept = delete;

    /// @brief Constructs the object with \c callback as the function to invoke when new notification events
    ///        are received.
    ///
    /// @param callback Callback to invoke when new notification events are received.
    explicit EventDispatcher(event_callback_t callback);

    virtual ~EventDispatcher();

    EventDispatcher& operator=(EventDispatcher const &) = delete;
    EventDispatcher& operator=(EventDispatcher &&) noexcept = delete;

    /// @brief Adds a new incoming event to be dispatched.
    ///
    /// @param event New event to dispatch to callers of invoke or to the callback function.
    void add_event(std::string event);

    /// @brief Creates a waitable pipe identified by \c action_id.
    ///
    /// @return \c future to receive response event on.
    ///
    /// @param action_id Action ID of an action.
    ///
    /// This function can be used by a caller to create a new response event pipe that events can be returned on.
    /// Invoking this function using the action ID of the action to be sent to the AMI server will create a
    /// promise/future pipe pair that this object will use to send the response event back on. This allows the client
    /// application to behave in a more synchronous manner since all messages belonging to an action response appear to
    /// be immediately returned from the AMI server at once.
    [[nodiscard]] std::future<reaction_ptr_t> get_event_pipe(std::string const &action_id);

    /// @brief Sets an exception on a pipe.
    ///
    /// @param action_id Action ID of the pipe to set the exception on.
    /// @param err Exception to set on the pipe.
    ///
    /// In some instances you may want to force a pipe to close immediately with an error. Invoking this function will
    /// set a future exception on a promise/future pipe and free any memory for cached response events being created.
    ///
    /// A promise/future pair cannot be destroyed unless promise::set_value or promise::set_exception and future::get
    /// have been invoked. If these functions haven't been invoked then the application will throw an
    /// std::broken_promise exception upon destruction of either the promise and/or future.
    void set_exception_on_pipe(std::string const &action_id, std::exception_ptr const &err);

    /// @brief Forces a null value through a pipe in order to force it closed.
    ///
    /// @param action_id Action ID of the pipe send a null value through.
    ///
    /// Much like \c set_exception_on_pipe but sends a nullptr on the promise/future pipe causing the future::get
    /// function to return immediately.
    ///
    /// This won't raise an exception but the client code invoking future::get will need to check the received value to
    /// be non-null.
    void set_null_on_pipe(std::string const &action_id);

private:
    /// @brief Starts the work thread.
    void start_work_thread();

    /// @brief Stops the work thread.
    void stop_work_thread();

    /// @brief Work thread for this object.
    ///
    /// This thread is responsible for parsing incoming Events messages and dispatching them to the \c dispatch_
    /// function or returning events via promise/future pipes to awaiting AMI clients.
    void work_thread();

    /// @brief Dispatches an AMI message in string format.
    ///
    /// @param event_buf String containing an AMI string event.
    void dispatch_event(std::string event_buf);

    /// @brief Dispatches a response event.
    ///
    /// @return \c true if the response event was dispatched over a promise/future pipe.
    ///
    /// @param action_id Action ID for \c dict.
    /// @param dict Collection of key/value pairs that make up the response event.
    bool dispatch_event(std::string const &action_id, util::KeyValDict &dict);

    /// @brief Cleans up the object on destruction.
    ///
    /// This object iterates through all of the promise pipe ends and sends nullptr through them and closes them. This
    /// function will also free any memory allocated for working multipart response events.
    void cleanup_object();

    std::vector<std::string> events_;                           ///< Events received from AMI.
    std::mutex events_mutex_;                                   ///< Mutex controlling access to event collection.

    std::thread thread_;                                        ///< Handle to working thread.
    std::atomic<bool> thread_run_{ false };                     ///< Flag to stop working thread.
    std::condition_variable thread_cv_;                         ///< Condition variable used to wake working thread on receipt of new events.

    event_callback_t dispatch_{ [](event_ptr_t) -> void {} };   ///< Dispatch function to call on non-response events.

    std::unordered_map<std::string, pipe_t> promise_map_;       ///< Promise map to return events on.
    std::mutex promise_map_mutex_;                              ///< Mutex to control access to promise collection.

    std::unordered_map<std::string, std::unique_ptr<reaction::EventList>> event_map_;   ///< Event map to store working events in. Some events are made up of multiple event messages; in-progress messages are stored here until ready for dispatch.
    std::mutex event_map_mutex_;                                ///< Mutex to control access to in-progress event collection.
};

}

#endif