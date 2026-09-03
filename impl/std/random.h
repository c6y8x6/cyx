#ifdef _RANDOM_ //<random>
static std::random_device random_device;
std::mt19937 gen(random_device());

template <typename Ti>
using irand = std::uniform_int_distribution<Ti>;
template<typename Tf>
using frand = std::uniform_real_distribution<Tf>;

using int_rand = irand<int>;
using float_rand = frand<double>;
#endif //_RANDOM_