// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include <gtest/gtest.h>

#include "c++ami/util/KeyValDict.hpp"

TEST(ami_message, message_parse_test)
{
    std::string const msg("a: b\r\nc: d\r\ne: f\r\n\r\n");

    cpp_ami::util::KeyValDict ami_msg(msg);
    EXPECT_EQ(msg, ami_msg.toString());
}
