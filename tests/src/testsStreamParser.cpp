// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include <boost/test/unit_test.hpp>

#include <string>
#include "c++ami/StreamParser.hpp"
#include <vector>

BOOST_AUTO_TEST_SUITE(cpp_ami_tests)

BOOST_AUTO_TEST_SUITE(stream_parser_tests)

BOOST_AUTO_TEST_CASE(byte_message_test)
{
    using namespace cpp_ami;

    std::string const expected_version("test version");
    auto version_handler = [&expected_version](std::string version) -> void {
        BOOST_CHECK(version == expected_version);
    };

    std::string const expected_message ="message 1\r\nmessage 2\r\nmessage 3\r\n\r\n";
    auto message_handler = [&expected_message, idx = 0](std::string message) -> void {
        BOOST_CHECK(message == expected_message);
    };

    StreamParser parser(version_handler, message_handler);

    for (auto const &part : expected_version) {
        parser.add_buf(std::string(1, part));
    }
    parser.add_buf("\r");
    parser.add_buf("\n");

    for (auto const &part : expected_message) {
        parser.add_buf(std::string(1, part));
    }
}

BOOST_AUTO_TEST_CASE(whole_message_test)
{
    using namespace cpp_ami;

    std::string const expected_version("test version");
    auto version_handler = [&expected_version](std::string version) -> void {
        BOOST_CHECK(version == expected_version);
    };

    std::string const expected_message ="message 1\r\nmessage 2\r\nmessage 3\r\n\r\n";
    auto message_handler = [&expected_message, idx = 0](std::string message) -> void {
        BOOST_CHECK(message == expected_message);
    };

    StreamParser parser(version_handler, message_handler);
    parser.add_buf(expected_version + "\r\n" + expected_message);
}

BOOST_AUTO_TEST_CASE(straddle_message_test)
{
    using namespace cpp_ami;

    std::string const expected_version("test version");
    auto version_handler = [&expected_version](std::string version) -> void {
        BOOST_CHECK(version == expected_version);
    };

    std::string const expected_message ="message 1\r\nmessage 2\r\nmessage 3\r\n\r\n";
    auto message_handler = [&expected_message, idx = 0](std::string message) -> void {
        BOOST_CHECK(message == expected_message);
    };

    StreamParser parser(version_handler, message_handler);
    parser.add_buf(expected_version + "\r");
    parser.add_buf(std::string("\n") + "message 1\r");
    parser.add_buf(std::string("\n") + "message 2\r");
    parser.add_buf(std::string("\n") + "message 3\r");
    parser.add_buf("\n");
    parser.add_buf("\r\n");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
