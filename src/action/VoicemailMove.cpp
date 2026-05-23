// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/action/VoicemailMove.hpp"

using namespace cpp_ami::action;

VoicemailMove::VoicemailMove(std::string context, std::string mailbox, std::string folder, std::string id,
                             std::string to_folder)
    : Action("VoicemailMove", {"Context", "Mailbox", "Folder", "ID", "ToFolder"})
{
    setValue("Context", std::move(context));
    setValue("Mailbox", std::move(mailbox));
    setValue("Folder", std::move(folder));
    setValue("ID", std::move(id));
    setValue("ToFolder", std::move(to_folder));
}
