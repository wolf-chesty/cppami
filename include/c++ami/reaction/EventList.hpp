// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#ifndef REACTION_EVENTLIST_HPP
#define REACTION_EVENTLIST_HPP

#include "c++ami/reaction/Reaction.hpp"

#include "c++ami/event/Event.hpp"
#include <memory>
#include <vector>

namespace cpp_ami {
namespace reaction {

/// @class EventList
///
/// @brief Represents an AMI EventList.
///
/// An AMI EventList is a collection of events that are sent back by AMI for actions that have multiple responses
/// associated with them. This object collects events associated with an ActionID into a single object.
///
/// This list consists of a "head" event which is the first event received that declares that what follows is a list of
/// Events in response to an action. Subsequent events are added to the \c m_events collection. The final event (Event
/// with a EventList of 'completed') will be placed in the \c m_tail object.
///
/// For responses that fail the first AMI event for the response will have a Status of 'failed' or an EventList value
/// of 'canceled'. In these instances \c m_events will be empty and \c m_tail will be a nullptr and \c m_head will
/// contain the only returned AMI event for the failed response.
///
class EventList : public Reaction {
public:
    EventList() = delete;
    EventList(EventList const &right);
    EventList(EventList &&) noexcept = default;

    /// @brief Creates an object using \c event as the head of the EventList.
    ///
    /// @param event Head event for the object.
    explicit EventList(event::Event event) noexcept;

    /// @brief Creates an object using \c dict as the head of the EventList.
    ///
    /// @param dict Data for the head event of the object.
    explicit EventList(util::KeyValDict dict) noexcept;
    ~EventList() override = default;

    EventList& operator=(EventList const &right);
    EventList& operator=(EventList &&) noexcept = default;

    /// @brief Adds \c event to the object.
    ///
    /// @param event Event that is a member of this object.
    bool add_event(event::Event event);

    /// @brief Adds \c dict data to the object.
    ///
    /// @param dict \c KeyValDict containing data for the new event to add to this object.
    bool add_event(util::KeyValDict dict);

    // cpp_ami::reaction::Reaction interface
    bool is_success() const override;
    std::string to_string() const override;

    size_t event_count() const;
    event::Event const& get_event(size_t event_idx) const;

private:
    static bool is_list_complete(std::string const &event_list_val);

    event::Event head_;                    ///< The first event in the AMI events that make up this object.
    std::unique_ptr<event::Event const> tail_;   ///< The last event in the AMI events that make up this object.
    std::vector<event::Event> events_;     ///< AMI events that make up this object.
};

}
}

#endif
