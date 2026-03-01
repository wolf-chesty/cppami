// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/action/MessageSend.hpp"

using namespace cpp_ami::action;

MessageSend::MessageSend()
    : Action("MessageSend", {"Destination", "To", "From", "Body", "Base64Body", "Variable"}, {"Destination", "To"})
{
}