// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#ifndef ACTION_LOGIN_HPP
#define ACTION_LOGIN_HPP

#include "c++ami/action/Action.hpp"

namespace cpp_ami::action {

class Login
    : public Action {
public:
    Login();
    explicit Login(std::string username, std::string secret);
};

}

#endif
