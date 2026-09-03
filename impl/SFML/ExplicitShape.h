extern inline const float defaultExquisity;
extern inline const float default_dx;


#pragma region classes ExplicitShape

class ExplicitShape : public sf::Drawable, public sf::Transformable
{
	using Func = float(*)(float);
	using Vector2C = sf::Vector2<sf::Color>;

private:
	Func get_f1() const { return reinterpret_cast<Func>(func1); }
	Func get_f2() const { return reinterpret_cast<Func>(func2); }

protected:
	virtual void updateVertices(size_t oldSegment = 0) {
		auto [x_min, x_max] = x_domain;
		assert((x_max >= x_min) && "Invalid x_domain");
		size_t segment = static_cast<size_t>((x_max - x_min) * exquisity);
		outlineVertices1.resize(segment);
		outlineVertices2.resize(segment);
		vertices.resize(2 * segment);

		Func func1 = get_f1();
		Func func2 = get_f2();
		float dx = 1.f / exquisity;
		float x = x_domain.x + oldSegment * dx;
		for (size_t index : rrange(oldSegment, segment)) {
			sf::Vector2f point1 = sf::Vector2f(x, -func1(x));
			sf::Vector2f point2 = sf::Vector2f(x, -func2(x));
			outlineVertices1[index].position = point1;
			outlineVertices2[index].position = point2;
			vertices[2 * index].position = point1;
			vertices[2 * index + 1].position = point2;
			x += dx;
		}
	}

	void updateOutlineColor1(size_t oldSegment = 0) {
		for (size_t index : rrange(oldSegment, outlineVertices1.getVertexCount())) {
			outlineVertices1[index].color = outlineColor1;
		}
	}

	void updateOutlineColor2(size_t oldSegment = 0) {
		for (size_t index : rrange(oldSegment, outlineVertices1.getVertexCount())) {
			outlineVertices2[index].color = outlineColor2;
		}
	}

	void updateFillColor(size_t oldSegment = 0) {
		for (size_t index : rrange(oldSegment, outlineVertices1.getVertexCount())) {
			sf::Color color;
			if (vertices[2 * index + 1].position.y < vertices[2 * index].position.y) {
				color = fillColor1;
			}
			else {
				color = fillColor2;
			}
			vertices[2 * index].color = color;
			vertices[2 * index + 1].color = color;
		}
	}

	ExplicitShape() {}

public:
	ExplicitShape(Func func1, Func func2, sf::Vector2f x_domain = { 0.f, 1.f }, float exquisity = defaultExquisity)
		: func1(func1), func2(func2), x_domain(x_domain), exquisity(exquisity)
	{
		outlineVertices1.setPrimitiveType(sf::PrimitiveType::LineStrip);
		outlineVertices2.setPrimitiveType(sf::PrimitiveType::LineStrip);
		vertices.setPrimitiveType(sf::PrimitiveType::TriangleStrip);

		updateVertices();
		updateOutlineColor1();
		updateOutlineColor2();
		updateFillColor();
	}

	ExplicitShape(Func func, sf::Vector2f x_domain = { 0.f, 1.f }, float exquisity = defaultExquisity)
		: ExplicitShape(func, [](float x) { return 0.f; }, x_domain, exquisity) {}


	virtual ~ExplicitShape() = default;

	sf::Vector2f getDomain() const { return x_domain; }
	sf::Color getOutlineColor1() const { return outlineColor1; }
	sf::Color getOutlineColor2() const { return outlineColor2; }
	sf::Color getFillColor1() const { return fillColor1; }
	sf::Color getFillColor2() const { return fillColor2; }
	float getExquisity() const { return exquisity; }

	void setDomain(sf::Vector2f x_domain) {
		this->x_domain = x_domain;
		updateVertices();
		updateOutlineColor1();
		updateOutlineColor2();
		updateFillColor();
	}

	void setOutlineColor1(sf::Color color) {
		this->outlineColor1 = color;
		updateOutlineColor1();
	}

	void setOutlineColor2(sf::Color color) {
		this->outlineColor2 = color;
		updateOutlineColor2();
	}

	void setOutlineColors(Vector2C color) {
		setOutlineColor1(color.x);
		setOutlineColor2(color.y);
	}

	void setFillColors(Vector2C color) {
		this->fillColor1 = color.x;
		this->fillColor2 = color.y;
		updateFillColor();
	}

	void setExquisity(float exquisity) {
		this->exquisity = exquisity;
		updateVertices();
		updateOutlineColor1();
		updateOutlineColor2();
		updateFillColor();
	}

	void expandDomain(float x_max) {
		float x_min = x_domain.x;
		assert((x_max >= x_min) && "Invalid x_domain");
		size_t segment = static_cast<size_t>((x_max - x_min) * exquisity);
		size_t oldSegment = getPointCount();
		if (segment == oldSegment) { return; }
		if (segment < oldSegment) {
			setDomain({ 0.f, x_max });
			return;
		}

		this->x_domain = sf::Vector2f(x_min, x_max);
		updateVertices(oldSegment);
		updateOutlineColor1(oldSegment);
		updateOutlineColor2(oldSegment);
		updateFillColor(oldSegment);
	}

