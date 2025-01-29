#include "deadlinecache.h"
#include "settings.h"

namespace Nutmeg
{
    Settings _ds;
    DeadlineCache deadlineCache(_ds.deadlinesCacheSize);
}
