// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/action/MailboxCount.hpp"

using namespace cpp_ami::action;

MailboxCount::MailboxCount(std::string mailbox)
    : Action("MailboxCount", { "Mailbox" })
{
    set_value("Mailbox", std::move(mailbox));
}
