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