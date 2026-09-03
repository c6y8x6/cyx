#ifdef _ITERATOR_ //<iterator>

template<typename Rng>
class ReverseView {
private:
	Rng& rng;

public:
	explicit ReverseView(Rng& r) : rng(r) {}
	auto begin() const { return std::rbegin(rng); }
	auto end() const { return std::rend(rng); }
};

//C++17
template<typename Rng>
ReverseView(Rng&) -> ReverseView<Rng>;

// overload for const container
template<typename Rng>
class ReverseView<const Rng> {
private:
	const Rng& rng;
public:
	explicit ReverseView(const Rng& r) : rng(r) {}
	auto begin() const { return std::rbegin(rng); }
	auto end() const { return std::rend(rng); }
};


template<typename Rng>
auto reverse(Rng& rng) {
	return cyx::ReverseView(rng);
}

template<typename Rng>
auto reverse(Rng&& rng) {
	return cyx::ReverseView<std::remove_reference_t<Rng>>(rng);
}

#endif //_ITERATOR_