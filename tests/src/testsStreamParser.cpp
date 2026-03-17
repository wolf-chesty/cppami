// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include <gtest/gtest.h>

#include "c++ami/StreamParser.hpp"
#include <string>

TEST(stream_parser, byte_test)
{
    using namespace cpp_ami;

    std::string const expected_version("test version");
    auto version_handler = [&expected_version](std::string version) -> void { EXPECT_EQ(version, expected_version); };

    std::string const expected_message = "message 1\r\nmessage 2\r\nmessage 3\r\n\r\n";
    auto message_handler = [&expected_message, idx = 0](std::string message) -> void {
        EXPECT_EQ(message, expected_message);
    };

    StreamParser parser(version_handler, message_handler);

    for (auto const &part : expected_version) {
        parser.addBuf(std::string(1, part));
    }
    parser.addBuf("\r");
    parser.addBuf("\n");

    for (auto const &part : expected_message) {
        parser.addBuf(std::string(1, part));
    }
}

TEST(stream_parser, whole_test)
{
    using namespace cpp_ami;

    std::string const expected_version("test version");
    auto version_handler = [&expected_version](std::string version) -> void { EXPECT_EQ(version, expected_version); };

    std::string const expected_message = "message 1\r\nmessage 2\r\nmessage 3\r\n\r\n";
    auto message_handler = [&expected_message, idx = 0](std::string message) -> void {
        EXPECT_EQ(message, expected_message);
    };

    StreamParser parser(version_handler, message_handler);
    parser.addBuf(expected_version + "\r\n" + expected_message);
}

TEST(stream_parser, straddle_test)
{
    using namespace cpp_ami;

    std::string const expected_version("test version");
    auto version_handler = [&expected_version](std::string version) -> void { EXPECT_EQ(version, expected_version); };

    std::string const expected_message = "message 1\r\nmessage 2\r\nmessage 3\r\n\r\n";
    auto message_handler = [&expected_message, idx = 0](std::string message) -> void {
        EXPECT_EQ(message, expected_message);
    };

    StreamParser parser(version_handler, message_handler);
    parser.addBuf(expected_version + "\r");
    parser.addBuf(std::string("\n") + "message 1\r");
    parser.addBuf(std::string("\n") + "message 2\r");
    parser.addBuf(std::string("\n") + "message 3\r");
    parser.addBuf("\n");
    parser.addBuf("\r\n");
}
