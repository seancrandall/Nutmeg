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

namespace Nutmeg
{
cache<Appointment> appointmentCache;
cache<CopyrightMatter> copyrightMatterCache;
cache<Document> documentCache;
cache<Enterprise> enterpriseCache;
cache<Entity> entityCache;
cache<Filing> filingCache;
cache<Matter> matterCache;
cache<Object> objectCache;
cache<PatentMatter> patentMatterCache;
cache<Person> personCache;
cache<Response> responseCache;
cache<Task> taskCache;
cache<TrademarkMatter> trademarkMatterCache;
cache<Object> dbcache;

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
