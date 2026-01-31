// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/reaction/Reaction.hpp"

using namespace cpp_ami::reaction;

bool Reaction::is_success(std::string const &status)
{
    return status == "Success" || status == "Goodbye";
}
