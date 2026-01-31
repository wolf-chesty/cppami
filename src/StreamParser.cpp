// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#include "c++ami/StreamParser.hpp"

#include "c++ami/CppAmiDefs.h"
#include <cassert>

using namespace cpp_ami;

StreamParser::StreamParser(version_callback_t version_callback, callback_t callback)
    : dispatch_(std::move(callback))
    , set_ami_version_(std::move(version_callback))
{
    stream_chunks_.reserve(100);

    start_work_thread();
}

StreamParser::~StreamParser()
{
    stop_work_thread();
}

void StreamParser::add_buf(std::string buf)
{
    std::unique_lock const lock(stream_chunks_mutex_);
    stream_chunks_.push_back(std::move(buf));
    thread_cv_.notify_one();
}

void StreamParser::start_work_thread()
{
    thread_run_ = true;
    thread_ = std::thread(&StreamParser::work_thread, this);

    std::string_view thread_name("ami_parser");
    assert(thread_name.length() <= 16);
    pthread_setname_np(thread_.native_handle(), thread_name.data());
}

void StreamParser::stop_work_thread()
{
    thread_run_ = false;
    thread_cv_.notify_one();

    assert(thread_.joinable());
    thread_.join();
}

void StreamParser::work_thread()
{
    decltype(stream_chunks_) stream_chunks;
    stream_chunks.reserve(stream_chunks_.capacity());

    event_buf_.clear();

    while (thread_run_) {
        std::unique_lock lock(stream_chunks_mutex_);
        thread_cv_.wait(lock, [this]() -> bool { return !thread_run_ || !stream_chunks_.empty(); });
        std::swap(stream_chunks_, stream_chunks);
        lock.unlock();

        for (auto &stream_chunk : stream_chunks) {
            process_chunk(std::move(stream_chunk));
        }
        stream_chunks.clear();
    }

    std::unique_lock const lock(stream_chunks_mutex_);
    for (auto &stream_chunk : stream_chunks_) {
        process_chunk(std::move(stream_chunk));
    }
    stream_chunks_.clear();
}

void StreamParser::process_chunk(std::string stream_chunk)
{
    // The very first event from AMI contains the AMI version; grab it
    if (first_event_) {
        first_event_ = false;

        auto eor_loc = stream_chunk.find(EOR);
        set_ami_version_(stream_chunk.substr(0, eor_loc));

        eor_loc += 2;
        if (stream_chunk.length() == eor_loc) {
            return;
        }
        stream_chunk.erase(0, eor_loc);
    }

    // Start parsing events
    auto eom_loc{std::string::npos};
    if (event_buf_.empty()) {
        // Happy path; entire event_buf has been cleared of messages. Just take the contents of stream_chunk and try
        // processing that for an event.
        event_buf_ = std::move(stream_chunk);
        eom_loc = event_buf_.find(EOM, 0);
    }
    else {
        // Not so happy path; event_buf has a partial event in it. Append stream_chunk to event_buf and try parsing.

        // Rather than always scanning from the beginning of event_buf start scanning from the chunk boundary minus an
        // EOM back off amount, this will capture end-of-message sequences that straddle the chunk boundary. If the EOM
        // sequence length is greater than the current event_buf length then just start scanning from the beginning of
        // event_buf.
        auto const scan_pos_start = event_buf_.length() < EOM.length()
            ? 0                                         // Beginning of event_buf
            : event_buf_.length() - EOM.length() + 1;  // EOM length - 1 byte before the chunk boundary

        event_buf_ += stream_chunk;
        eom_loc = event_buf_.find(EOM, scan_pos_start);
    }

    while (eom_loc != std::string::npos) {
        // Found EOM sequence; increment eom_loc to include EOM sequence
        eom_loc += EOM.length();

        if (eom_loc == event_buf_.length()) {
            // The entirety of event_buf is an event; dispatch event_buf
            dispatch_(std::move(event_buf_));
            break;
        }

        // event_buf contains multiple events; parse the first one and dispatch it
        dispatch_(event_buf_.substr(0, eom_loc));
        // Remove dispatched event from event_buf
        event_buf_.erase(0, eom_loc);

        // Search for next event in event_buf
        eom_loc = event_buf_.find(EOM, 0);
    }
}
