// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/reaction/Event.hpp"

using namespace cpp_ami::reaction;

Event::Event(KeyValDict dict) noexcept
    : KeyValDict(std::move(dict))
{
}

bool Event::isSuccess() const
{
    if (auto const response = getValue("Response")) {
        return Reaction::isSuccess(*response);
    }

    // If the event doesn't have a Response field then it's an event that comes after the first event  in an EventList
    // response (only the first AMI event in response to an AMI Action contains the Response field). AMI will only send
    // subsequent multipart events is if the AMI action completed successfully. In that case, success?
    return true;
}

std::string Event::toString() const
{
    return KeyValDict::toString();
}

void Event::forEach(for_each_lambda_t lambda) const
{
    lambda(event::Event(*this));
}
