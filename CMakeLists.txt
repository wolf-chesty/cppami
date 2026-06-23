cmake_minimum_required(VERSION 3.21)
cmake_policy(VERSION 3.21)

project(c++ami VERSION 0.0.0.0 LANGUAGES CXX C)

# ----------------------------------------------------------------------------------------------------------------------
# Options
# ----------------------------------------------------------------------------------------------------------------------
option(CPPAMI_UNIT_TESTS "Build unit tests" OFF)
option(CPPAMI_EXAMPLE "Build example" OFF)

# ----------------------------------------------------------------------------------------------------------------------
# Project dependencies
# ----------------------------------------------------------------------------------------------------------------------
find_package(fmt 12.1.0 REQUIRED)

# ----------------------------------------------------------------------------------------------------------------------
# Project definition
# ----------------------------------------------------------------------------------------------------------------------
add_library(c++ami OBJECT "")

target_compile_features(c++ami PRIVATE cxx_std_20)

target_include_directories(c++ami
PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(c++ami
PUBLIC
    fmt
    pthread
    uuid
)

target_sources(c++ami
PRIVATE
    src/action/Action.cpp
    src/action/Challenge.cpp
    src/action/DeviceStateList.cpp
    src/action/Events.cpp
    src/action/ExtensionState.cpp
    src/action/ExtensionStateList.cpp
    src/action/GetConfig.cpp
    src/action/GetConfigJson.cpp
    src/action/Getvar.cpp
    src/action/ListCommands.cpp
    src/action/Login.cpp
    src/action/Logoff.cpp
    src/action/MailboxCount.cpp
    src/action/MailboxStatus.cpp
    src/action/MessageSend.cpp
    src/action/Park.cpp
    src/action/ParkedCalls.cpp
    src/action/Parkinglots.cpp
    src/action/Ping.cpp
    src/action/PjsipNotify.cpp
    src/action/PjsipShowAuths.cpp
    src/action/PjsipShowContacts.cpp
    src/action/PjsipShowEndpoint.cpp
    src/action/PjsipShowEndpoints.cpp
    src/action/PjsipShowRegistrationInboundContactStatuses.cpp
    src/action/PjsipShowRegistrationsInbound.cpp
    src/action/PjsipShowRegistrationsOutbound.cpp
    src/action/PjsipShowResourceLists.cpp
    src/action/PjsipShowSubscriptionsInbound.cpp
    src/action/PjsipShowSubscriptionsOutbound.cpp
    src/action/Setvar.cpp
    src/action/UserEvent.cpp
    src/action/VoicemailBoxSummary.cpp
    src/action/VoicemailForward.cpp
    src/action/VoicemailMove.cpp
    src/action/VoicemailRefresh.cpp
    src/action/VoicemailRemove.cpp
    src/action/VoicemailUsersList.cpp
    src/action/VoicemailUserStatus.cpp
    src/action/WaitEvent.cpp

    src/event/Event.cpp

    src/net/SocketReader.cpp
    src/net/SocketWriter.cpp
    src/net/TcpSocket.cpp

    src/reaction/Event.cpp
    src/reaction/EventList.cpp
    src/reaction/Reaction.cpp

    src/util/KeyValDict.cpp
    src/util/ScopeGuard.cpp

    src/Connection.cpp
    src/EventDispatcher.cpp
    src/StreamParser.cpp
)

add_subdirectory(tests)

add_subdirectory(example)
