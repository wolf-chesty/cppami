// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/action/UserEvent.hpp"

using namespace cpp_ami::action;

UserEvent::UserEvent()
    : Action("UserEvent", {"UserEvent", "Header1", "HeaderN"})
{
}
