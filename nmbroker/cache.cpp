#include "cache.h"

#include "objects/object.h"

namespace Nutmeg
{
cache<Object> dbcache(200); // Capacity for 200 objects

void initCaches() {
    // Initialization logic if needed
}

} //namespace Nutmeg
