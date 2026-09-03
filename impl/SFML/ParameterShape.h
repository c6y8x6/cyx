extern inline const float defaultExquisity;
extern inline const float default_dx;
constexpr float thicknessConversionConstant = 0.01f;



sf::Vector2<sf::Vector2f> calculateSuitableConversion(sf::Vector2f targetSize, const sf::FloatRect& originRect) {
	sf::Vector2f targetCenter = targetSize * 0.5f;
	auto dealNeg = [](float a) {return a == -0.f ? a : -a; };
	float factor = std::max({ targetCenter.x / (dealNeg(originRect.position.x)),
					   targetCenter.y / (dealNeg(originRect.position.y)),
					   targetCenter.x / (originRect.size.x + originRect.position.x),
					   targetCenter.y / (originRect.size.y + originRect.position.y) });
	return { {factor, factor}, targetCenter };
}



#pragma region classes ParameterShape

enum class OutlineType {
	None, Single, Normal, Serration, Envelop
};


class ParameterShape : public sf::Drawable, public sf::Transformable
{
	using Func = float(*)(float);

protected:

	virtual sf::Vector2f calculateLocalPoint(float t) const {
		return sf::Vector2f(func1(t), -func2(t));
	}

	void updateVertices() {
		auto [t_min, t_max] = t_domain;
		assert((t_max >= t_min) && "Invalid t_domain");
		size_t segment = static_cast<size_t>((t_max - t_min) * exquisity);
		float dt = 1.f / exquisity;
		vertices.resize(segment + 2);
		vertices[0].position = sf::Vector2f();

		float t = t_min;
		for (size_t index : rrange(1, segment + 1)) {
			vertices[index].position = calculateLocalPoint(t);
			t += dt;
		}
		//vertices[segment + 1].position = vertices[1].position;
		vertices[segment + 1].position = vertices[segment].position;//unclosed
	}

	void outlineStrategy(size_t index) {
		sf::Vector2f position = vertices[index + 1].position;
		sf::Vector2f nextPosition = vertices[index + 2].position;
		outlineVertices[2 * index].position = position;

		position = nextPosition - position;
		if (olt == OutlineType::Normal or olt == OutlineType::Serration) {
			position = position.perpendicular();
		}
		outlineVertices[2 * index + 1].position = outlineVertices[2 * index].position;
		if (position != sf::Vector2f()) {
			position = position.normalized();
			position *= thickness;
			outlineVertices[2 * index + 1].position += position;
		}
	}

	void updateOutlineVertices(size_t oldSegment = 0) {

		if (olt == OutlineType::None) { return; }
		size_t verticesCount = vertices.getVertexCount() - 1;
		if (olt == OutlineType::Single) {
			outlineVertices.setPrimitiveType(sf::PrimitiveType::LineStrip);
			outlineVertices.resize(verticesCount);
			for (size_t index : rrange(oldSegment, verticesCount)) {
				outlineVertices[index].position = vertices[index + 1].position;
			}
		}
		else {
			if (olt == OutlineType::Normal) { outlineVertices.setPrimitiveType(sf::PrimitiveType::TriangleStrip); }
			else { outlineVertices.setPrimitiveType(sf::PrimitiveType::LineStrip); }
			if (verticesCount < 4) { return; }
			outlineVertices.resize(verticesCount * 2);
			for (size_t index : rrange(oldSegment, verticesCount - 1)) {
				outlineStrategy(index);
			}
			//outlineVertices[2 * verticesCount - 2].position = outlineVertices[0].position;
			//outlineVertices[2 * verticesCount - 1].position = outlineVertices[1].position;
			outlineVertices[2 * verticesCount - 2].position = outlineVertices[2 * verticesCount - 4].position;
			outlineVertices[2 * verticesCount - 1].position = outlineVertices[2 * verticesCount - 3].position;
			//unclosed
		}
	}

	void updateOutline() {
		updateVertices();
		updateOutlineVertices();
	}

	void updateFillColor() {
		for (size_t index : rrange(vertices.getVertexCount())) {
			vertices[index].color = getFillColor();
		}
	}

	void updateOutlineColor(size_t oldSegment = 0) {
		for (size_t index : rrange(oldSegment, outlineVertices.getVertexCount())) {
			outlineVertices[index].color = getOutlineColor();
		}
	}

	ParameterShape() {}

public:

	ParameterShape(Func func1, Func func2, sf::Vector2f t_domain = { 0.f, 1.f },
				   OutlineType olt = OutlineType::None, float thickness = 0.f, float exquisity = defaultExquisity)
		: func1(func1), func2(func2), t_domain(t_domain), exquisity(exquisity), olt(olt), 
		thickness(thickness * thicknessConversionConstant)
	{
		vertices.setPrimitiveType(sf::PrimitiveType::TriangleFan);
		updateOutline();
		updateFillColor();
		updateOutlineColor();
	}

