add_executable (unit_tests "")

enable_testing ()

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.30.0" )
    cmake_policy (SET CMP0167 NEW)
endif ()

find_package (Boost
    REQUIRED COMPONENTS
        filesystem
        system
        unit_test_framework
)

add_test (NAME Test COMMAND unit_tests)

target_include_directories (unit_tests
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (unit_tests
    PRIVATE
        Boost::system
        Boost::filesystem
        Boost::unit_test_framework

        c++ami
)

target_sources (unit_tests
    PRIVATE
        src/main.cpp
        src/ami_message_tests.cpp
        src/scope_guard_tests.cpp
)
