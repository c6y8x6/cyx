#ifdef _ITERATOR_ //<iterator>


class bool_vector {
	using Bool = bool;
	using schar = signed char;

	using value_type = Bool;
	using reference = Bool&;
	using const_reference = const Bool&;
	using size_type = size_t;

private:

	class iterator {
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = Bool;
		using difference_type = std::ptrdiff_t;
		using pointer = Bool*;
		using reference = Bool&;


		iterator(std::vector<schar>::iterator iter) : iter(iter) {}
		Bool& operator*() { return reinterpret_cast<Bool&>(*iter); }

		iterator& operator++() {
			++iter;
			return *this;
		}

		iterator& operator--() {
			--iter;
			return *this;
		}

		bool operator!=(const iterator& other) {
			return iter != other.iter;
		}

	public:
		std::vector<schar>::iterator iter;
	};

	using const_iterator = const iterator;

public:
	bool_vector() {}

	bool_vector(std::initializer_list<Bool> list) {
		Data.reserve(list.size());
		std::copy(list.begin(), list.end(), std::back_inserter(Data));
	}

	iterator begin() { return iterator(Data.begin()); }
	iterator end() { return iterator(Data.end()); }

	using reverse_iterator = std::reverse_iterator<iterator>;
	reverse_iterator rbegin() { return reverse_iterator(end()); }
	reverse_iterator rend() { return reverse_iterator(begin()); }

	void assign(size_type count, const Bool& value) {
		Data.assign(count, static_cast<schar>(value));
	}

	template< class InputIt >
	void assign(InputIt first, InputIt last) {
		Data.assign(first, last);
	}

	void assign(std::initializer_list<Bool> ilist) {
		Data.clear();
		Data.reserve(ilist.size());
		std::copy(ilist.begin(), ilist.end(), std::back_inserter(Data));
	}

	auto get_allocator() const { return Data.get_allocator(); }

	reference at(size_type pos) { return reinterpret_cast<reference>(Data.at(pos)); }
	const_reference at(size_type pos) const { return reinterpret_cast<const_reference>(Data.at(pos)); }

	reference operator[](size_type pos) { return reinterpret_cast<reference>(Data[pos]); }
	const_reference operator[](size_type pos) const { return reinterpret_cast<const_reference>(Data[pos]); }

	reference front() { return reinterpret_cast<reference>(Data.front()); }
	const_reference front() const { return reinterpret_cast<const_reference>(Data.front()); }

	reference back() { return reinterpret_cast<reference>(Data.back()); }
	const_reference back() const { return reinterpret_cast<const_reference>(Data.back()); }

	Bool* data() { return reinterpret_cast<Bool*>(Data.data()); }
	const Bool* data() const { return reinterpret_cast<const Bool*>(Data.data()); }

	bool empty() const { return Data.empty(); }

	size_type size() const { return Data.size(); }

	size_type max_size() const { return Data.max_size(); }

	void reserve(size_type new_cap) { Data.reserve(new_cap); }

	size_type capacity() const { return Data.capacity(); }

	void shrink_to_fit() { Data.shrink_to_fit(); }

	void clear() { Data.clear(); }

	iterator insert(const_iterator pos, const Bool& value) { return iterator(Data.insert(pos.iter, value)); }
	iterator insert(const_iterator pos, Bool&& value) { return iterator(Data.insert(pos.iter, std::forward<Bool>(value))); }
	iterator insert(const_iterator pos, size_type count, const Bool& value) { return iterator(Data.insert(pos.iter, count, value)); }
	template< class InputIt >
	iterator insert(const_iterator pos, InputIt first, InputIt last) { return iterator(Data.insert<InputIt>(pos.iter, first, last)); }
	iterator insert(const_iterator pos, std::initializer_list<Bool> ilist) {
		size_t pos_off = pos.iter - Data.begin();
		size_t org_size = Data.size();
		Data.resize(org_size + ilist.size());
		auto new_pos = Data.begin() + pos_off;
		auto new_orgend = Data.begin() + org_size;
		std::copy_backward(new_pos, new_orgend, Data.end());
		std::copy(ilist.begin(), ilist.end(), new_pos);
		return iterator(new_pos);
	}

	template< class... Args >
	iterator emplace(const_iterator pos, Args&&... args) { return iterator(Data.emplace(pos.iter, args)); }

	iterator erase(const_iterator pos) { return iterator(Data.erase(pos.iter)); }
	iterator erase(const_iterator first, const_iterator last) { return iterator(Data.erase(first.iter, last.iter)); }

	void push_back(const Bool& value) { Data.push_back(value); }
	void push_back(Bool&& value) { Data.push_back(std::forward<Bool>(value)); }

	template< class... Args >
	void emplace_back(Args&&... args) { Data.emplace_back(args); }
	template< class... Args >
	reference emplace_back(Args&&... args) { return reinterpret_cast<reference>(Data.emplace_back(args)); }

	void pop_back() { Data.pop_back(); }

	void resize(size_type count) { Data.resize(count); }
	void resize(size_type count, const value_type& value) { Data.resize(count, value); }

	void swap(bool_vector& other) noexcept { Data.swap(other.Data); }

private:
	std::vector<schar> Data;
};


bool operator== (const bool_vector& lhs, const bool_vector& rhs) {
	size_t size = lhs.size();
	if (size != rhs.size()) { return false; }
	for (size_t i = 0; i < size; i++) {
		if (lhs[i] != rhs[i]) { return false; }
	}
	return true;
}


#endif //_ITERATOR_