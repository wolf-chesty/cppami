// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/event/Event.hpp"

using namespace cpp_ami::event;

Event::Event(KeyValDict dict) noexcept
    : KeyValDict(std::move(dict))
{
}
