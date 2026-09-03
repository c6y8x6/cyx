//to be developed

//1. ParticleSystem
//2. SplitTexture
//3. TitleMap



class Particle : public sf::Drawable, public sf::Transformable
{
public:
	

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		states.transform *= getTransform();
		states.texture = nullptr;
		if (!pattern) { return; }
		target.draw(*pattern, states);
	}


private:
	sf::Drawable* pattern;
};





//class ParticleSystem
//{
//public:
//	class Emitter
//	{
//	public:
//
//	
//		sf::Drawable* pattern = nullptr;
//	};
//
//private:
//
//};