// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#ifndef ACTION_MWI_DELETE_HPP
#define ACTION_MWI_DELETE_HPP

#include "c++ami/action/Action.hpp"

namespace cpp_ami::action {

class MWIDelete : public Action {
public:
    explicit MWIDelete(std::string mailbox);
};

} // namespace cpp_ami::action

#endif
