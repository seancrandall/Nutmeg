#ifndef NUTMEG_CACHE_H
#define NUTMEG_CACHE_H

#include <QCache>
#include <functional>

#include "nutmeg.h"

namespace Nutmeg
{
class Object;

template<typename T>
class cache : public QCache<Key, T*>
{
public:
    cache(int capacity = 100) : QCache<Key, T*>(capacity) {}

    template<typename U>
    /**
     * @brief getObjectFromCache
     * @param id Primary key of the object to fetch from cache
     * @param fetchMethod is a pointer to the function that <T> uses to get a record from the DB
     * @param obj The object actually calling the function. If calling from inside class, pass "this"
     * @return Rerturns a pointer to
     */
    static T* getObjectFromCache(Key id, T* (U::*fetchMethod)(Key), U* obj)
    {
        if(cache<T>::contains(id))
        {
            return *cache<T>::object(id);
        }
        else
        {
            T* result = (obj->*fetchMethod)(id);
            if(result)
            {
                cache<T>::insert(id, result);
            }
            return result;
        }
    }

    static void invalidateObjectCache(Key id)
    {
        cache<T>::remove(id);
    }
};

// Global cache instances for each type if needed
extern cache<Object> dbcache;

// Initialize caches
void initCaches();
}

#endif // NUTMEG_CACHE_H
