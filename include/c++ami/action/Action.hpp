// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#ifndef ACTION_ACTION_HPP
#define ACTION_ACTION_HPP

#include "c++ami/util/KeyValDict.hpp"

#include <string>

namespace cpp_ami::action {

class Action
    : public util::KeyValDict {
public:
    Action() = delete;
    Action(Action const &) = default;
    Action(Action &&) noexcept = default;
    explicit Action(std::string action, std::vector<std::string> ordered_keys = {});
    ~Action() override = default;

    Action& operator=(Action const &) = default;
    Action& operator=(Action &&) noexcept = default;

    static std::string create_uuid();

    std::string get_action() const;
    std::string get_action_id() const;

    std::string to_string() const override;

private:
    std::string action_;
    std::string action_id_;
};

}

#endif
