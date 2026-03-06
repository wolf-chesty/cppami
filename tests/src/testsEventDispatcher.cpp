// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include <boost/test/unit_test.hpp>

#include <string>
#include "c++ami/action/Park.hpp"
#include "c++ami/EventDispatcher.hpp"

BOOST_AUTO_TEST_SUITE(cpp_ami_tests)

BOOST_AUTO_TEST_SUITE(event_dispatcher_tests)

BOOST_AUTO_TEST_CASE(dispatch_test)
{
    using namespace cpp_ami;

    action::Park park;
    park["Channel"] = "ama";
    park["TimeoutChannel"] = "say";
    park["AnnounceChannel"] = "mama";
    park["Timeout"] = "sa";
    park["Parkinglot"] = "mama";
    park["ParkingSpace"] = "kusa";

    EventDispatcher dispatcher([&park](EventDispatcher::event_ptr_t dict) -> void {
        BOOST_CHECK(dict->to_string() == park.to_string());
    });

    dispatcher.add_event(park.to_string());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
