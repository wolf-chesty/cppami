// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include <boost/test/unit_test.hpp>

#include "c++ami/util/KeyValDict.hpp"

BOOST_AUTO_TEST_SUITE(ami_message_tests)

BOOST_AUTO_TEST_CASE(message_parse_test)
{
    std::string const msg("a: b\r\nc: d\r\ne: f\r\n\r\n");

    cpp_ami::util::KeyValDict ami_msg(msg);
    BOOST_CHECK(msg == ami_msg.to_string());
}

BOOST_AUTO_TEST_SUITE_END()