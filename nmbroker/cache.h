#ifndef NUTMEG_CACHE_H
#define NUTMEG_CACHE_H

#include <QCache>
#include "nutmeg.h"

namespace Nutmeg
{
class Object;

// Forward declaration for cache
template<typename T> class cache;

// Now, instead of a global cache, we'll use this function to get the correct cache
template<typename T>
cache<T>& getCache();

template<typename T>
class cache : public QCache<Key, T*>
{
public:
    cache(int capacity = 100) : QCache<Key, T*>(capacity) {}

    static T* getObjectFromCache(Key id, T* (*fetchMethod)(Key), cache<T>& cacheInstance)
    {
        if(cacheInstance.contains(id))
        {
            return *cacheInstance.object(id); // No need for dynamic_cast here since we're using the correct cache
        }
        else
        {
            T* result = fetchMethod(id);
            if(result)
            {
                cacheInstance.insert(id, result);
            }
            return result;
        }
    }

    static void invalidateObjectCache(Key id, cache<T>& cacheInstance)
    {
        cacheInstance.remove(id);
    }
};

// Example of how to define and use specific caches
//extern cache<Object> dbcache;
//extern cache<Appointment> appointmentCache;

// Assume these are defined in cache.cpp or similar
//template<>
//cache<Object>& getCache<Object>() { return dbcache; }

//template<>
//cache<Appointment>& getCache<Appointment>() { return appointmentCache; }

// Function to initialize caches if needed
//void initCaches();
}

#endif // NUTMEG_CACHE_H
