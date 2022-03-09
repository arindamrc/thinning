#ifndef GLOBALS_H
#define GLOBALS_H

#include <limits>

#define MAXVAL(U) std::numeric_limits<U>::max()
#define MINVAL(U) std::numeric_limits<U>::min()

using real_t = double;
using integer_t = int;
using unsigned_t = unsigned int;

static const real_t EPSILON = 1e-6;
static const real_t PI = 3.1415926;
static const real_t MAXREAL = MAXVAL(real_t);

#endif // GLOBALS_H
