#ifdef _ITERATOR_ //<iterator>

template<typename Ti>
class range {
	static_assert(std::is_integral_v<Ti>);
private:
	Ti start, stop, step;

public:
	range(Ti stop) : start(0), stop(stop), step(1) {}
	range(Ti start, Ti stop) : start(start), stop(stop), step(1) {}
	range(Ti start, Ti stop, Ti step) : start(start), stop(stop), step(step) {}


	class iterator {
	private:
		Ti current, step;
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = Ti;
		using difference_type = std::ptrdiff_t;
		using pointer = Ti*;
		using reference = Ti&;


		iterator(Ti current, Ti step) : current(current), step(step) {}

		Ti operator*() const { return current; }

		iterator& operator++() {
			current += step;
			return *this;
		}

		iterator& operator--() {
			current += step;
			return *this;
		}

		bool operator!=(const iterator& other) const {
			if (step > 0) { return current < other.current; }
			else { return current > other.current; }
		}
	};

	iterator begin() const { return iterator(start, step); }
	iterator end() const { return iterator(stop, step); }

	using reverse_iterator = std::reverse_iterator<iterator>;
	reverse_iterator rbegin() { return reverse_iterator(iterator(stop, -step)); }
	reverse_iterator rend() { return reverse_iterator(iterator(start, -step)); }
};

using rrange = range<ssize_t>;





template<typename Tf>
class frange {
	static_assert(std::is_floating_point_v<Tf>);
private:
	Tf start, stop, step;

private:
	static Tf extension(bool extend) { return extend ? epsilon<Tf>() : static_cast<Tf>(0); }

public:
	frange(Tf start, Tf stop, Tf step, bool extend = false)
		: start(start), stop(stop + extension(extend)), step(step) {}
	frange(Tf start, Tf stop, int steps, bool extend = false)
		: start(start), stop(stop + extension(extend)), step((stop - start) / steps) {}
	frange(Tf start, int steps, Tf step, bool extend = false)
		: start(start), stop(start + steps * step + extension(extend)), step(step) {}

	class iterator {
	private:
		Tf current, step;
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = Tf;
		using difference_type = std::ptrdiff_t;
		using pointer = Tf*;
		using reference = Tf&;

		iterator(Tf current, Tf step) : current(current), step(step) {}

		Tf operator*() const { return current; }

		iterator& operator++() {
			current += step;
			return *this;
		}

		iterator& operator--() {
			current += step;
			return *this;
		}

		bool operator!=(const iterator& other) const {
			if (step > 0)
				return current < other.current;
			else
				return current > other.current;
		}
	};

	iterator begin() const { return iterator(start, step); }
	iterator end() const { return iterator(stop, step); }

	using reverse_iterator = std::reverse_iterator<iterator>;
	reverse_iterator rbegin() { return reverse_iterator(iterator(stop, -step)); }
	reverse_iterator rend() { return reverse_iterator(iterator(start, -step)); }
};

#endif //_ITERATOR_