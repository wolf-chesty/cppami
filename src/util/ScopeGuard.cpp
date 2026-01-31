// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/util/ScopeGuard.hpp"

#include <utility>

using namespace cpp_ami::util;

ScopeGuard::ScopeGuard(ScopeGuard &&right) noexcept
{
    operator=(std::move(right));
}

ScopeGuard::ScopeGuard(callback_t func)
    : callback_(std::move(func))
{
}

ScopeGuard::~ScopeGuard()
{
    callback_();
}

ScopeGuard& ScopeGuard::operator=(ScopeGuard &&right) noexcept
{
    callback_ = std::exchange(right.callback_, []() -> void {});
    return *this;
}
