// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/action/WaitEvent.hpp"

using namespace cpp_ami::action;

WaitEvent::WaitEvent()
    : Action("WaitEvent", {"Timeout"})
{
}
