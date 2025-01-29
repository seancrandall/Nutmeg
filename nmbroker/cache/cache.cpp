#include "cache.h"
#include "objects/object.h"
#include "objects/appointment.h"
#include "objects/copyrightmatter.h"
#include "objects/document.h"
#include "objects/enterprise.h"
#include "objects/entity.h"
#include "objects/filing.h"
#include "objects/matter.h"
#include "objects/patentmatter.h"
#include "objects/person.h"
#include "objects/response.h"
#include "objects/task.h"
#include "objects/trademarkmatter.h"

#include "settings.h"

namespace Nutmeg
{
Settings cs;

cache<Appointment> appointmentCache (cs.objectCacheSize);
cache<CopyrightMatter> copyrightMatterCache(cs.objectCacheSize);
cache<Document> documentCache(cs.objectCacheSize);
cache<Enterprise> enterpriseCache(cs.objectCacheSize);
cache<Entity> entityCache(cs.objectCacheSize);
cache<Filing> filingCache(cs.objectCacheSize);
cache<Matter> matterCache(cs.objectCacheSize);
cache<Object> objectCache(cs.objectCacheSize);
cache<PatentMatter> patentMatterCache(cs.objectCacheSize);
cache<Person> personCache(cs.objectCacheSize);
cache<Response> responseCache(cs.objectCacheSize);
cache<Task> taskCache(cs.objectCacheSize);
cache<TrademarkMatter> trademarkMatterCache(cs.objectCacheSize);
cache<Object> dbcache(cs.objectCacheSize);

Settings set;

template<>
cache<Appointment>& getCache<Appointment>() { return appointmentCache; }

template<>
cache<CopyrightMatter>& getCache<CopyrightMatter>() { return copyrightMatterCache; }

template<>
cache<Document>& getCache<Document>() { return documentCache; }

template<>
cache<Enterprise>& getCache<Enterprise>() { return enterpriseCache; }

template<>
cache<Entity>& getCache<Entity>() { return entityCache; }

template<>
cache<Filing>& getCache<Filing>() { return filingCache; }

template<>
cache<Matter>& getCache<Matter>() { return matterCache; }

template<>
cache<Object>& getCache<Object>() { return objectCache; }

template<>
cache<PatentMatter>& getCache<PatentMatter>() { return patentMatterCache; }

template<>
cache<Person>& getCache<Person>() { return personCache; }

template<>
cache<Response>& getCache<Response>() { return responseCache; }

template<>
cache<Task>& getCache<Task>() { return taskCache; }

template<>
cache<TrademarkMatter>& getCache<TrademarkMatter>() { return trademarkMatterCache; }

void initCaches() {
    // Initialization logic if needed
}
}
