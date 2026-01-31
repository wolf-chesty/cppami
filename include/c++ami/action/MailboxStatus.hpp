// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#ifndef ACTION_MAILBOXSTATUS_HPP
#define ACTION_MAILBOXSTATUS_HPP

#include "c++ami/action/Action.hpp"

namespace cpp_ami::action {

class MailboxStatus
    : public Action {
public:
    MailboxStatus(std::string mailbox);
};

}

#endif
