add_executable(example "")

target_compile_features(example PRIVATE cxx_std_20)

target_link_libraries(example
PUBLIC
    c++ami
)

target_sources(example
PRIVATE
    main.cpp
)