	size_t getPointCount() const { return outlineVertices1.getVertexCount(); }

	sf::Vector2f getLocalPoint1(size_t index) const {
		assert(index < getPointCount() && "Index out of range in ParameterShape::getLocalPoint1");
		return outlineVertices1[index].position;
	}

	sf::Vector2f getLocalPoint2(size_t index) const {
		assert(index < getPointCount() && "Index out of range in ParameterShape::getLocalPoint2");
		return outlineVertices2[index].position;
	}

	sf::Vector2f getGlobalPoint1(size_t index) const { return getTransform() * getLocalPoint1(index); }
	sf::Vector2f getGlobalPoint2(size_t index) const { return getTransform() * getLocalPoint2(index); }

	sf::Vector2f getLocalGeometricCenter() const {
		sf::Vector2f position = sf::Vector2f();
		size_t segment = getPointCount();
		if (segment == 0) { return sf::Vector2f(); }
		for (size_t index : rrange(segment)) {
			position += outlineVertices1[index].position;
			position += outlineVertices2[index].position;
		}
		position /= static_cast<float>(2 * segment);
		return position;
	}

	sf::Vector2f getGlobalGeometricCenter() const { return getTransform() * getLocalGeometricCenter(); }

	sf::FloatRect getLocalBounds() const {
		size_t count = getPointCount();
		if (count == 0) { return sf::FloatRect(); }
		float y_min = getLocalPoint1(0).y;
		float y_max = y_min;
		for (size_t index : rrange(1, count)) {
			float y = getLocalPoint1(index).y;
			if (y_min > y) { y_min = y; }
			else if (y_max < y) { y_max = y; }
		}
		for (size_t index : rrange(count)) {
			float y = getLocalPoint2(index).y;
			if (y_min > y) { y_min = y; }
			else if (y_max < y) { y_max = y; }
		}
		auto [x_min, x_max] = x_domain;
		return sf::FloatRect({ x_min, y_min }, { x_max - x_min, y_max - y_min });
	}

	sf::FloatRect getGlobalBounds() const {
		size_t count = getPointCount();
		if (count == 0) { return sf::FloatRect(); }
		auto [x_min, y_min] = getGlobalPoint1(0);
		auto [x_max, y_max] = getGlobalPoint1(0);
		for (size_t index : rrange(1, count)) {
			auto [x, y] = getGlobalPoint1(index);
			if (x_min > x) { x_min = x; }
			else if (x_max < x) { x_max = x; }
			if (y_min > y) { y_min = y; }
			else if (y_max < y) { y_max = y; }
		}
		for (size_t index : rrange(count)) {
			auto [x, y] = getGlobalPoint2(index);
			if (x_min > x) { x_min = x; }
			else if (x_max < x) { x_max = x; }
			if (y_min > y) { y_min = y; }
			else if (y_max < y) { y_max = y; }
		}
		return sf::FloatRect({ x_min, y_min }, { x_max - x_min, y_max - y_min });
	}

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		states.transform *= getTransform();
		target.draw(vertices, states);
		target.draw(outlineVertices1, states);
		target.draw(outlineVertices2, states);
	}

	sf::VertexArray outlineVertices1{};
	sf::VertexArray outlineVertices2{};
	sf::VertexArray vertices{};
	sf::Vector2f x_domain = { 0.f, 1.f };
	float exquisity = defaultExquisity;

	sf::Color outlineColor1 = sf::Color::White;
	sf::Color outlineColor2 = sf::Color::White;
	sf::Color fillColor1 = sf::Color::Transparent;
	sf::Color fillColor2 = sf::Color::Transparent;
	void* func1 = 0;
	void* func2 = 0;
};






class FirstDifferentialShape : public ExplicitShape
{
	using Func = double(*)(double, double);
	using Vector2d = sf::Vector2<double>;
	using Vector2C = sf::Vector2<sf::Color>;

private:
	Func get_f() const { return reinterpret_cast<Func>(func1); }

	void updateVertices(size_t oldSegment = 0) override {
		auto [x_min, x_max] = x_domain;
		assert((x_max >= x_min) && "Invalid x_domain");
		size_t segment = static_cast<size_t>((x_max - x_min) * exquisity);
		outlineVertices1.resize(segment);
		outlineVertices2.resize(segment);
		vertices.resize(2 * segment);

		Func func = get_f();
		for (size_t index : rrange(oldSegment, segment)) {
			Vector2d point1{};
			Vector2d point2{};
			if (index == 0) {
				point2 = { x_domain.x, -initial_y };
				point1 = { x_domain.x, 0.f };
			}
			else {
				auto [x0, y0] = this->lastPos;
				y0 = -y0;
				double k = func(x0, y0);
				double y = y0 + k * dx;
				double x = x0 + dx;
				point2 = { x, -y };
				point1 = { x, 0.f };
			}
			this->lastPos = point2;
			outlineVertices1[index].position = static_cast<sf::Vector2f>(point1);
			outlineVertices2[index].position = static_cast<sf::Vector2f>(point2);
			vertices[2 * index].position = static_cast<sf::Vector2f>(point1);
			vertices[2 * index + 1].position = static_cast<sf::Vector2f>(point2);
		}
	}

public:

