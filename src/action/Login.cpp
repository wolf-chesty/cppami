// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/action/Login.hpp"

#include <stdexcept>

using namespace cpp_ami::action;

Login::Login()
    : Action("Login", { "Username", "AuthType", "Secret", "Key", "Events" })
{
}

Login::Login(std::string username, std::string secret)
    : Login()
{
    set_value("Username", std::move(username));
    set_value("Secret", std::move(secret));
}
