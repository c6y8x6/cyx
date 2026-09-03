//to be developed ...

/*

class Entity : public sf::Drawable, public sf::Transformable
{
public:
	Entity(sf::Drawable* shape_ptr, uint32_t coverage, int32_t idNumber, int moveMode = 0)
	: shape_ptr(shape_ptr), coverage(coverage), idNumber(idNumber), moveMode(moveMode) {}

	void basicUpdate(sf::Vector2f position = sf::Vector2f()) {
		existingTime++;
		if (countDown > 0) { countDown--; }
		if (moveMode == 1) {
			velocity += acceleration;
			move(velocity);
			rotate(angularVelocity);
		}
		else if (moveMode == 2) {
			setPosition(position);
		}
	}

	void setMoveMode(int moveMode) {
		assert((0 <= moveMode and moveMode <= 2) && "The moveMode should between 0 and 2. ");
		this->moveMode = moveMode;
	}

	uint32_t getCoverage() const { return coverage; }
	int getMoveMode() const { return moveMode; }
	//getShape_ptr()
	//getTexture_ptr()




	
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		states.transform *= getTransform();
		states.texture = texture_ptr;
		target.draw(*shape_ptr, states);
	}


private:
	uint32_t coverage;
	int moveMode = 0;
public:
	sf::Vector2f velocity{};
	sf::Vector2f acceleration{};
	sf::Angle angularVelocity{};
	int32_t countDown = 0;
	sf::FloatRect collisionBox{ getPosition(), sf::Vector2f() };
private:
	sf::Drawable* shape_ptr;
	sf::Texture* texture_ptr = nullptr;
	sf::Vector2f textureFactor = { 1.f, 1.f };
	sf::Vector2f textureOffset = { 0.f, 0.f };

	uint32_t existingTime = 0;
	int32_t idNumber;
};



class ParticleSystem : public Entity
{
	class Emmiter : public Entity {};

public:
	ParticleSystem(int count, uint32_t coverage, int32_t idNumber, sf::Drawable* shape_ptr = nullptr)
		: particleCount(count), particles(count), Entity(shape_ptr, coverage, idNumber) {}


private:



private:
	int32_t particleCount = 0;
	std::vector<sf::Drawable> particles{};
	std::vector<sf::Drawable> category{};
};

*/