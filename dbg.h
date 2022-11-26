/* Multilevel debug/log
 * 
 *  Example:
 *
 *  // use level name as 1st argument of out DBG_PRN_IMPL:
 *
 *  #define DBG_LEVEL_IS_ARG
 *  #define DBG_PRN_IMPL(L, F, ...) printf(F, L, __VA_ARGS__)
 *  #include "dbg.h"
 *  // ^^^ only for ex. how to use own DBG_PRN_IMPL with level name
 *  //     as parameter
 *  
 *  // define levels
 *  #define HARWARE_LEVEL   DBG_LEVEL_ALIAS(1)
 *  #define WRAPPER_LEVEL   DBG_LEVEL_ALIAS(2)
 *  #define APP_LEVEL       DBG_LEVEL_ALIAS(3)
 *  
 *  #undef DBG_LEVEL
 *  #define DBG_LEVEL (HARWARE_LEVEL|APP_LEVEL)
 *  
 *  void main() {
 *          DBG_PRN(HARWARE_LEVEL, "%s: There %d\n", 1);
 *          DBG_PRN(WRAPPER_LEVEL, "%s: There %d\n", 2); // no effect
 *          DBG_PRN(APP_LEVEL, "%s: There %d\n", 3);
 *  }
 */

#ifndef __DBG_H
#define __DBG_H

#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>

#define DBG_LEVEL_ALIAS(L) (1<<(L))

#ifndef DBG_LEVEL
#define DBG_LEVEL 0
#endif

#ifndef DBG_LEVEL_FMT
#define DBG_LEVEL_FMT "%s: "
#endif

#define _DBG_PRN_IMPL_L(L, F, ...) printf(F, L, __VA_ARGS__)
#define _DBG_PRN_IMPL(F, ...) printf(F, __VA_ARGS__)
#define _DBG_DUMP_IMPL_L(L, p, sz) do { \
    int i; \
    for (i=0; i<(sz); i++) { \
        if (0 == i%16) { \
            DBG_PRN_IMPL("", "%s%s%08" PRIXPTR ": %02X "/*"%s%s%08lX: %02X "*/, i?"\n":"", \
                    16*i/16 + (uintptr_t)p, (uint8_t)(p)[i]); \
        } else if (0 == (i-7)%16) { \
            DBG_PRN_IMPL("", "%s%02X|", (uint8_t)(p)[i]); \
        } else { \
            DBG_PRN_IMPL("", "%s%02X ", (uint8_t)(p)[i]); \
        } \
    } \
    DBG_PRN_IMPL("", "%s%s", "\n"); \
} while (0)
#define _DBG_DUMP_IMPL(p, sz) do { \
    int i; \
    for (i=0; i<(sz); i++) { \
        if (0 == i%16) { \
            DBG_PRN_IMPL("%s%08" PRIXPTR ": %02X "/*"%s%08lX: %02X "*/, i?"\n":"", \
                    16*i/16 + (uintptr_t)p, (uint8_t)(p)[i]); \
        } else if (0 == (i-7)%16) { \
            DBG_PRN_IMPL("%02X|", (uint8_t)(p)[i]); \
        } else { \
            DBG_PRN_IMPL("%02X ", (uint8_t)(p)[i]); \
        } \
    } \
    DBG_PRN_IMPL("%s", "\n"); \
} while (0)

#ifdef DBG_LEVEL_IS_ARG

#ifndef DBG_PRN_IMPL
#define DBG_PRN_IMPL _DBG_PRN_IMPL_L
#endif
#ifndef DBG_DUMP_IMPL
#define DBG_DUMP_IMPL _DBG_DUMP_IMPL_L
#endif

#define DBG_PRN(L, F, ...) if ((DBG_LEVEL) & (L)) DBG_PRN_IMPL(#L, DBG_LEVEL_FMT F, __VA_ARGS__)
#define DBG_DUMP(L, ...) if ((DBG_LEVEL) & (L)) DBG_DUMP_IMPL(#L, __VA_ARGS__)

#else

#ifndef DBG_PRN_IMPL
#define DBG_PRN_IMPL _DBG_PRN_IMPL
#endif
#ifndef DBG_DUMP_IMPL
#define DBG_DUMP_IMPL _DBG_DUMP_IMPL
#endif

#define DBG_PRN(L, ...) if ((DBG_LEVEL) & (L)) DBG_PRN_IMPL(__VA_ARGS__)
#define DBG_DUMP(L, ...) if ((DBG_LEVEL) & (L)) DBG_DUMP_IMPL(__VA_ARGS__)

#endif

/// Print without level
#define DBG_PRN_UL(L, F, ...) if ((DBG_LEVEL) & (L)) DBG_PRN_IMPL("", "%s" F, __VA_ARGS__)

#endif /* EOF */

