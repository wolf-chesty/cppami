find_package(GTest)

include(GoogleTest)

add_executable(cppami_unit_tests "")

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

if(CPPAMI_COVERAGE)
    target_compile_options(cppami_unit_tests PRIVATE -O0 --coverage -g)
    target_link_options(cppami_unit_tests PRIVATE --coverage)
    target_link_libraries(cppami_unit_tests PRIVATE gcov)
elseif(CPPAMI_PERF)
    target_compile_options(cppami_unit_tests PRIVATE -fno-omit-frame-pointer -g)
    target_link_options(cppami_unit_tests PRIVATE -fno-omit-frame-pointer -g)
elseif(CPPAMI_ASAN)
    target_compile_options(cppami_unit_tests PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer -g)
    target_link_options(cppami_unit_tests PRIVATE -fsanitize=address,undefined)
elseif(CPPAMI_TSAN)
    target_compile_options(cppami_unit_tests PRIVATE -fsanitize=thread -fno-omit-frame-pointer -g)
    target_link_options(cppami_unit_tests PRIVATE -fsanitize=thread)
endif ()
