// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#ifndef REACTION_REACTION_HPP
#define REACTION_REACTION_HPP

#include <string>

namespace cpp_ami::reaction {

/// @class Reaction
///
/// @brief Provides a interface for returning Events that are were sent by AMI in response to an \c Action.
///
/// Whenever an \c Action is sent to AMI, AMI will respond with \c Events that fulfill the request of the action. Class
/// provides a baseclass for those events so that such events can be distinguished from the normal, uninitiated AMI
/// messages that get sent by the system when system events occur.
class Reaction {
public:
    Reaction() = default;
    Reaction(Reaction const &) = default;
    Reaction(Reaction &&) noexcept = default;
    virtual ~Reaction() = default;

    Reaction& operator=(Reaction const &) = default;
    Reaction& operator=(Reaction &&) noexcept = default;

    /// @brief Checks the Status field of the message and returns \c true if the field has a success value.
    ///
    /// @return \c true if the Status field has a success value.
    virtual bool is_success() const = 0;

    /// @brief Returns a string containing the KeyValDict representation of the data for this object.
    ///
    /// @return String representation of the data for this object.
    virtual std::string to_string() const = 0;

protected:
    /// @brief Returns \c true if \c status contains values indicating that the AMI action completed successfully.
    ///
    /// @return \c true if \c status contains a value indicating that the AMI action was successful.
    ///
    /// @param status String value containing the Status field for this object.
    static bool is_success(std::string const &status);
};

}

#endif
