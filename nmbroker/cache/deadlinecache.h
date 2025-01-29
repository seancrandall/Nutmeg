#ifndef NUTMEG_DEADLINECACHE_H
#define NUTMEG_DEADLINECACHE_H

#include <QCache>
#include "nutmeg.h"

namespace Nutmeg
{
class Deadline;  // Forward declaration of the class

class DeadlineCache : public QCache<Key, Deadline*>
{
public:
    explicit DeadlineCache(int capacity = 200) : QCache<Key, Deadline*>(capacity) {}

    Deadline* getDeadlineFromCache(Key id, Deadline* (*fetchMethod)(Key))
    {
        if (contains(id))  // Use 'contains' directly on this instance
        {
            return *object(id);
        }
        else
        {
            Deadline* result = fetchMethod(id);
            if (result)
            {
                Deadline** tempPtr = new Deadline*[1];
                tempPtr[0] = result;
                insert(id, tempPtr);
                delete[] tempPtr;  // Clean up the temporary array
            }
            return result;
        }
    }

    void invalidateDeadlineCache(Key id)
    {
        remove(id);  // Use 'remove' directly on this instance
    }
};

extern DeadlineCache deadlineCache;  // Declare the global cache instance

}

#endif // NUTMEG_DEADLINECACHE_H
