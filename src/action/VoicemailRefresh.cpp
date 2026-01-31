// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/action/VoicemailRefresh.hpp"

using namespace cpp_ami::action;

VoicemailRefresh::VoicemailRefresh()
    : Action("VoicemailRefresh", { "Context", "Mailbox" })
{
}
