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

TEST(ami_message, multipart_parse_test)
{
    cpp_ami::util::KeyValDict action({"Endpoint", "URI", "channel", "Option", "Variable"},
                                     {"Endpoint", "URI", "channel", "Option", "Variable"});
    action["URI"] = "uri";
    action.setValues(
        "Variable",
        {"Event=Yealink-xml", "Content-Type=application/xml",
         "Content=<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?><YealinkIPPhoneExecute Beep=\"yes\" refresh=\"1\"><ExecuteItem URI=\"Led:LINE10_RED=off\"/></YealinkIPPhoneExecute>"});

    EXPECT_EQ(
        action.toString(),
        "URI: uri\r\n"
        "Variable: Event=Yealink-xml\r\n"
        "Variable: Content-Type=application/xml\r\n"
        "Variable: Content=<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?><YealinkIPPhoneExecute Beep=\"yes\" refresh=\"1\"><ExecuteItem URI=\"Led:LINE10_RED=off\"/></YealinkIPPhoneExecute>\r\n\r\n");
}

TEST(ami_message, value_array)
{
    std::vector<std::string> in{"sookie", "sookie", "now"};
    auto const value = cpp_ami::util::KeyValDict::toString("UnitTest", in);
    EXPECT_EQ(value, "sookie\r\nUnitTest: sookie\r\nUnitTest: now");

    auto out = cpp_ami::util::KeyValDict::fromString("UnitTest", value);
    EXPECT_EQ(in.size(), out.size());
    for (size_t i = 0; i < in.size(); ++i) {
        EXPECT_EQ(in[i], out[i]);
    }
}

TEST(ami_message, value_array_single)
{
    std::vector<std::string> in{"single"};
    auto const value = cpp_ami::util::KeyValDict::toString("UnitTest", in);
    EXPECT_EQ(value, "single");

    auto out = cpp_ami::util::KeyValDict::fromString("UnitTest", value);
    EXPECT_EQ(in.size(), out.size());
    for (size_t i = 0; i < in.size(); ++i) {
        EXPECT_EQ(in[i], out[i]);
    }
}

TEST(ami_test, value_array_empty)
{
    std::vector<std::string> in{"single", ""};
    auto const value = cpp_ami::util::KeyValDict::toString("UnitTest", in);
    EXPECT_EQ(value, "single\r\nUnitTest: ");

    auto out = cpp_ami::util::KeyValDict::fromString("UnitTest", value);
    EXPECT_EQ(in.size(), out.size());
    for (size_t i = 0; i < in.size(); ++i) {
        EXPECT_EQ(in[i], out[i]);
    }
}