#ifndef NUTMEG_FLAGCACHE_H
#define NUTMEG_FLAGCACHE_H

#include <QCache>
#include "nutmeg.h"

namespace Nutmeg
{
class Flag;  // Forward declaration of the class

class FlagCache : public QCache<Key, Flag*>
{
public:
    explicit FlagCache(int capacity = 1000) : QCache<Key, Flag*>(capacity) {}

    Flag* getFlagFromCache(Key id, Flag* (*fetchMethod)(Key))
    {
        if (contains(id))  
        {
            return *object(id);
        }
        else
        {
            Flag* result = fetchMethod(id);
            if (result)
            {
                Flag** tempPtr = new Flag*[1];
                tempPtr[0] = result;
                insert(id, tempPtr);
                delete[] tempPtr;  // Clean up the temporary array
            }
            return result;
        }
    }

    void invalidateFlagCache(Key id)
    {
        remove(id);  
    }
};

extern FlagCache flagCache;  // Declare the global cache instance

}

#endif // NUTMEG_FLAGCACHE_H
