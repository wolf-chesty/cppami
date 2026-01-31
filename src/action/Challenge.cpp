// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/action/Challenge.hpp"

using namespace cpp_ami::action;

Challenge::Challenge()
    : Action("Challenge", { "AuthType" })
{
    set_value("AuthType", "MD5");
}
