// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/action/PjsipNotify.hpp"

using namespace cpp_ami::action;

PJSIPNotify::PJSIPNotify()
    : Action("PJSIPNotify", {"Endpoint", "URI", "channel", "Option", "Variable"},
             {"Endpoint", "URI", "channel", "Option", "Variable"})
{
}
