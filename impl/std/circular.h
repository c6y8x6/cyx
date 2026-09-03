#ifdef _ITERATOR_ //<iterator>


// only support stateless allocator, like std::allocator
template <typename T, class Alloc = std::allocator<T>>
class circular {
	static_assert(std::is_object_v<T>);
private:

	class iterator {
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using reference = T&;

		iterator(T* ptr, size_t offset, size_t maxSize)
			: start(ptr), p(ptr + offset), maxSize(maxSize)
		{
			assert(ptr != nullptr);
		}

		iterator& take_principle() {
			p = start + cyx::mod<int64_t>(p - start, maxSize);
			return *this;
		}

		T& operator*() const {
			T* q = start + cyx::mod<int64_t>(p - start, maxSize);
			return *q;
		}

		iterator& operator++() {
			p++;
			return *this;
		}

		iterator& operator--() {
			p--;
			return *this;
		}

		bool operator!=(const iterator& other) const {
			return p != other.p;
		}

		T* data() { return p; }
		size_t offset() { return p - start; }

	private:
		T* start;
		T* p;
		size_t maxSize;
	};

public:
	circular(int maxSize) : maxSize(maxSize) { ptr = Alloc().allocate(maxSize); }

	circular(std::initializer_list<T> list) {
		size_ = list.size();
		maxSize = size_;
		ptr = Alloc().allocate(maxSize);
		std::copy(list.begin(), list.end(), ptr);
	}

	~circular() {
		cyx::destroy_n(iterator(ptr, entrance_, maxSize), size_);
		Alloc().deallocate(ptr, maxSize);
		ptr = nullptr;
	}

	circular(const circular& other) {
		ptr = Alloc().allocate(maxSize);
		std::copy(iterator(other.ptr, other.entrance_, other.maxSize),
				  iterator(other.ptr, other.entrance_ + other.size_, other.maxSize), ptr);
		entrance_ = other.entrance_;
		size_ = other.size_;
		maxSize = other.maxSize;
	}

	circular& operator=(const circular& other) {
		if (this != &other) {
			std::copy(iterator(other.ptr, other.entrance_, other.maxSize),
					  iterator(other.ptr, other.entrance_ + other.size_, other.maxSize), ptr);
			entrance_ = other.entrance_;
			size_ = other.size_;
			maxSize = other.maxSize;
		}
		return *this;
	}

	circular(circular&& other) noexcept {
		ptr = other.ptr;
		entrance_ = other.entrance_;
		size_ = other.size_;
		maxSize = other.maxSize;
		other.ptr = nullptr;
	}

	circular& operator=(circular&& other) noexcept {
		if (this != &other) {
			cyx::destroy_n(iterator(ptr, entrance_, maxSize), size_);
			Alloc().deallocate(ptr, maxSize);
			ptr = other.ptr;
			entrance_ = other.entrance_;
			size_ = other.size_;
			maxSize = other.maxSize;
			other.ptr = nullptr;
		}
		return *this;
	}

	void swap(circular& other) noexcept {
		std::swap(ptr, other.ptr);
		std::swap(entrance_, other.entrance_);
		std::swap(size_, other.size_);
		std::swap(maxSize, other.maxSize);
	}

public:
	iterator begin() const { return iterator(ptr, entrance_, maxSize); }
	iterator end() const { return iterator(ptr, entrance_ + size_, maxSize); }

	using reverse_iterator = std::reverse_iterator<iterator>;
	reverse_iterator rbegin() const { return reverse_iterator(end()); }
	reverse_iterator rend() const { return reverse_iterator(begin()); }

	T& operator[] (size_t offset) {
		iterator I(ptr, entrance_ + offset, maxSize);
		return *I;
	}

	T* data() const { return ptr; }
	size_t size() const { return size_; }
	size_t entrance() const { return entrance_; }
	size_t capacity() const { return maxSize; }
	T& front() const { return *iterator(ptr, entrance_, maxSize); }
	T& back() const {
		assert(size_ > 0);
		return *iterator(ptr, entrance_ + size_ - 1, maxSize);
	}
	void clear() {
		cyx::destroy_n(iterator(ptr, entrance_, maxSize), size_);
		entrance_ = 0;
		size_ = 0;
	}
	bool empty() const { return size_ == 0; }

	template <typename... Args>
	void emplace_back(Args&&... args) {
		assert(size_ <= maxSize);
		if (size_ < maxSize) {
			cyx::construct_at(iterator(ptr, entrance_ + size_, maxSize), std::forward<Args>(args)...);
			size_++;
		}
		else {
			cyx::destroy_at(iterator(ptr, entrance_, maxSize));
			cyx::construct_at(iterator(ptr, entrance_, maxSize), std::forward<Args>(args)...);
			entrance_ = (entrance_ + 1) % maxSize;
		}
	}

	void push_back(const T& v) {
		assert(size_ <= maxSize);
		if (size_ < maxSize) {
			cyx::construct_at(iterator(ptr, entrance_ + size_, maxSize), std::move(v));
			size_++;
		}
		else {
			cyx::destroy_at(iterator(ptr, entrance_, maxSize));
			cyx::construct_at(iterator(ptr, entrance_, maxSize), std::move(v));
			entrance_ = (entrance_ + 1) % maxSize;
		}
	}

	void pop_back() {
		assert(size_ > 0);
		cyx::destroy_at(iterator(ptr, entrance_ + size_ - 1, maxSize));
		size_--;
	}


private:
	T* ptr = nullptr;
	size_t entrance_ = 0;
	size_t size_ = 0;
	size_t maxSize;
};


#endif //_ITERATOR_