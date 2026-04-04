// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/action/GetConfigJson.hpp"

using namespace cpp_ami::action;

GetConfigJSON::GetConfigJSON()
    : Action("GetConfigJSON", {"Filename", "Category", "Filter"})
{
}