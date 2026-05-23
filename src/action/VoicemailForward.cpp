// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/action/VoicemailForward.hpp"

using namespace cpp_ami::action;

VoicemailForward::VoicemailForward(std::string context, std::string mailbox, std::string folder, std::string id,
                                   std::string to_context, std::string to_mailbox, std::string to_folder)
    : Action("VoicemailForward", {"Context", "Mailbox", "Folder", "ID", "ToContext", "ToMailbox", "ToFolder"})
{
    setValue("Context", std::move(context));
    setValue("Mailbox", std::move(mailbox));
    setValue("Folder", std::move(folder));
    setValue("ID", std::move(id));
    setValue("ToContext", std::move(to_context));
    setValue("ToMailbox", std::move(to_mailbox));
    setValue("ToFolder", std::move(to_folder));
}
