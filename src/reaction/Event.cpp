// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/reaction/Event.hpp"

using namespace cpp_ami::reaction;

Event::Event(KeyValDict dict) noexcept
    : KeyValDict(std::move(dict))
{
}

bool Event::is_success() const
{
    if (auto const response = get_value("Response")) {
        return Reaction::is_success(*response);
    }

    // If the event doesn't have a Response field then it's an event that comes after the first event  in an EventList
    // response (only the first AMI event in response to an AMI Action contains the Response field). AMI will only send
    // subsequent multipart events is if the AMI action completed successfully. In that case, success?
    return true;
}

std::string Event::to_string() const
{
    return KeyValDict::to_string();
}
