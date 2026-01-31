// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/action/ExtensionState.hpp"

using namespace cpp_ami::action;

ExtensionState::ExtensionState()
    : Action("ExtensionState", { "Exten", "Context" })
{
}
