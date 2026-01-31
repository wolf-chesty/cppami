// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include <boost/test/unit_test.hpp>

#include "c++ami/util/ScopeGuard.hpp"

BOOST_AUTO_TEST_SUITE(scope_guard_tests)

BOOST_AUTO_TEST_CASE(exit_scope_test)
{
    bool test_cond = true;

    {
        cpp_ami::util::ScopeGuard guard([&test_cond]() {
            test_cond = false;
        });
    }

    BOOST_CHECK(test_cond == false);
}

BOOST_AUTO_TEST_CASE(in_scope_test)
{
    bool test_cond = true;

    cpp_ami::util::ScopeGuard guard([&test_cond]() {
        test_cond = false;
    });

    BOOST_CHECK(test_cond == true);
}

BOOST_AUTO_TEST_CASE(move_test)
{
    bool test_cond = true;
    std::unique_ptr<cpp_ami::util::ScopeGuard> g;

    {
        cpp_ami::util::ScopeGuard guard([&test_cond]() {
            test_cond = false;
        });

        g = std::make_unique<cpp_ami::util::ScopeGuard>(std::move(guard));
    }
    BOOST_CHECK(test_cond == true);

    g.reset();
    BOOST_CHECK(test_cond == false);
}

BOOST_AUTO_TEST_SUITE_END()