#ifndef _CYX_
#define _CYX_

#include <cassert>
#include <cstdint>
#include <climits>
#include <cmath>
#include <array>
#include <vector>
#include <numeric>
#include <functional>



namespace cyx {


#include <impl/std/basic.h>

#include <impl/std/random.h>
#include <impl/std/numbers.h>
#include <impl/std/math_func.h>

#include <impl/std/range.h>
#include <impl/std/circular.h>
#include <impl/std/bool_vector.h>
#include <impl/std/reverse.h>

#include <impl/std/string.h>


} //namespace cyx



#ifdef _RANDOM_ //<random>
using cyx::int_rand;
using cyx::float_rand;
#endif //_RANDOM_

#ifdef _ITERATOR_ //<iterator>
using cyx::range;
using cyx::rrange;
using cyx::frange;
#endif //_ITERATOR_


#endif //_CYX_