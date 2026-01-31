// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#ifndef AMI_EVENT_HPP
#define AMI_EVENT_HPP

#include "c++ami/util/KeyValDict.hpp"

namespace cpp_ami::event {

///
/// @class Event
///
/// @brief Contains data for an AMI event that isn't associated to an AMI action.
///
/// This object is an interface for an AMI event object that is not associated with an AMI action. By inheriting
/// unmatched events from this base class it can be a bit easier writing polymorphic code taking advantage of the type
/// of this object.
///
class Event
    : public util::KeyValDict {
public:
    Event() = delete;
    Event(Event const &) = default;
    Event(Event &&) noexcept = default;

    /// @brief Creates an object with the contents of a \c KeyValDict.
    ///
    /// @param dict \c KeyValDict object containing event values.
    explicit Event(KeyValDict dict) noexcept;

    ~Event() override = default;

    Event& operator=(Event const &) = default;
    Event& operator=(Event &&) noexcept = default;
};

}

#endif
