#ifndef NUTMEG_CACHE_H
#define NUTMEG_CACHE_H

#include <QCache>
#include "nutmeg.h"

namespace Nutmeg
{
class Object;
class Appointment;
class CopyrightMatter;
class Document;
class Enterprise;
class Entity;
class Filing;
class Matter;
class PatentMatter;
class Person;
class Response;
class Task;
class TrademarkMatter;

// Forward declaration for cache
template<typename T> class cache;

// Now, instead of a global cache, we'll use this function to get the correct cache
template<typename T>
cache<T>& getCache();

template<typename T>
class cache : public QCache<Key, T*>
{
public:
    cache(int capacity = 200) : QCache<Key, T*>(capacity) {}

    static T* getObjectFromCache(Key id, T* (*fetchMethod)(Key), cache<T>& cacheInstance)
    {
        if(cacheInstance.contains(id))
        {
            return *cacheInstance.object(id);
        }
        else
        {
            T* result = fetchMethod(id);
            if(result)
            {
                T** tempPtr = new T*[1];
                tempPtr[0] = result;
                cacheInstance.insert(id, tempPtr);
                delete[] tempPtr; // Clean up the temporary pointer array
            }
            return result;
        }
    }

    static void invalidateObjectCache(Key id, cache<T>& cacheInstance)
    {
        cacheInstance.remove(id);
    }
};

extern cache<Appointment> appointmentCache;
extern cache<CopyrightMatter> copyrightMatterCache;
extern cache<Document> documentCache;
extern cache<Enterprise> enterpriseCache;
extern cache<Entity> entityCache;
extern cache<Filing> filingCache;
extern cache<Matter> matterCache;
extern cache<Object> objectCache;
extern cache<PatentMatter> patentMatterCache;
extern cache<Person> personCache;
extern cache<Response> responseCache;
extern cache<Task> taskCache;
extern cache<TrademarkMatter> trademarkMatterCache;
extern cache<Object> dbcache;

void initCaches();
}

#endif // NUTMEG_CACHE_H