	virtual ~ParameterShape() = default;

	sf::Vector2f getDomain() const { return t_domain; }
	sf::Color getFillColor() const { return fillColor; }
	float getExquisity() const { return exquisity; }
	float getExquisityInv() const { return 1.f / exquisity; }
	OutlineType getOutlineType() const { return olt; }
	float getOutlineThickness() const { return thickness / thicknessConversionConstant; }
	sf::Color getOutlineColor() const { return outlineColor; }

	void makeClose() {
		size_t verticesCount = vertices.getVertexCount() - 1;
		vertices[verticesCount] = vertices[1];
		if (olt == OutlineType::None or olt == OutlineType::Single or verticesCount < 4) { return; }
		outlineVertices[2 * verticesCount - 2] = outlineVertices[0];
		outlineVertices[2 * verticesCount - 1] = outlineVertices[1];
		outlineStrategy(verticesCount - 2);
	}

	void setDomain(sf::Vector2f t_domain) {
		this->t_domain = t_domain;
		updateOutline();
		updateFillColor();
		updateOutlineColor();
	}

	void setFillColor(sf::Color color) {
		this->fillColor = color;
		updateFillColor();
	}

	void setExquisity(float exquisity) {
		this->exquisity = exquisity;
		updateOutline();
		updateFillColor();
		updateOutlineColor();
	}

	void setOutline(float thickness, sf::Color color) {
		this->thickness = thickness * thicknessConversionConstant;
		this->outlineColor = color;
		updateOutlineVertices();
		updateOutlineColor();
	}

	void setOutlineThickness(float thickness) {
		this->thickness = thickness * thicknessConversionConstant;
		updateOutlineVertices();
		updateOutlineColor();
	}

	void setOutlineColor(sf::Color color) {
		this->outlineColor = color;
		updateOutlineColor();
	}


	void expandDomain(float t_max) {
		float t_min = t_domain.x;
		assert((t_max >= t_min) && "Invalid t_domain");
		size_t segment = static_cast<size_t>((t_max - t_min) * exquisity);
		size_t oldSegment = getPointCount();
		if (segment == oldSegment) { return; }
		if (segment < oldSegment) {
			setDomain({ 0.f, t_max });
			return;
		}

		vertices.resize(segment + 2);
		float dt = 1.f / exquisity;
		float t = t_min + oldSegment * dt;
		for (size_t index : rrange(oldSegment + 1, segment + 1)) {
			vertices[index].position = calculateLocalPoint(t);
			vertices[index].color = getFillColor();
			t += dt;
		}
		vertices[segment + 1].position = vertices[segment].position;//unclosed
		vertices[segment + 1].color = getFillColor();

		oldSegment -= 2;
		if (static_cast<int64_t>(oldSegment) < 0) { oldSegment = 0; }
		updateOutlineVertices(oldSegment);
		updateOutlineColor(oldSegment);

		if (texture_ptr != nullptr) {
			for (size_t index : rrange(oldSegment, vertices.getVertexCount())) {
				vertices[index].texCoords = vertices[index].position.componentWiseMul(textureFactor) + textureOffset;
			}
		}
	}


	size_t getPointCount() const {
		return vertices.getVertexCount() - 2;
	}
	//static_cast<size_t>((t_max - t_min) * exquisity)

	sf::Vector2f getLocalPoint(size_t index) const {
		assert(index < getPointCount() && "Index out of range in ParameterShape::getLocalPoint");
		return vertices[index + 1].position;
	}

	sf::Vector2f getGlobalPoint(size_t index) const {
		return getTransform() * getLocalPoint(index);
	}

	sf::Vector2f getLocalGeometricCenter() const {
		size_t count = getPointCount();
		if (count == 0) { return sf::Vector2f(); }
		sf::Vector2f center(0.f, 0.f);
		for (size_t index : rrange(count)) {
			center += getLocalPoint(index);
		}
		center /= static_cast<float>(count);
		return center;
	}

	sf::Vector2f getGlobalGeometricCenter() const {
		return getTransform() * getLocalGeometricCenter();
	}

	sf::FloatRect getLocalBounds() const {
		size_t count = getPointCount();
		if (count == 0) { return sf::FloatRect(); }
		auto [x_min, y_min] = getLocalPoint(0);
		auto [x_max, y_max] = getLocalPoint(0);
		for (size_t index : rrange(1, count)) {
			auto [x, y] = getLocalPoint(index);
			if (x_min > x) { x_min = x; }
			else if (x_max < x) { x_max = x; }
			if (y_min > y) { y_min = y; }
			else if (y_max < y) { y_max = y; }
		}
		return sf::FloatRect({ x_min, y_min }, { x_max - x_min, y_max - y_min });
	}

