// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/action/Action.hpp"

#include "c++ami/CppAmiDefs.h"
#include <cassert>
#include <uuid/uuid.h>

using namespace cpp_ami::action;

Action::Action(std::string action, std::vector<std::string> ordered_keys, std::unordered_set<std::string> optional_keys)
    : KeyValDict(std::move(ordered_keys), std::move(optional_keys))
    , action_(std::move(action))
    , action_id_(createUuid())
{
    assert(!action_.empty());
}

std::string Action::createUuid()
{
    uuid_t uuid;
    uuid_generate(uuid);

    std::string uuid_buf(37, '\0');
    uuid_unparse_lower(uuid, uuid_buf.data());
    uuid_buf.resize(uuid_buf.size() - 1);

    return uuid_buf;
}

std::string Action::getAction() const
{
    return action_;
}

std::string Action::getActionId() const
{
    return action_id_;
}

std::string Action::toString() const
{
    static std::string action_key{"Action"};
    static std::string action_id_key{"ActionID"};
    return action_key + SEP + action_ + EOR + action_id_key + SEP + action_id_ + EOR + KeyValDict::toString();
}
