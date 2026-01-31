// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/action/Getvar.hpp"

using namespace cpp_ami::action;

Getvar::Getvar()
    : Action("Getvar", { "Channel", "Variable" })
{
}
