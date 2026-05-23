
// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/action/MwiDelete.hpp"

using namespace cpp_ami::action;

MWIDelete::MWIDelete(std::string mailbox)
    : Action("MWIDelete", {"Mailbox"})
{
    setValue("Mailbox", std::move(mailbox));
}