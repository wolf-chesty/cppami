// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/action/Challenge.hpp"
#include "c++ami/action/DeviceStateList.hpp"
#include "c++ami/action/Events.hpp"
#include "c++ami/action/ExtensionState.hpp"
#include "c++ami/action/Getvar.hpp"
#include "c++ami/action/ListCommands.hpp"
#include "c++ami/action/Login.hpp"
#include "c++ami/action/Logoff.hpp"
#include "c++ami/action/MailboxCount.hpp"
#include "c++ami/action/MailboxStatus.hpp"
#include "c++ami/action/ParkedCalls.hpp"
#include "c++ami/action/Parkinglots.hpp"
#include "c++ami/action/Ping.hpp"
#include "c++ami/action/VoicemailBoxSummary.hpp"
#include "c++ami/action/VoicemailRefresh.hpp"
#include "c++ami/Connection.hpp"
#include "c++ami/reaction/EventList.hpp"
#include <cassert>
#include <iostream>

#include <quill/Backend.h>
#include <quill/Frontend.h>
#include <quill/Logger.h>
#include <quill/LogMacros.h>
#include <quill/sinks/ConsoleSink.h>

int main()
{
    quill::Backend::start();

    auto logger = quill::Frontend::create_or_get_logger("root",
                                                    quill::Frontend::create_or_get_sink<quill::ConsoleSink>("sink_id_1"));
    LOG_INFO(logger, "Starting example");

    cpp_ami::Connection conn(logger, "10.3.29.93");

    conn.add_callback([logger](cpp_ami::util::KeyValDict const *dict) -> void {
        std::cout << dict->to_string();
    });

    cpp_ami::action::Login login("admin", "test");
    login["AuthType"] = "plain";
    login["Events"] = "off";

    if (auto reaction = conn.invoke(login); reaction->is_success()) {
        std::cout << login.to_string() << reaction->to_string();

        cpp_ami::action::Events events;
        reaction = conn.invoke(events);
        events["EventMask"] = "off";
        std::cout << events.to_string() << reaction->to_string();

        std::atomic<bool> run = true;
        std::thread ping_thread([&run, &conn]() -> void {
            while (run) {
                cpp_ami::action::Ping const ping;
                auto response = conn.invoke(ping);
                std::cout << ping.to_string() << response->to_string();

                std::this_thread::sleep_for(std::chrono::seconds(5));
            }
        });

        cpp_ami::action::ListCommands const list_commands;
        reaction = conn.invoke(list_commands);
        std::cout << list_commands.to_string() << reaction->to_string();
        conn.async_invoke(list_commands);

        cpp_ami::action::Parkinglots const parking_lots;
        reaction = conn.invoke(parking_lots);
        std::cout << parking_lots.to_string() << reaction->to_string();

        cpp_ami::action::ExtensionState ext_state;
        ext_state["Exten"] = "701@parkedcalls";
        std::cout << ext_state.to_string() << reaction->to_string();

        cpp_ami::action::ParkedCalls const parked_calls;
        reaction = conn.invoke(parked_calls);
        std::cout << parked_calls.to_string() << reaction->to_string();

        cpp_ami::action::MailboxStatus const mailbox_status("5558675309");
        reaction = conn.invoke(mailbox_status);
        std::cout << mailbox_status.to_string() << reaction->to_string();

        cpp_ami::action::MailboxCount const mailbox_count("5558675309");
        reaction = conn.invoke(mailbox_count);
        std::cout << mailbox_count.to_string() << reaction->to_string();

        cpp_ami::action::VoicemailBoxSummary const vm_summary("default", "5558675309");
        reaction = conn.invoke(vm_summary);
        assert(dynamic_cast<cpp_ami::reaction::EventList const *>(reaction.get()));
        std::cout << vm_summary.to_string() << reaction->to_string();

        cpp_ami::action::VoicemailRefresh const vm_refresh;
        reaction = conn.invoke(vm_refresh);
        std::cout << vm_refresh.to_string() << reaction->to_string();

        cpp_ami::action::DeviceStateList const device_state_list;
        reaction = conn.invoke(device_state_list);
        std::cout << device_state_list.to_string() << reaction->to_string();

        std::this_thread::sleep_for(std::chrono::seconds(30));

        run = false;
        ping_thread.join();

        cpp_ami::action::Logoff logoff;
        reaction = conn.invoke(logoff);
        std::cout << logoff.to_string() << reaction->to_string();
    }
    else {
        std::cout << "Unable to login\n";
    }

    return 0;
}