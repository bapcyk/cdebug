#include <string.h>

// use level name as 1st argument of out DBG_PRN_IMPL
#define DBG_LEVEL_IS_ARG
#define DBG_LEVEL_FMT "[%s] "
#include "dbg.h"

// define levels
#define HARDWARE_LEVEL   DBG_LEVEL_ALIAS(1)
#define WRAPPER_LEVEL   DBG_LEVEL_ALIAS(2)
#define APP_LEVEL       DBG_LEVEL_ALIAS(3)

#undef DBG_LEVEL
#define _DBG_LEVEL (HARDWARE_LEVEL|APP_LEVEL)
#define DBG_LEVEL _DBG_LEVEL

void main() {
    char dim[] = "0123456789Hello World!";
    DBG_PRN(HARDWARE_LEVEL, "There %d\n", 1);
    DBG_PRN(WRAPPER_LEVEL, "There %d\n", 2);
    // example how to locally change DBG_LEVEL and then restore
#undef DBG_LEVEL
#define DBG_LEVEL 0
    DBG_PRN(APP_LEVEL, "There %d\n", 3);
#undef DBG_LEVEL
#define DBG_LEVEL _DBG_LEVEL
    DBG_DUMP(APP_LEVEL, dim, strlen(dim));
    DBG_PRN_UL(APP_LEVEL, "%s\n", "Done.");
}
