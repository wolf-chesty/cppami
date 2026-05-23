// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/action/VoicemailRemove.hpp"

using namespace cpp_ami::action;

VoicemailRemove::VoicemailRemove(std::string context, std::string mailbox, std::string folder, std::string id)
    : Action("VoicemailRemove", {"Context", "Mailbox", "Folder", "ID"})
{
    setValue("Context", std::move(context));
    setValue("Mailbox", std::move(mailbox));
    setValue("Folder", std::move(folder));
    setValue("ID", std::move(id));
}
