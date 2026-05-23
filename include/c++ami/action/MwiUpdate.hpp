// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#ifndef ACTION_MWI_UPDATE_HPP
#define ACTION_MWI_UPDATE_HPP

#include "c++ami/action/Action.hpp"

namespace cpp_ami::action {

class MWIUpdate : public Action {
public:
    explicit MWIUpdate(std::string mailbox);
};

} // namespace cpp_ami::action

#endif
