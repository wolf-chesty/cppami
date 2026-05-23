// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/action/MwiGet.hpp"

using namespace cpp_ami::action;

MWIGet::MWIGet(std::string mailbox)
    : Action("MWIGet", {"Mailbox"})
{
    setValue("Mailbox", std::move(mailbox));
}
