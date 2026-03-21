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
                                     {"Endpoint", "URI", "channel", "Option", "Variable"}, {"Variable"});
    action["URI"] = "uri";
    action["Variable"] =
        "Event=Yealink-xml\r\n"
        "Content-Type=application/xml\r\n"
        "Content=<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?><YealinkIPPhoneExecute Beep=\"yes\" refresh=\"1\"><ExecuteItem URI=\"Led:LINE10_RED=off\"/></YealinkIPPhoneExecute>";

    EXPECT_EQ(
        action.toString(),
        "URI: uri\r\n"
        "Variable: Event=Yealink-xml\r\n"
        "Variable: Content-Type=application/xml\r\n"
        "Variable: Content=<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?><YealinkIPPhoneExecute Beep=\"yes\" refresh=\"1\"><ExecuteItem URI=\"Led:LINE10_RED=off\"/></YealinkIPPhoneExecute>\r\n\r\n");
}
