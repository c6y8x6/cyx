#ifdef _CMATH_ //<cmath>


float indicate(bool condition) {
	return static_cast<float>(condition);
}

template<typename T>
T mod(T a, T b) {
	if constexpr (std::is_integral_v<T>) {
		static_assert(std::is_signed_v<T>);
		assert(b > 0);
		return (a % b + b) % b;
	}
	else if constexpr (std::is_floating_point_v<T>) {
		assert(b > 0);
		return std::fmod((std::fmod(a, b) + b), b);
	}
	return (a % b + b) % b;
}

template<typename Tf>
Tf inv(Tf a) {
	static_assert(std::is_floating_point_v<Tf>);
	return static_cast<Tf>(1) / a;
}

template<typename Tf>
bool isClose(Tf x1, Tf x2, Tf delta = epsilon_v<Tf>) {
	static_assert(std::is_floating_point_v<Tf>);
	return abs(x1 - x2) <= delta;
}

float sgn(float x) {
	return (float)((x > 0) - (x < 0));
}

template<typename Ti>
bool isprime(Ti n) {
	static_assert(std::is_integral_v<Ti>);
	if (n <= 1) { return false; }
	if (n == 2) { return true; }
	if (n % 2 == 0) { return false; }
	for (Ti i = 3; i * i <= n; i += 2) {
		if (n % i == 0) { return false; }
	}
	return true;
}

#endif //_CMATH_