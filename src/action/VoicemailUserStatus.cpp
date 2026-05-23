// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/action/VoicemailUserStatus.hpp"

using namespace cpp_ami::action;

VoicemailUserStatus::VoicemailUserStatus(std::string context, std::string mailbox)
    : Action("VoicemailUserStatus", {"Context", "Mailbox"})
{
    setValue("Context", std::move(context));
    setValue("Mailbox", std::move(mailbox));
}
