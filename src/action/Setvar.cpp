// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/action/Setvar.hpp"

using namespace cpp_ami::action;

Setvar::Setvar()
    : Action("SetVar", {"Channel", "Variable", "Value"})
{
}