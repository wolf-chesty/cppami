// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/action/MwiUpdate.hpp"

using namespace cpp_ami::action;

MWIUpdate::MWIUpdate(std::string mailbox)
    : Action("MWIUpdate", {"Mailbox", "OldMessages", "NewMessages"}, {"OldMessages", "NewMessages"})
{
    setValue("Mailbox", std::move(mailbox));
}
