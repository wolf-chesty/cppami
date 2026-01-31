// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/action/Events.hpp"

using namespace cpp_ami::action;

Events::Events()
    : Action("Events", { "EventMask" })
{
    set_value("EventMask", "on");
}
