// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#ifndef AMI_DEFS_HPP
#define AMI_DEFS_HPP

#include <string>

namespace cpp_ami {

static std::string const EOM{"\r\n\r\n"};
static std::string const EOR{"\r\n"};
static std::string const SEP{": "};

constexpr size_t MAX_BUF_SIZE{65535};

}

#endif