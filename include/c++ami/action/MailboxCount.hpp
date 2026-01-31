// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#ifndef ACTION_MAILBOXCOUNT_HPP
#define ACTION_MAILBOXCOUNT_HPP

#include "c++ami/action/Action.hpp"

namespace cpp_ami::action {

class MailboxCount
    : public Action {
public:
    MailboxCount(std::string mailbox);
};

}

#endif
