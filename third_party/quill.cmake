include (FetchContent)

FetchContent_Declare (
    quill
        GIT_REPOSITORY https://github.com/odygrd/quill.git
        GIT_TAG tags/v11.0.2
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable (quill)
