#ifndef _CYX_SFML_
#define _CYX_SFML_
#include <span>
#include <cyx.h>



//<SFML/System.hpp>
#ifdef _SFML_SYSTEM

//<iostream>
#ifdef _IOSTREAM_

template<typename T>
std::ostream& operator<< (std::ostream& os, const sf::Vector2<T>& vec) {
	os << "(" << vec.x << ", " << vec.y << ")";
	return os;
}

template<typename T>
std::ostream& operator<< (std::ostream& os, const sf::Vector3<T>& vec) {
	os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return os;
}

#endif //_IOSTREAM_


template<typename T>
constexpr sf::Vector2<T>& operator*= (sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs) {
	lhs.x *= rhs.x;
	lhs.y *= rhs.y;
	return lhs;
} //component-wise multiplication and assignment, not dot product

template<typename T>
constexpr sf::Vector3<T>& operator*= (sf::Vector3<T>& lhs, const sf::Vector3<T>& rhs) {
	lhs.x *= rhs.x;
	lhs.y *= rhs.y;
	lhs.z *= rhs.z;
	return lhs;
} //component-wise multiplication and assignment, not dot product

#endif //_SFML_SYSTEM



//<SFML/Graphics.hpp>
#ifdef _SFML_GRAPHICS

//<iostream>
#ifdef _IOSTREAM_

std::ostream& operator<< (std::ostream& os, const sf::Transform& transform) {
	const float* matrix = transform.getMatrix();
	std::cout << "[" << matrix[0] << ", " << matrix[1] << ", " << matrix[2] << "; "
		<< matrix[4] << ", " << matrix[5] << ", " << matrix[6] << "; "
		<< matrix[8] << ", " << matrix[9] << ", " << matrix[10] << "]";
	return os;
}

template<typename T>
std::ostream& operator<< (std::ostream& os, const sf::Rect<T>& rect) {
	std::cout << "{" << rect.position << ", " << rect.size << "}";
	return os;
}

std::ostream& operator<< (std::ostream& os, const sf::Color& color) {
	std::cout << "0x" << std::hex << color.toInteger() << std::dec;
	return os;
}

#endif //_IOSTREAM_

#endif //_SFML_GRAPHICS





namespace cyx {

	//<SFML/System.hpp>
#ifdef _SFML_SYSTEM
	template<typename T>
	bool isInRect(const sf::Vector2<T>& point, const sf::Vector2<T>& interval) {
		return static_cast<T>(0) <= point.x and point.x < interval.x and 
			static_cast<T>(0) <= point.y and point.y < interval.y;
	}

	template<typename T>
	bool isInRect(const sf::Vector2<T>& point, const sf::Vector2<sf::Vector2<T>>& rect) {
		return rect.x.x <= point.x and point.x < rect.y.x and
			rect.x.y <= point.y and point.y < rect.y.y;
	}

	template<typename T>
	bool isInCircle(const sf::Vector2<T>& point, T radius) {
		if (not isInRect(point, { {-radius, -radius}, {radius, radius} })) {
			return false;
		}
		return (point.x * point.x + point.y * point.y < radius * radius);
	}

	template<typename T>
	const T& clamp(const T& value, const sf::Vector2<T>& bounds) {
		const auto& [lowerBound, upperBound] = bounds;
		assert(lowerBound <= upperBound);
		if (value <= lowerBound) { return lowerBound; }
		if (value >= upperBound) { return upperBound; }
		return value;
	}
#endif //_SFML_SYSTEM



	//<SFML/Window.hpp>
#ifdef _SFML_WINDOW
	bool isMouseIn(const sf::Window& window) {
		auto mousePos = sf::Mouse::getPosition(window);
		auto windowSize = static_cast<sf::Vector2i>(window.getSize());
		return isInRect(mousePos, windowSize);
	}
#endif //_SFML_WINDOW



	//<SFML/Graphics.hpp>
#ifdef _SFML_GRAPHICS

	inline constexpr float defaultExquisity = 100.f;
	inline constexpr float default_dx = 1e-2f;

#include <impl/SFML/ParameterShape.h>
#include <impl/SFML/ExplicitShape.h>
#include <impl/SFML/ViewController.h>
	//#include <impl/SFML/Entity.h>

#endif //_SFML_GRAPHICS

} //namespace cyx


#endif //_CYX_SFML_