// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#ifndef UTIL_SCOPE_GUARD_HPP
#define UTIL_SCOPE_GUARD_HPP

#include <functional>

namespace cpp_ami::util {

///
/// @class ScopeGuard
///
/// @brief Allows you to set a function that will be invoked when the lifetime of this object expires.
///
class ScopeGuard {
public:
    using callback_t = std::function<void()>;

public:
    ScopeGuard() = delete;
    ScopeGuard(ScopeGuard const &) = delete;
    ScopeGuard(ScopeGuard &&right) noexcept;

    /// @brief Constructs object and takes \c func as the function that will be invoked on destruction of this object.
    ///
    /// \param func Function to be invoked when this object is destroyed.
    explicit ScopeGuard(callback_t func);

    /// @brief Invokes the callback.
    virtual ~ScopeGuard();

    ScopeGuard& operator=(ScopeGuard const &) = delete;
    ScopeGuard& operator=(ScopeGuard &&right) noexcept;

private:
    callback_t callback_;       ///< Callback that gets invoked upon object destruction.
};

}

#endif
