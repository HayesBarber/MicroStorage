#pragma once
#include <Arduino.h>
#include <Preferences.h>
#include <tuple>

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

class MicroStorage {
public:
    template<typename... Pairs>
    static auto get(const char *ns, Pairs&&... pairs) -> std::tuple<decltype(pairs.get(std::declval<Preferences&>()))...> {
        Preferences prefs;
        prefs.begin(ns, true);
        using TupleType = std::tuple<decltype(pairs.get(prefs))...>;
        TupleType values = std::make_tuple(pairs.get(prefs)...);
        prefs.end();
        return values;
    }
};
