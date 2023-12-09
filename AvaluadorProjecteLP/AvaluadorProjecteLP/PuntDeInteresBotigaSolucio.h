#pragma once
#include "PuntDeInteresBase.h"

class PuntDeInteresBotigaSolucio : public PuntDeInteresBase {

public:
	PuntDeInteresBotigaSolucio() : m_shop(""), m_opening_hours(""), m_weelchair("") {}
	PuntDeInteresBotigaSolucio(Coordinate coord, const std::string& name, const std::string& shop,
		const std::string& open, const std::string& weelchair) : PuntDeInteresBase(coord, name),
		m_shop(shop), m_opening_hours(open), m_weelchair(weelchair) {}
	unsigned int getColor();
	std::string getName() { return PuntDeInteresBase::getName(); }

	PuntDeInteresBotigaSolucio* clone() { return new PuntDeInteresBotigaSolucio(*this); }

private:
	std::string m_shop;
	std::string m_opening_hours;
	std::string m_weelchair;
};