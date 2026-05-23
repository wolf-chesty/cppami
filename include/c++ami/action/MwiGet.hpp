// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#ifndef ACTION_MWI_GET_HPP
#define ACTION_MWI_GET_HPP

#include "c++ami/action/Action.hpp"

namespace cpp_ami::action {

class MWIGet : public Action {
public:
    explicit MWIGet(std::string mailbox);
};

} // namespace cpp_ami::action

#endif
