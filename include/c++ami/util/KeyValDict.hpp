// Copyright (c) 2026 Christopher L Walker
// SPDX-License-Identifier: MIT

#ifndef UTIL_KEYVALPAIR_HPP
#define UTIL_KEYVALPAIR_HPP

#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace cpp_ami::util {

/// @class KeyValDict
///
/// @brief Contains a dictionary of key-value pairs.
///
/// The Asterisk Management Interface (AMI) communicates by sending groups of key-value pairs. This object takes a text
/// stream containing an AMI Action/Event and converts it into an object.
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
    /// @param optional_keys Optional keys for the object.
    explicit KeyValDict(std::vector<std::string> ordered_keys, std::unordered_set<std::string> optional_keys = {});

    virtual ~KeyValDict() = default;

    KeyValDict &operator=(KeyValDict const &) = default;
    KeyValDict &operator=(KeyValDict &&) noexcept = default;

    /// @brief Returns \c true if the object has a key of value \c key.
    ///
    /// @return \c true if a key named \c key exists in the object.
    ///
    /// @param key Key to search for in collection.
    bool hasKey(std::string const &key) const;

    /// @brief Returns the value for \c key in the collection.
    ///
    /// @return Value for key \c key.
    ///
    /// @param key Key to return value for.
    std::string &operator[](std::string const &key);

    /// @brief Returns the value for \c key in the collection.
    ///
    /// @return Value for key \c key.
    ///
    /// @param key Key to return value for.
    std::string const &operator[](std::string const &key) const;

    /// @brief Returns the value for \c key in the collection.
    ///
    /// @return Value for key \c key if key exists. std::nullopt if the value doesn't exist.
    ///
    /// @param key Key to return value for.
    std::optional<std::string> getValue(std::string const &key) const;

    /// @brief Sets the value for key \c key to value \c val.
    ///
    /// @param key Key to set value for.
    /// @param val Value to set key to.
    void setValue(std::string const &key, std::string val);

    /// @brief Returns number of keys in object.
    ///
    /// @return Number of keys in object.
    virtual size_t count() const;

    /// @brief Returns an AMI string representation of the object.
    ///
    /// @return AMI string representation of the object.
    virtual std::string toString() const;

    /// @brief Returns \c true if the key value is optional for this object.
    ///
    /// @return \c true if key is optional for this object.
    bool isOptional(std::string const &key) const;

    /// @brief Splits value into separate values.
    ///
    /// @param value Value to split.
    ///
    /// @return List of values.
    std::vector<std::string> split(std::string const &value) const;

    /// @brief Encodes multiple values into a value.
    ///
    /// @param key Key to set values on.
    /// @param values Collection of values.
    void setValues(std::string const &key, std::vector<std::string> const &values);

    /// @brief Decodes multiple values from an array.
    ///
    /// @return Collection of values.
    ///
    /// @param key Ket to pull value from.
    std::vector<std::string> getValues(std::string const &key) const;

    /// @brief Encodes a collection of values into a string.
    ///
    /// @return String representation of a collection of values.
    ///
    /// @param key Key of value.
    /// @param values Collection of values.
    static std::string toString(std::string const &key, std::vector<std::string> const &values);

    /// @brief Decodes a collection of values from a string.
    ///
    /// @return Collection of values.
    ///
    /// @param key Key of value.
    /// @param value String representation of values.
    static std::vector<std::string> fromString(std::string const &key, std::string const &value);

    /// @brief Returns an escaped string for use with AMI. Strings containing double quotes (") should be escaped
    ///        through this function before being used as an AMI value.
    ///
    /// @return Escaped string.
    ///
    /// @param value Value to escape.
    static std::string escape(std::string const &value);

protected:
    /// @brief Initializes the object using the key/value pairs found in \c event_buf.
    ///
    /// @param event_buf String containing AMI key/value pairs.
    void setMessage(std::string const &event_buf);

private:
    std::vector<std::string> ordered_keys_;               ///< Collection of ordered keys for object.
    std::unordered_set<std::string> optional_keys_;       ///< Optional keys for object.
    std::unordered_map<std::string, std::string> values_; ///< Collection of Key/value pairs.
};

} // namespace cpp_ami::util

#endif
