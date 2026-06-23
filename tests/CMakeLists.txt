find_package(GTest)

include(GoogleTest)

add_executable(cppami_unit_tests "")

target_compile_features(cppami_unit_tests PRIVATE cxx_std_20)

enable_testing()

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.30.0")
    cmake_policy(SET CMP0167 NEW)
endif()

add_test(NAME Test COMMAND cppami_unit_tests)

target_include_directories(cppami_unit_tests
PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(cppami_unit_tests
PRIVATE
    GTest::gtest_main
    c++ami
)

target_sources(cppami_unit_tests
PRIVATE
    src/testsAmiMessages.cpp
    src/testsEventDispatcher.cpp
    src/testsScopeGuard.cpp
    src/testsSocketReader.cpp
    src/testsSocketWriter.cpp
    src/testsStreamParser.cpp
    src/testsTcpSocket.cpp
)
