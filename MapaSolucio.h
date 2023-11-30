#pragma once
#include <iostream>
#include "MapaBase.h"
#include "CamiSolucio.h"
#include "PuntDeInteresBotigaSolucio.h"
#include "PuntDeInteresRestaurantSolucio.h"

class MapaSolucio : public MapaBase
{
public:
    void reinciaVectors();
    void getPdis(std::vector<PuntDeInteresBase*>&);
    void getCamins(std::vector<CamiBase*>&);
    void parsejaXmlElements(std::vector<XmlElement>& xmlElements);
private:
    std::vector<PuntDeInteresBase*> m_puntsDeInteres;
    std::vector<CamiBase*> m_camins;
};
