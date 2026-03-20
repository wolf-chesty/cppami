// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#ifndef ACTION_ACTION_HPP
#define ACTION_ACTION_HPP

#include "c++ami/util/KeyValDict.hpp"

#include <string>

namespace cpp_ami::action {

class Action : public util::KeyValDict {
public:
    Action() = delete;
    Action(Action const &) = default;
    Action(Action &&) noexcept = default;
    explicit Action(std::string action, std::vector<std::string> ordered_keys = {},
                    std::unordered_set<std::string> optional_keys = {}, std::unordered_set<std::string> list_keys = {});
    ~Action() override = default;

    Action &operator=(Action const &) = default;
    Action &operator=(Action &&) noexcept = default;

    static std::string createUuid();

    std::string getAction() const;
    std::string getActionId() const;

    std::string toString() const override;

private:
    std::string action_;
    std::string action_id_;
};

} // namespace cpp_ami::action

#endif
