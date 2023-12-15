#include "CamiSolucio.h"


std::vector<Coordinate> CamiSolucio::getCamiCoords()
{
	//VERSI� ANTERIOR DE getCaminCoords
	/*
	Coordinate cor1, cor2, cor3, cor4;
	cor1.lat = 41.4928803;
	cor1.lon = 2.1452381;
	cor2.lat = 41.4929072;
	cor2.lon = 2.1452474;
	cor3.lat = 41.4933070;
	cor3.lon = 2.1453852;
	cor4.lat = 41.4939882;
	cor4.lon = 2.1456419;
	std::vector<Coordinate> vCor;
	vCor.push_back(cor1);
	vCor.push_back(cor2);
	vCor.push_back(cor3);
	vCor.push_back(cor4);
	return vCor;
	*/

	std::vector<Coordinate> vCor;

	for (int i = 0; i < m_cami.size(); i++)
		vCor.push_back(m_cami[i]);

	return vCor;
}