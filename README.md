# MicroStorage

MicroStorage is a lightweight C++ library for ESP32 projects using the Arduino framework. It simplifies reading and writing multiple key-value pairs to ESP32's Preferences storage with minimal boilerplate.

## Features

- Type-safe abstraction for key-value entries.
- Support for `String` and `int` out of the box.
- Batch `get()` and `set()` operations with clean tuple unpacking.
- Customizable via templated base class.
- Clear one or more namespaces using `MicroStorage::clear(...)`.

## Example

```cpp
auto [username, retryCount] = MicroStorage::get(
    "my_namespace",
    StringEntry("username", "defaultUser"),
    IntEntry("retryCount", 3)
);

MicroStorage::set(
    "my_namespace",
    StringEntry("username", username),
    IntEntry("retryCount", retryCount + 1)
);
```

## Clearing Namespaces

You can clear one or more namespaces from preferences:

```cpp
bool success = MicroStorage::clear("namespace1", "namespace2");
if (!success) {
    Serial.println("Failed to clear one or more namespaces");
}
```

## Entry Types

- `StringEntry`: for `String` values.
- `IntEntry`: for `int` values.
- `Entry<T>`: base class for defining custom entry types.

## Notes

- The `value` in each entry serves as both the default for `get()` and the source value for `set()`.
- Namespace strings should be unique to avoid key collisions.
