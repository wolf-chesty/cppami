// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#ifndef ACTION_VOICEMAIL_FORWARD_HPP
#define ACTION_VOICEMAIL_FORWARD_HPP

#include "c++ami/action/Action.hpp"

namespace cpp_ami::action {

class VoicemailForward : public Action {
public:
    explicit VoicemailForward(std::string context, std::string mailbox, std::string folder, std::string id,
                              std::string to_context, std::string to_mailbox, std::string to_folder);
};

} // namespace cpp_ami::action

#endif
