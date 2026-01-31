// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/reaction/EventList.hpp"

#include <cassert>

using namespace cpp_ami::reaction;

EventList::EventList(EventList const &right)
    : head_(right.head_)
    , tail_(new event::Event(*right.tail_))
    , events_(right.events_)
{
}

EventList::EventList(event::Event event) noexcept
    : head_(std::move(event))
{
}

EventList::EventList(util::KeyValDict dict) noexcept
    : head_(std::move(dict))
{
}

EventList &EventList::operator=(EventList const &right)
{
    head_ = right.head_;
    tail_ = std::make_unique<event::Event>(*right.tail_);
    events_ = right.events_;
    return *this;
}

bool EventList::is_success() const
{
    auto const response = head_.get_value("Response");
    return response && Reaction::is_success(*response);
}

bool EventList::is_list_complete(std::string const &event_list_val)
{
    return event_list_val == "Complete" || event_list_val == "cancelled";
}

bool EventList::add_event(event::Event event)
{
    if (auto const val = event.get_value("EventList"); val && is_list_complete(*val)) {
        tail_ = std::make_unique<event::Event>(std::move(event));
        return true;
    }

    events_.push_back(std::move(event));
    return false;
}

bool EventList::add_event(util::KeyValDict dict)
{
    return add_event(event::Event(std::move(dict)));
}

std::string EventList::to_string() const
{
    std::string result = head_.to_string();
    for (auto const &event : events_) {
        result += event.to_string();
    }
    if (tail_) {
        result += tail_->to_string();
    }
    return result;
}

size_t EventList::event_count() const
{
    return events_.size();
}

cpp_ami::event::Event const& EventList::get_event(size_t event_idx) const
{
    assert(event_idx < events_.size());
    return events_[event_idx];
}
