#ifndef CYX_DEFAULT_FLOAT_TYPE
#define CYX_DEFAULT_FLOAT_TYPE double
#endif //CYX_DEFAULT_FLOAT_TYPE


template<class T>
struct Invalid {
	static_assert(false);
};

#define CYX_DEFINE_NUMBER_CONSTANT(name, value) \
template<class T> constexpr T name##_v = Invalid<T>{}; \
template<std::floating_point Tf> constexpr Tf name##_v<Tf> = static_cast<Tf>(value); \
inline constexpr CYX_DEFAULT_FLOAT_TYPE name = name##_v<CYX_DEFAULT_FLOAT_TYPE>


template<class T> constexpr T epsilon_v = Invalid<T>{};

template<std::floating_point Tf>
constexpr Tf epsilon_v<Tf> = std::invoke([](void)->Tf {
	if constexpr (std::same_as<Tf, float>) { return 1e-5f; }
	else if constexpr (std::same_as<Tf, double>) { return 1e-12; }
	else if constexpr (std::same_as<Tf, long double>) { return 1e-15L; }
	else { static_assert(false); }
});

inline constexpr float epsilon_f = epsilon_v<float>;
inline constexpr double epsilon_d = epsilon_v<double>;
inline constexpr long double epsilon_ld = epsilon_v<long double>;

CYX_DEFINE_NUMBER_CONSTANT(inf, std::numeric_limits<CYX_DEFAULT_FLOAT_TYPE>::infinity());


CYX_DEFINE_NUMBER_CONSTANT(sqrt2, 1.414213562373095048801688724209698L);
CYX_DEFINE_NUMBER_CONSTANT(sqrt3, 1.732050807568877293527446341505872L);
CYX_DEFINE_NUMBER_CONSTANT(sqrt5, 2.236067977499789696409173668731276L);
CYX_DEFINE_NUMBER_CONSTANT(sqrt6, 2.449489742783178098197284074705891L);
CYX_DEFINE_NUMBER_CONSTANT(sqrt7, 2.645751311064590590501615753639260L);
CYX_DEFINE_NUMBER_CONSTANT(phi, 1.618033988749894848204586834365638L);

CYX_DEFINE_NUMBER_CONSTANT(pi, 3.141592653589793238462643383279502L);
CYX_DEFINE_NUMBER_CONSTANT(tau, 6.283185307179586476925286766559005L);

CYX_DEFINE_NUMBER_CONSTANT(e, 2.718281828459045235360287471352662L);
CYX_DEFINE_NUMBER_CONSTANT(ln2, 0.693147180559945309417232121458176L);
CYX_DEFINE_NUMBER_CONSTANT(ln3, 1.098612288668109691395245236922525L);
CYX_DEFINE_NUMBER_CONSTANT(ln5, 1.609437912434100374600759333226187L);
CYX_DEFINE_NUMBER_CONSTANT(ln10, 2.302585092994045684017991454684364L);