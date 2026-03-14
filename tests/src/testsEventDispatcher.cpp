// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include <gtest/gtest.h>

#include <string>
#include "c++ami/action/Park.hpp"
#include "c++ami/EventDispatcher.hpp"

TEST(event_dispatcher, dispatch_test)
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
        EXPECT_EQ(dict->toString(), park.toString());
    });

    dispatcher.addEvent(park.toString());
}
