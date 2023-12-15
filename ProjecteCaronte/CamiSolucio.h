#pragma once
#include "CamiBase.h"

class CamiSolucio : public CamiBase
{
public:
    CamiSolucio() { m_cami = getCamiCoords(); }
    CamiSolucio(const std::vector<Coordinate>& vCor) { m_cami = vCor; }
    std::vector<Coordinate> getCamiCoords();
    CamiSolucio* clone() { return new CamiSolucio(*this); }

private:
    std::vector<Coordinate> m_cami;
};