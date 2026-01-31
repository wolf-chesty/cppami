// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#ifndef UTIL_KEYVALPAIR_HPP
#define UTIL_KEYVALPAIR_HPP

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace cpp_ami::util {

///
/// @class KeyValDict
///
/// @brief Contains a dictionary of key-value pairs.
///
/// The Asterisk Management Interface (AMI) communicates by sending groups of key-value pairs. This object takes a text
/// stream containing an AMI Action/Event and converts it into an object.
///
class KeyValDict {
public:
    KeyValDict() = delete;
    KeyValDict(KeyValDict const &) = default;
    KeyValDict(KeyValDict &&) noexcept = default;

    /// @brief Constructs an object from \c event_buf string containing the AMI message.
    ///
    /// @param event_buf String buffer containing the textual representation of an AMI Action/Event.
    explicit KeyValDict(std::string event_buf);

    /// @brief Constructs an object containing keys \c ordered_keys.
    ///
    /// @param ordered_keys Ordered keys for the object.
    explicit KeyValDict(std::vector<std::string> ordered_keys);

    virtual ~KeyValDict() = default;

    KeyValDict &operator=(KeyValDict const &) = default;
    KeyValDict &operator=(KeyValDict &&) noexcept = default;

    /// @brief Returns \c true if the object has a key of value \c key.
    ///
    /// @return \c true if a key named \c key exists in the object.
    ///
    /// @param key Key to search for in collection.
    bool has_key(std::string const &key) const;

    /// @brief Returns the value for \c key in the collection.
    ///
    /// @return Value for key \c key.
    ///
    /// @param key Key to return value for.
    std::string& operator[](std::string const &key);

    /// @brief Returns the value for \c key in the collection.
    ///
    /// @return Value for key \c key.
    ///
    /// @param key Key to return value for.
    std::string const& operator[](std::string const &key) const;

    /// @brief Returns the value for \c key in the collection.
    ///
    /// @return Value for key \c key if key exists. std::nullopt if the value doesn't exist.
    ///
    /// @param key Key to return value for.
    std::optional<std::string> get_value(std::string const &key) const;

    /// @brief Sets the value for key \c key to value \c val.
    ///
    /// @param key Key to set value for.
    /// @param val Value to set key to.
    void set_value(std::string const &key, std::string val);

    /// @brief Returns number of keys in object.
    ///
    /// @return Number of keys in object.
    virtual size_t count() const;

    /// @brief Returns an AMI string representation of the object.
    ///
    /// @return AMI string representation of the object.
    virtual std::string to_string() const;

protected:
    /// @brief Initializes the object using the key/value pairs found in \c event_buf.
    ///
    /// @param event_buf String containing AMI key/value pairs.
    void set_message(std::string event_buf);

private:
    std::vector<std::string> ordered_keys_;                 ///< Collection of ordered keys for object.
    std::unordered_map<std::string, std::string> values_;   ///< Collection of Key/value pairs.
};

}

#endif
