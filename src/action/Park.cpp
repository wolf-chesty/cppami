// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/action/Park.hpp"

using namespace cpp_ami::action;

Park::Park()
    : Action("Park", {"Channel", "TimeoutChannel", "AnnounceChannel", "Timeout", "Parkinglot", "ParkingSpace"},
             {"TimeoutChannel", "AnnounceChannel", "Timeout", "Parkinglot", "ParkingSpace"})
{
}