	sf::FloatRect getGlobalBounds() const {
		size_t count = getPointCount();
		if (count == 0) { return sf::FloatRect(); }
		auto [x_min, y_min] = getGlobalPoint(0);
		auto [x_max, y_max] = getGlobalPoint(0);
		for (size_t index : rrange(1, count)) {
			auto [x, y] = getGlobalPoint(index);
			if (x_min > x) { x_min = x; }
			else if (x_max < x) { x_max = x; }
			if (y_min > y) { y_min = y; }
			else if (y_max < y) { y_max = y; }
		}
		return sf::FloatRect({ x_min, y_min }, { x_max - x_min, y_max - y_min });
	}


	void setTexture(sf::Texture* texture_ptr = nullptr) {
		this->texture_ptr = texture_ptr;
		if (texture_ptr == nullptr) { return; }
		auto [factor, offset] = calculateSuitableConversion(
			static_cast<sf::Vector2f>(texture_ptr->getSize()), this->getLocalBounds()
		);
		this->textureFactor = factor;
		this->textureOffset = offset;
		for (size_t index : rrange(vertices.getVertexCount())) {
			vertices[index].texCoords = vertices[index].position.componentWiseMul(textureFactor) + textureOffset;
		}
	}

	void setTexture(sf::Texture* texture_ptr, sf::Vector2f factor, sf::Vector2f offset) {
		this->texture_ptr = texture_ptr;
		if (texture_ptr == nullptr) { return; }
		this->textureFactor = factor;
		this->textureOffset = offset;
		for (size_t index : rrange(vertices.getVertexCount())) {
			vertices[index].texCoords = vertices[index].position.componentWiseMul(textureFactor) + textureOffset;
		}
	}

	void setTexture(sf::Texture* texture_ptr, float factor, sf::Vector2f offset) {
		setTexture(texture_ptr, { factor, factor }, offset);
	}


protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		states.transform *= getTransform();
		states.texture = texture_ptr;
		target.draw(vertices, states);
		states.texture = nullptr;
		if (olt != OutlineType::None) {
			target.draw(outlineVertices, states);
		}
	}

	sf::VertexArray vertices{};
	sf::VertexArray outlineVertices{};

	Func func1 = reinterpret_cast<Func>(0);
	Func func2 = reinterpret_cast<Func>(0);
	sf::Vector2f t_domain = { 0.f, 1.f };
	float exquisity = defaultExquisity;
	OutlineType olt = OutlineType::None;
	float thickness = 0.f;
	sf::Vector2f textureFactor = { 1.f, 1.f };
	sf::Vector2f textureOffset = { 0.f, 0.f };

	sf::Color fillColor = sf::Color::White;
	sf::Color outlineColor = sf::Color::Transparent;
	sf::Texture* texture_ptr = nullptr;
};






class PolarShape : public ParameterShape
{
	using Func = float(*)(float);

public:

	PolarShape(Func funcr, sf::Vector2f theta_domain = { 0.f, 2.f * cyx::pi},
			   OutlineType olt = OutlineType::None, float thickness = 0.f, float exquisity = defaultExquisity)
	{
		this->func1 = funcr;
		this->func2 = funcr;
		this->t_domain = theta_domain;
		this->exquisity = exquisity;
		this->olt = olt;
		this->thickness = thickness;
		vertices.setPrimitiveType(sf::PrimitiveType::TriangleFan);
		updateOutline();
		updateFillColor();
		updateOutlineColor();
	}

	sf::Vector2f calculateLocalPoint(float theta) const override {
		return sf::Vector2f(func1(theta) * cos(theta), -func1(theta) * sin(theta));
	}

private:
};



class ArcLine : public ParameterShape
{
	using Func = float(*)(float);

public:

	ArcLine(Func funcs, float alpha_max, OutlineType olt = OutlineType::Single, float thickness = 1.f, float d_alpha = default_dx)
	{
		this->func1 = funcs;
		this->func2 = funcs;
		this->t_domain = { 0.f, alpha_max };
		this->exquisity = 1.f / d_alpha;
		this->olt = olt;
		this->thickness = thickness;

		this->fillColor = sf::Color::Transparent;
		this->outlineColor = sf::Color::White;
		vertices.setPrimitiveType(sf::PrimitiveType::TriangleFan);
		updateOutline();
		updateFillColor();
		updateOutlineColor();
	}

	sf::Vector2f calculateLocalPoint(float alpha) const override {
		float d_alpha = 1.f / exquisity;
		float ds = func1(alpha + d_alpha) - func1(alpha);
		float dx = ds * cos(alpha);
		float dy = ds * sin(alpha);
		sf::Vector2f Pos = sf::Vector2f(lastPos.x + dx, lastPos.y - dy);
		lastPos = Pos;
		return Pos;
	}

private:
	mutable sf::Vector2f lastPos{ 0.f, 0.f };
};


#pragma endregion