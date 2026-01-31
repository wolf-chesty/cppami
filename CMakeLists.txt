cmake_minimum_required (VERSION 3.12)
cmake_policy (VERSION 3.12)

set (CMAKE_CXX_STANDARD 20)

project (c++ami VERSION 0.0.0.0 LANGUAGES CXX C)

# ----------------------------------------------------------------------------------------------------------------------
# Options
# ----------------------------------------------------------------------------------------------------------------------
option (SANITIZE_ADDRESS "Enable AddressSanitizer (ASan) for memory error detection tests." OFF)

option (SANITIZE_THREAD "Enable ThreadSanitizer (TSan) for detecting thread-related issues." OFF)

option (UNIT_TESTS "Build unit tests" ON)

option (EXAMPLE "Build example" ON)

# ----------------------------------------------------------------------------------------------------------------------
# Project dependencies
# ----------------------------------------------------------------------------------------------------------------------
include (third_party/cpp-httplib.cmake)
include (third_party/fmt.cmake)
include (third_party/pugixml.cmake)
include (third_party/quill.cmake)

# ----------------------------------------------------------------------------------------------------------------------
# Project definition
# ----------------------------------------------------------------------------------------------------------------------
add_library (c++ami OBJECT "")

target_include_directories (c++ami
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (c++ami
    PUBLIC
        fmt
        pthread
        quill
        uuid
)

target_sources (c++ami
    PRIVATE
        src/action/Action.cpp
        src/action/Challenge.cpp
        src/action/DeviceStateList.cpp
        src/action/Events.cpp
        src/action/ExtensionState.cpp
        src/action/Getvar.cpp
        src/action/ListCommands.cpp
        src/action/Login.cpp
        src/action/Logoff.cpp
        src/action/MailboxCount.cpp
        src/action/MailboxStatus.cpp
        src/action/ParkedCalls.cpp
        src/action/Parkinglots.cpp
        src/action/Ping.cpp
        src/action/VoicemailBoxSummary.cpp
        src/action/VoicemailRefresh.cpp

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

if (CPPAMI_SANITIZE_ADDRESS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined -fno-omit-frame-pointer -g")
endif ()

if (CPPAMI_SANITIZE_THREAD)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -fno-omit-frame-pointer -g")
endif ()

if (CPPAMI_UNIT_TESTS)
    add_subdirectory (tests)
endif ()

if (CPPAMI_EXAMPLE)
    add_subdirectory (example)
endif ()
