#ifndef __SAFIRE_UTILS_H__
#define __SAFIRE_UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <memory.h>

#include <cglm/cglm.h>

#define X 0
#define Y 1
#define Z 2
#define W 3

#define R 0
#define G 1
#define B 2
#define A 3

#ifndef NDEBUG
# include <assert.h>
# define SAFIRE_ASSERT(x, message) assert(x && message)
# define SAFIRE_ASSERT2(x, y, message) assert(x && y && message)
# define SAFIRE_ASSERT3(x, y, z, message) assert(x && y && z && message)
# define SAFIRE_ASSERT4(x, y, z, w, message) assert(x && y && z && w && message)
#else
# define SAFIRE_ASSERT(x, message)
# define SAFIRE_ASSERT2(x, y, message)
# define SAFIRE_ASSERT3(x, y, z, message)
# define SAFIRE_ASSERT4(x, y, z, w, message)
#endif // NDEBUG

#endif // __SAFIRE_UTILS_H__