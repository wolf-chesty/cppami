// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/action/VoicemailBoxSummary.hpp"

using namespace cpp_ami::action;

VoicemailBoxSummary::VoicemailBoxSummary(std::string context, std::string mailbox)
    : Action("VoicemailBoxSummary", {"Context", "Mailbox"})
{
    setValue("Context", std::move(context));
    setValue("Mailbox", std::move(mailbox));
}
