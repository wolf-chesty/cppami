// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/action/GetConfig.hpp"

using namespace cpp_ami::action;

GetConfig::GetConfig()
    : Action("GetConfig", {"Filename", "Category", "Filter"})
{
}
