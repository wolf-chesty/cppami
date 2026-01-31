// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#ifndef AMI_STREAM_PARSER_HPP
#define AMI_STREAM_PARSER_HPP

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace cpp_ami {

///
/// @class StreamParser
///
/// @brief Parses data read from the AMI socket server.
///
/// As messages data is read from the socket attached to the AMI server this class will build full message
/// events and dispatch them to a callback function.
///
class StreamParser {
public:
    using callback_t = std::function<void(std::string)>;
    using version_callback_t = std::function<void(std::string)>;

public:
    StreamParser() = delete;
    StreamParser(StreamParser const &) = delete;
    StreamParser(StreamParser &&) noexcept = delete;

    /// @brief Constructs an object with \c version_callback and \c callback functions to handle events from
    ///        the AMI server.
    ///
    /// @param version_callback Callback that will be invoked when the AMI server version is received.
    /// @param callback Callback that will be invoked when AMI messages are received.
    explicit StreamParser(version_callback_t version_callback, callback_t callback);

    virtual ~StreamParser();

    StreamParser& operator=(StreamParser const &) = delete;
    StreamParser& operator=(StreamParser &&) noexcept = delete;

    /// @brief Adds a buffer sequence read from the socket to the list of buffer chunks to process.
    ///
    /// @param buf String sequence read from a socket connected to AMI.
    void add_buf(std::string buf);

private:
    /// @brief Starts the worker thread.
    void start_work_thread();

    /// @brief Stops the worker thread.
    void stop_work_thread();

    /// @brief Worker thread that will process all stream chunks creating AMI events messages for dispatch.
    void work_thread();

    /// @brief Evaluates \c stream_chunk and determines if it contains an entire AMI event message or is part
    ///        of an AMI message.
    ///
    /// @param stream_chunk Message part.
    ///
    /// This functions evaluates \c stream_chunk to determine if it contains an entire AMI message event, if so
    /// a callback is invoked with the message for further processing. If \c stream_chunk contains a part of an
    /// AMI message \c stream_chunk is appended onto a working event buffer. When it is determined that the
    /// event buffer contains an entire event message the event buffer is dispatched and further message chunks
    /// are evaluated.
    void process_chunk(std::string stream_chunk);

    bool first_event_{ true };      ///< Flag indicating if a received chunk is the first message part. The first message will contain the AMI version string.
    std::string event_buf_;         ///< Working event buffer. Partial AMI events are concatenated to this string to build up a complete message.

    std::vector<std::string> stream_chunks_;    ///< Collection of stream chunks to process.
    std::mutex stream_chunks_mutex_;            ///< Mutex to control access to collection of stream chunks.

    std::thread thread_;                        ///< Handle to worker thread.
    std::atomic<bool> thread_run_{ false };     ///< Flag to stop worker thread.
    std::condition_variable thread_cv_;         ///< Condition flag to wake sleeping thread when new message chunks have arrived for processing.

    callback_t dispatch_{ [](std::string) -> void {} };                 ///< Dispatch function to invoke with AMI event messages.
    version_callback_t set_ami_version_{ [](std::string) -> void {} };  ///< Dispatch function to invoke with the AMI server version.
};

}

#endif

