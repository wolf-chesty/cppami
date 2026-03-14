// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include <gtest/gtest.h>

#include "c++ami/util/ScopeGuard.hpp"

TEST(scope_guard, exit_scope_test)
{
    bool test_cond = true;

    {
        cpp_ami::util::ScopeGuard guard([&test_cond]() {
            test_cond = false;
        });
    }

    EXPECT_EQ(test_cond, false);
}

TEST(scope_guard, in_scope_test)
{
    bool test_cond = true;

    cpp_ami::util::ScopeGuard guard([&test_cond]() {
        test_cond = false;
    });

    EXPECT_EQ(test_cond, true);
}

TEST(scope_guard, move_test)
{
    bool test_cond = true;
    std::unique_ptr<cpp_ami::util::ScopeGuard> g;

    {
        cpp_ami::util::ScopeGuard guard([&test_cond]() {
            test_cond = false;
        });

        g = std::make_unique<cpp_ami::util::ScopeGuard>(std::move(guard));
    }
    EXPECT_EQ(test_cond, true);

    g.reset();
    EXPECT_EQ(test_cond, false);
}
