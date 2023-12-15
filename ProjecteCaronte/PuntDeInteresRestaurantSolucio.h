#pragma once
#include "PuntDeInteresBase.h"

class PuntDeInteresRestaurantSolucio : public PuntDeInteresBase {

public:
	PuntDeInteresRestaurantSolucio() : m_cuisine(""), m_weelchair("") {}
	PuntDeInteresRestaurantSolucio(Coordinate coord, const std::string& name, const std::string& cuisine,
		std::string weelchair) : PuntDeInteresBase(coord, name), m_cuisine(cuisine), m_weelchair(weelchair) {}
	unsigned int getColor();
	std::string getName() { return PuntDeInteresBase::getName(); }

	PuntDeInteresRestaurantSolucio* clone() { return new PuntDeInteresRestaurantSolucio(*this); }

private:
	std::string m_cuisine;
	std::string m_weelchair;
};