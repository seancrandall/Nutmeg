#ifndef NUTMEG_CACHE_H
#define NUTMEG_CACHE_H

#include <QCache>
#include "nutmeg.h"

namespace Nutmeg
{
class Object;

// Forward declaration of the cache to allow dbcache to be declared first
template<typename T> class cache;

// Declare dbcache here before defining the cache class
extern cache<Object> dbcache;

template<typename T>
class cache : public QCache<Key, T*>
{
public:
    cache(int capacity = 100) : QCache<Key, T*>(capacity) {}

    static T* getObjectFromCache(Key id, T* (*fetchMethod)(Key))
    {
        // Use the global instance of cache (dbcache) to call member functions
        if(dbcache.contains(id))
        {
            return *dbcache.object(id);
        }
        else
        {
            T* result = fetchMethod(id);
            if(result)
            {
                dbcache.insert(id, &result);
            }
            return result;
        }
    }

    static void invalidateObjectCache(Key id)
    {
        dbcache.remove(id);
    }
};

// Function to initialize caches if needed
void initCaches();
}

#endif // NUTMEG_CACHE_H
