#ifndef PROPERTY_H
#define PROPERTY_H

// Define the Property macro
#define Property(getMethod, setMethod) __declspec(property(get = getMethod, put = setMethod))
// Example: Property(getMethod, setMethod) int myValue;

// Define the Property macro without a setter
#define ReadOnlyProperty(getMethod) __declspec(property(get = getMethod))

#endif // PROPERTY_H