	FirstDifferentialShape(Func func, double initial_y, sf::Vector2f x_domain = { 0.f, 1.f }, float dx = default_dx)
		: initial_y(initial_y), dx(dx)
	{
		this->func1 = func;
		this->func2 = func;
		this->x_domain = x_domain;
		this->exquisity = 1.f / dx;

		outlineVertices1.setPrimitiveType(sf::PrimitiveType::LineStrip);
		outlineVertices2.setPrimitiveType(sf::PrimitiveType::LineStrip);
		vertices.setPrimitiveType(sf::PrimitiveType::TriangleStrip);

		updateVertices();
		updateOutlineColor1();
		updateOutlineColor2();
		updateFillColor();
	}


	double get_dx() const { return dx; }

	sf::Vector2f getLocalPoint(size_t index) const { return getLocalPoint2(index); }
	sf::Vector2f getGlocalPoint(size_t index) const { return getGlobalPoint2(index); }


	void setDomain(sf::Vector2f x_domain, double initial_y) {
		this->x_domain = x_domain;
		this->initial_y = initial_y;
		updateVertices();
		updateOutlineColor1();
		updateOutlineColor2();
		updateFillColor();
	}

private:
	double initial_y;
	double dx;
	Vector2d lastPos = { 0.0, 0.0 };
};





class SecondDifferentialShape : public ExplicitShape
{
	using Func = double(*)(double, double, double);
	using Vector2d = sf::Vector2<double>;
	using Vector2C = sf::Vector2<sf::Color>;

private:
	Func get_f() const { return reinterpret_cast<Func>(func1); }

	void updateVertices(size_t oldSegment = 0) override {
		auto [x_min, x_max] = x_domain;
		assert((x_max >= x_min) && "Invalid x_domain");
		size_t segment = static_cast<size_t>((x_max - x_min) * exquisity);
		outlineVertices1.resize(segment);
		outlineVertices2.resize(segment);
		vertices.resize(2 * segment);

		Func func = get_f();
		for (size_t index : rrange(oldSegment, segment)) {
			Vector2d point1{};
			Vector2d point2{};
			if (index == 0) {
				point2 = { x_domain.x, -initial_y };
				point1 = { x_domain.x, 0.f };
			}
			else {
				auto [x0, y0] = this->lastPos;
				y0 = -y0;
				double a = func(x0, y0, current_k);
				current_k = current_k + a * dx;
				double y = y0 + current_k * dx;
				double x = x0 + dx;
				point2 = { x, -y };
				point1 = { x, 0.f };
			}
			this->lastPos = point2;
			outlineVertices1[index].position = static_cast<sf::Vector2f>(point1);
			outlineVertices2[index].position = static_cast<sf::Vector2f>(point2);
			vertices[2 * index].position = static_cast<sf::Vector2f>(point1);
			vertices[2 * index + 1].position = static_cast<sf::Vector2f>(point2);
		}
	}

public:

	SecondDifferentialShape(Func func, Vector2d initial_yk, sf::Vector2f x_domain = { 0.f, 1.f }, float dx = default_dx)
		: initial_y(initial_yk.x), current_k(initial_yk.y), dx(dx)
	{
		this->func1 = func;
		this->func2 = func;
		this->x_domain = x_domain;
		this->exquisity = 1.f / dx;

		outlineVertices1.setPrimitiveType(sf::PrimitiveType::LineStrip);
		outlineVertices2.setPrimitiveType(sf::PrimitiveType::LineStrip);
		vertices.setPrimitiveType(sf::PrimitiveType::TriangleStrip);

		updateVertices();
		updateOutlineColor1();
		updateOutlineColor2();
		updateFillColor();
	}


	double get_dx() const { return dx; }

	sf::Vector2f getLocalPoint(size_t index) const { return getLocalPoint2(index); }
	sf::Vector2f getGlocalPoint(size_t index) const { return getGlobalPoint2(index); }


	void setDomain(sf::Vector2f x_domain, Vector2d initial_yk) {
		this->x_domain = x_domain;
		this->initial_y = initial_yk.x;
		this->current_k = initial_yk.y;
		updateVertices();
		updateOutlineColor1();
		updateOutlineColor2();
		updateFillColor();
	}

private:
	double initial_y;
	double current_k;
	double dx;
	Vector2d lastPos = { 0.0, 0.0 };
};






#pragma endregion