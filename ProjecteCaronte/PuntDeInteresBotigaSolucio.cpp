#include "PuntDeInteresBotigaSolucio.h"

unsigned int PuntDeInteresBotigaSolucio::getColor()
{
	if (m_shop == "supermarket")
		return 0xA5BE00;
	if (m_shop == "tobacco")
		return 0xFFAD69;
	if (m_shop == "bakery")
	{
		if ((m_opening_hours.find("06:00-22:00") != std::string::npos) && (m_weelchair == "yes"))
			return 0x4CB944;
		return 0xE85D75;
	}
	if (m_shop != "")
		return 0xEFD6AC;
	return PuntDeInteresBase::getColor();
}