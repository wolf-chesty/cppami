// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/EventDispatcher.hpp"

#include "c++ami/reaction/Event.hpp"
#include "c++ami/reaction/EventList.hpp"
#include <cassert>

using namespace cpp_ami;

EventDispatcher::EventDispatcher(event_callback_t callback)
    : dispatch_(std::move(callback))
{
    events_.reserve(100);

    startWorkThread();
}

EventDispatcher::~EventDispatcher()
{
    stopWorkThread();

    cleanupObject();
}

void EventDispatcher::cleanupObject()
{
    std::scoped_lock const lock(promise_map_mutex_, event_map_mutex_);

    // Not sure what to do here; sending nullptr's out on pipes to avoid std::broken_promise exception on terminate
    assert(promise_map_.empty());
    for (auto &[_, pipe] : promise_map_) {
        pipe.set_value(nullptr);
    }

    assert(event_map_.empty());
}

void EventDispatcher::startWorkThread()
{
    thread_run_ = true;
    thread_ = std::thread(&EventDispatcher::workThread, this);

    std::string_view thread_name("ami_dispatcher");
    assert(thread_name.length() <= 16);
    pthread_setname_np(thread_.native_handle(), thread_name.data());
}

void EventDispatcher::stopWorkThread()
{
    thread_run_ = false;
    thread_cv_.notify_one();

    assert(thread_.joinable());
    thread_.join();
}

void EventDispatcher::workThread()
{
    decltype(events_) events;
    events.reserve(events_.capacity());

    while (thread_run_) {
        std::unique_lock lock(events_mutex_);
        thread_cv_.wait(lock, [this]() -> bool { return !thread_run_ || !events_.empty(); });

        std::swap(events_, events);
        lock.unlock();

        for (auto event_buf : events) {
            dispatchEvent(std::move(event_buf));
        }
        events.clear();
    }

    // Finish building events
    std::lock_guard const lock(events_mutex_);
    for (auto event_buf : events_) {
        dispatchEvent(std::move(event_buf));
    }
    events_.clear();
}

void EventDispatcher::dispatchEvent(std::string event_buf)
{
    util::KeyValDict dict(std::move(event_buf));
    if (auto const action_id = dict.getValue("ActionID"); !action_id || !dispatchEvent(action_id.value(), dict)) {
        // Event is either missing the action ID or isn't in response to an AMI action; dispatch a regular event
        dispatch_(std::make_unique<event::Event const>(std::move(dict)));
    }
}

bool EventDispatcher::dispatchEvent(std::string const &action_id, util::KeyValDict &dict)
{
    std::scoped_lock const lock(promise_map_mutex_, event_map_mutex_);

    // Grab pipe for variable return
    auto const p_it = promise_map_.find(action_id);
    // Nothing is waiting for the Event; let normal dispatch handler handle this
    if (p_it == promise_map_.end()) {
        return false;
    }
    auto &pipe = p_it->second;

    // Grab iterator for EventList associated with action_id
    auto const e_it = event_map_.find(action_id);

    // Event is (currently?) not part of an EventList
    if (e_it == event_map_.end()) {
        // Event does not start an EventList; immediately return Event and clear pipe
        if (!dict.hasKey("EventList")) {
            pipe.set_value(std::make_unique<reaction::Event const>(std::move(dict)));
            promise_map_.erase(p_it);
        }
        // Event creates EventList; create new EventList and check AMI status
        else if (auto event_list = std::make_unique<reaction::EventList>(std::move(dict)); event_list->isSuccess()) {
            event_map_.emplace(action_id, std::move(event_list));
        }
        // EventList creation failed; immediately return EventList and clear pipe
        else {
            pipe.set_value(std::move(event_list));
            promise_map_.erase(p_it);
        }
        return true;
    }

    // Event is part of an EventList; grab working EventList and append Event
    if (auto &event_list = e_it->second; event_list->addEvent(std::move(dict))) {
        // EventList is complete; return EventList
        pipe.set_value(std::move(event_list));
        // Clear pipe and cached EventList
        promise_map_.erase(p_it);
        event_map_.erase(e_it);
    }

    return true;
}

void EventDispatcher::addEvent(std::string event)
{
    std::lock_guard const lock(events_mutex_);
    events_.push_back(std::move(event));
    thread_cv_.notify_one();
}

std::future<EventDispatcher::reaction_ptr_t> EventDispatcher::getEventPipe(std::string const &action_id)
{
    // Create new promise/future pair for event return
    pipe_t promise;
    auto future = promise.get_future();

    // Add promise for return event
    std::lock_guard const lock(promise_map_mutex_);
    promise_map_.emplace(action_id, std::move(promise));

    return future;
}

void EventDispatcher::setExceptionOnPipe(std::string const &action_id, std::exception_ptr const &err)
{
    std::scoped_lock const lock(promise_map_mutex_, event_map_mutex_);

    // Invoke set_exception on the promise end of the pipe so that this end of the pipe can be closed
    if (auto const it = promise_map_.find(action_id); it != promise_map_.end()) {
        it->second.set_exception(err);
        promise_map_.erase(it);
    }

    // Clear the working reaction
    event_map_.erase(action_id);
}

void EventDispatcher::setNullOnPipe(std::string const &action_id)
{
    std::scoped_lock const lock(promise_map_mutex_, event_map_mutex_);

    // Invoke set_value on the promise end of the pipe so that this end of the pipe can be closed
    if (auto const it = promise_map_.find(action_id); it != promise_map_.end()) {
        it->second.set_value(nullptr);
        promise_map_.erase(it);
    }

    // Clear the working reaction
    event_map_.erase(action_id);
}
