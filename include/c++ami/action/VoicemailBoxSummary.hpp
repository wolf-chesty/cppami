// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#ifndef ACTION_VOICEMAILBOXSUMMARY_HPP
#define ACTION_VOICEMAILBOXSUMMARY_HPP

#include "c++ami/action/Action.hpp"

namespace cpp_ami::action {

class VoicemailBoxSummary
    : public Action {
public:
    explicit VoicemailBoxSummary(std::string context, std::string mailbox);
};

}

#endif
