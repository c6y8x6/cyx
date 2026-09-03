#define iper(container) ((container).begin()), ((container).end())


using ssize_t = std::make_signed_t<size_t>;


class MainOutside {
public:
	MainOutside()
	{
		;
		//<iostream>
#ifdef _IOSTREAM_
		std::ios::sync_with_stdio(false);
		std::cout << std::boolalpha;
		//std::cout << std::showbase; //std::cout << std::hex << 0; //???
		std::cout << std::showpoint;
#endif //_IOSTREAM_
	}
}MainOutside;


void patient_exit(int v) {
	system("pause");
	std::exit(v);
}

template<typename T>
static void explain_error(T t) {
	std::cerr << t;
}

template<typename T, typename ... Args>
static void explain_error(T t, const Args& ... args) {
	std::cerr << t;
	explain_error(args...);
}

template<typename ...Args>
void report_error(Args&& ... args) {
	std::cerr << "Error: ";
	explain_error(std::forward<Args>(args)...);
	std::cerr << "\n" << std::endl;
}

template<typename ...Args>
void throw_error(Args&& ... args) {
	report_error(std::forward<Args>(args)...);
	patient_exit(-1);
}



template<class ForwardIt, typename ...Args>
void construct_at(ForwardIt iterator, Args&& ... args) {
	std::construct_at(std::addressof(*iterator), std::forward<Args>(args)...);
}

template<class ForwardIt>
void destroy_at(ForwardIt iterator) {
	std::destroy_at(std::addressof(*iterator));
}

template<class ForwardIt>
void destroy_n(ForwardIt iterator, size_t n) {
	for (size_t i = 0; i < n; i++) {
		destroy_at(iterator);
		++iterator;
	}
}

template<class ForwardIt>
void destroy(ForwardIt iterator, ForwardIt end) {
	while (iterator != end) {
		destroy_at(iterator);
		++iterator;
	}
}