add_executable (example "")

set (CMAKE_CXX_STANDARD 20)

target_link_libraries (example
    PUBLIC
        c++ami
)

target_sources (example
    PRIVATE
        main.cpp
)

if (CPPAMI_COVERAGE)
    target_link_options (example PRIVATE --coverage)
    target_link_libraries (example PRIVATE gcov)
elseif (CPPAMI_SANITIZE_ADDRESS)
    target_compile_options (example PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer -g)
    target_link_options (example PRIVATE -fsanitize=address,undefined)
elseif (CPPAMI_SANITIZE_THREAD)
    target_compile_options (example PRIVATE -fsanitize=thread -fno-omit-frame-pointer -g)
    target_link_options (example PRIVATE -fsanitize=thread)
endif ()
