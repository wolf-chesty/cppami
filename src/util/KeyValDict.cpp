// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/util/KeyValDict.hpp"

#include "c++ami/CppAmiDefs.h"
#include <algorithm>
#include <cassert>
#include <fmt/core.h>
#include <stdexcept>
#include <utility>

using namespace cpp_ami::util;

KeyValDict::KeyValDict(std::string event_buf)
{
    setMessage(std::move(event_buf));
}

KeyValDict::KeyValDict(std::vector<std::string> ordered_keys, std::unordered_set<std::string> optional_keys)
    : ordered_keys_(std::move(ordered_keys))
    , optional_keys_(std::move(optional_keys))
{
#ifndef NDEBUG
    for (auto const &optional_key : optional_keys_) {
        assert(std::find(ordered_keys_.begin(), ordered_keys_.end(), optional_key) != ordered_keys_.end());
    }
#endif
}

size_t KeyValDict::count() const
{
    return ordered_keys_.size();
}

bool KeyValDict::hasKey(std::string const &key) const
{
    return std::find(ordered_keys_.begin(), ordered_keys_.end(), key) != ordered_keys_.end();
}

std::string& KeyValDict::operator[](std::string const &key)
{
    if (!hasKey(key)) {
        throw std::runtime_error(fmt::format("unknown key {}", key));
    }
    return values_[key];
}

std::string const& KeyValDict::operator[](std::string const &key) const
{
    if (!hasKey(key)) {
        throw std::runtime_error(fmt::format("unknown key {}", key));
    }

    if (auto const it = values_.find(key); it != values_.end()) {
        return it->second;
    }

    static std::string const empty_string;
    return empty_string;
}

std::optional<std::string> KeyValDict::getValue(std::string const &key) const
{
    if (auto const it = values_.find(key); it != values_.end()) {
        return std::optional(it->second);
    }
    return std::nullopt;
}

void KeyValDict::setValue(std::string const &key, std::string val)
{
    if (!hasKey(key)) {
        throw std::runtime_error(fmt::format("unknown key {}", key));
    }
    values_[key] = std::move(val);
}

void KeyValDict::setMessage(std::string event_buf)
{
    assert(!event_buf.empty());

    ordered_keys_.clear();
    values_.clear();

    for (size_t key_beg = 0; key_beg < event_buf.length(); ) {
        auto const key_end = event_buf.find(SEP, key_beg);
        if (key_end == std::string::npos) {
            break;
        }

        std::string_view key(event_buf.data() + key_beg, key_end - key_beg);
        // Maintain key order
        ordered_keys_.emplace_back(key);

        // Calculate value position
        auto const val_beg = key_end + 2;
        auto const val_end = event_buf.find(EOR, val_beg);
        std::string_view val(event_buf.data() + val_beg, val_end - val_beg);
        // Capture key value
        values_.emplace(key, val);

        key_beg = val_end + 2;
    }
}

std::string KeyValDict::toString() const
{
    std::string action_string;
    for (auto const &key : ordered_keys_) {
        auto const it_val = values_.find(key);
        if (it_val != values_.end()) {
            action_string += fmt::format("{}{}{}{}", key, SEP, it_val->second, EOR);
        }
        else if (!isOptional(key)) {
            action_string += fmt::format("{}{}{}{}", key, SEP, "", EOR);
        }
    }
    return action_string + EOR;
}

bool KeyValDict::isOptional(std::string const &key) const
{
    return optional_keys_.find(key) != optional_keys_.end();
}
