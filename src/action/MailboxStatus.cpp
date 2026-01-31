// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/action/MailboxStatus.hpp"

using namespace cpp_ami::action;

MailboxStatus::MailboxStatus(std::string mailbox)
    : Action("MailboxStatus", { "Mailbox" })
{
    set_value("Mailbox", std::move(mailbox));
}
