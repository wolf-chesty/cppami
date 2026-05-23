// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#ifndef ACTION_VOICEMAIL_USER_STATUS_HPP
#define ACTION_VOICEMAIL_USER_STATUS_HPP

#include "c++ami/action/Action.hpp"

namespace cpp_ami::action {

class VoicemailUserStatus : public Action {
public:
    explicit VoicemailUserStatus(std::string context, std::string mailbox);
};

} // namespace cpp_ami::action

#endif
