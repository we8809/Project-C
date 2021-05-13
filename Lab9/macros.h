#ifndef MACROS_H
#define MACROS_H

#include <string.h>

#define MAX(x, y) (((x) >= (y)) ? (x) : (y))

#define MIN(x, y) (((x) >= (y)) ? (y) : (x))

#define ABS(x)    (((x) < 0) ? ((x) * -1) : (x))

#define RANGE(curr, min, max) for ((curr) = (min); (curr) <= (max); (curr)++)

#define RANGE_DESC(curr, max, min) for ((curr) = (max); (curr) >= (min); (curr)--)

#define SET(ary, start, count, value)                                   \
        {                                                               \
            size_t i;                                                   \
            for (i = (start); i < ((start) + (count)); i++) {           \
                *(ary + i) = (value);                                   \
            }                                                           \
        }

#endif  /* MACROS_H */
