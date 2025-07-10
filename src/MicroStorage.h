#pragma once
/**
 * @file MicroStorage.h
 * @brief A lightweight abstraction for storing and retrieving typed key-value pairs using ESP32 Preferences.
 */
#include <Arduino.h>
#include <Preferences.h>
#include <tuple>
#include <utility>

/**
 * @class MicroStorage
 * @brief Provides static methods to persist and retrieve multiple key-value entries using ESP32 Preferences.
 */
class MicroStorage {
public:
    /**
     * @brief Retrieves values for the given entries from the specified namespace in Preferences.
     * 
     * @tparam Pairs Variadic template for Entry-derived types.
     * @param ns The Preferences namespace.
     * @param pairs One or more Entry instances.
     * @return A tuple containing the values read from Preferences.
     */
    template<typename... Pairs>
    static auto get(const char *ns, Pairs&&... pairs) -> std::tuple<decltype(pairs.get(std::declval<Preferences&>()))...> {
        Preferences prefs;
        prefs.begin(ns, true);
        using TupleType = std::tuple<decltype(pairs.get(prefs))...>;
        TupleType values = std::make_tuple(pairs.get(prefs)...);
        prefs.end();
        return values;
    }

    /**
     * @brief Persists the values of the given entries into the specified namespace in Preferences.
     * 
     * @tparam Pairs Variadic template for Entry-derived types.
     * @param ns The Preferences namespace.
     * @param pairs One or more Entry instances.
     */
    template<typename... Pairs>
    static void set(const char *ns, const Pairs&... pairs) {
        Preferences prefs;
        prefs.begin(ns, false);
        (pairs.set(prefs), ...);
        prefs.end();
    }
};

/**
 * @brief Abstract base class representing a typed key-value pair.
 * 
 * @tparam T Type of the value.
 */
template<typename T>
class Entry {
protected:
    const char *key;
    T value;

public:
    Entry(const char *key, const T& value) : key(key), value(value) {}
    virtual ~Entry() = default;

    virtual T get(Preferences& prefs) const = 0;
    virtual void set(Preferences& prefs) const = 0;
};

/**
 * @brief Represents a key-value pair where the value is a String.
 */
class StringEntry : public Entry<String> {
public:
    StringEntry(const char* key, const String& value) : Entry<String>(key, value) {}

    String get(Preferences& prefs) const override {
        return prefs.getString(key, value);
    }

    void set(Preferences& prefs) const override {
        prefs.putString(key, value);
    }
};

/**
 * @brief Represents a key-value pair where the value is an int.
 */
class IntEntry : public Entry<int> {
public:
    IntEntry(const char* key, int value) : Entry<int>(key, value) {}

    int get(Preferences& prefs) const override {
        return prefs.getInt(key, value);
    }

    void set(Preferences& prefs) const override {
        prefs.putInt(key, value);
    }
};
