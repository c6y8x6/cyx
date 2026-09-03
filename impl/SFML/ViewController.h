class ViewController
{
public:
	ViewController(sf::RenderWindow& window)
		: window(window), view(window.getView())
	{}


	void startMoveView() {
		lastMousePos = sf::Vector2f(sf::Mouse::getPosition(window));
	}

	void moveView() {
		auto currentMousePos = sf::Vector2f(sf::Mouse::getPosition(window));
		float accumulatedFactor = view.getSize().x / window.getDefaultView().getSize().x;
		sf::Vector2f delta = (lastMousePos - currentMousePos) * accumulatedFactor;
		sf::Vector2f newCenter = view.getCenter() + delta;

		if (moveLimit.size.x > 0.f) {
			assert(moveLimit.size.y > 0.f);
			sf::Vector2f halfSize = view.getSize() * 0.5f;
			sf::Vector2f minCenter = moveLimit.position + halfSize;
			sf::Vector2f maxCenter = moveLimit.position + moveLimit.size - halfSize;
			newCenter.x = cyx::clamp(newCenter.x, { minCenter.x, maxCenter.x });
			newCenter.y = cyx::clamp(newCenter.y, { minCenter.y, maxCenter.y });
		}

		view.setCenter(newCenter);
		lastMousePos = currentMousePos;
		window.setView(view);
	}

	void endMoveView() const {}

	void zoomView(float zoomAmount, bool isCentral) {
		float zoomFactor = std::pow(zoomFactorRatio, zoomAmount);
		float accumulatedFactor = view.getSize().x / window.getDefaultView().getSize().x;
		accumulatedFactor *= zoomFactor;
		accumulatedFactor = cyx::clamp(accumulatedFactor, zoomLimit);
		if (isCentral) {
			view.setSize(window.getDefaultView().getSize() * accumulatedFactor);
		}
		else {
			sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
			sf::Vector2f worldBefore = window.mapPixelToCoords(mousePixel, view);
			view.setSize(window.getDefaultView().getSize() * accumulatedFactor);
			sf::Vector2f worldAfter = window.mapPixelToCoords(mousePixel, view);
			view.move(worldBefore - worldAfter);
		}

		if (moveLimit.size.x > 0.f) {
			assert(moveLimit.size.y > 0.f);
			sf::Vector2f viewSize = view.getSize();
			float correctionFactor = std::min({ moveLimit.size.x / viewSize.x, moveLimit.size.y / viewSize.y, 1.f });
			correctionFactor *= 0.999f;
			view.zoom(correctionFactor);
			sf::Vector2f newCenter = view.getCenter();
			sf::Vector2f halfSize = view.getSize() * 0.5f;
			sf::Vector2f minCenter = moveLimit.position + halfSize;
			sf::Vector2f maxCenter = moveLimit.position + moveLimit.size - halfSize;
			newCenter.x = cyx::clamp(newCenter.x, { minCenter.x, maxCenter.x });
			newCenter.y = cyx::clamp(newCenter.y, { minCenter.y, maxCenter.y });
			view.setCenter(newCenter);
		}

		window.setView(view);
	}

	void zoomView(const sf::Event::MouseWheelScrolled* mouseWheelScrolled) {
		zoomView(-mouseWheelScrolled->delta, sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::LControl));
	}

	void resetView() {
		view = window.getDefaultView();
		window.setView(view);
	}


	void setMoveLimit(const sf::FloatRect& limit) {
		assert(limit.size.x > 0.f and limit.size.y > 0.f);
		moveLimit = limit;
	}

	void resetMoveLimit() {
		moveLimit.size = { -1.f,-1.f };
	}

	sf::FloatRect getMoveLimit() const {
		return moveLimit;
	}

	void setZoomLimit(sf::Vector2f limit) {
		assert(limit.x <= limit.y);
		zoomLimit = {std::pow(zoomFactorRatio, limit.x), std::pow(zoomFactorRatio, limit.y)};
	}

	void setZoomLimit(float lower) {
		zoomLimit.x = std::pow(zoomFactorRatio, lower);
	}

	void resetZoomLimit() {
		zoomLimit = { 0.f, cyx::inf };
	}

	sf::Vector2f getZoomLimit() const {
		float lower = std::log(zoomLimit.x) / std::log(zoomFactorRatio);
		sf::Vector2f defaultSize = window.getDefaultView().getSize();
		float maxZoomX = moveLimit.size.x / defaultSize.x;
		float maxZoomY = moveLimit.size.y / defaultSize.y;
		float maxZoom = std::min(maxZoomX, maxZoomY);
		float upper = std::log(std::min(maxZoom, zoomLimit.y)) / std::log(zoomFactorRatio);
		return { lower, upper };
	}


private:
	sf::RenderWindow& window;
	sf::View view;
	sf::Vector2f lastMousePos{};

	sf::FloatRect moveLimit{ {0.f, 0.f}, {-1.f,-1.f} };
	sf::Vector2f zoomLimit{ 0.f, cyx::inf };

	static constexpr float zoomFactorRatio = 1.2f;
};