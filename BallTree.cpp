#include "BallTree.h"

void BallTree::construirArbre(const std::vector<Coordinate>& coordenades) 
{
    // TODO: Utilitza aquest metode per construir el teu BallTree
    // TODO: amb les coordenades que se us passen per parametre

    m_coordenades = coordenades;
    m_pivot = Util::calcularPuntCentral(m_coordenades);
    std::vector<double> distanciesC;
    size_t posPuntMesLlunyC = 0;
    for (size_t i = 0; i < m_coordenades.size(); i++) //CALCULEM LES DIST�NCIES I EL PUNT M�S LLUNY� ALHORA
    {
        distanciesC.push_back(Util::DistanciaHaversine(m_pivot, m_coordenades[i]));
        if (distanciesC[posPuntMesLlunyC] < distanciesC[i])
            posPuntMesLlunyC = i;
    }
    m_radi = distanciesC[posPuntMesLlunyC];

    if (m_coordenades.size() != 1)
    {
        Coordinate puntA;
        puntA.lat=m_coordenades[posPuntMesLlunyC].lat;
        puntA.lon = m_coordenades[posPuntMesLlunyC].lon;
        Coordinate puntB;
        puntB.lat = m_coordenades[0].lat;
        puntB.lon = m_coordenades[0].lon;
        Coordinate aux;
        for (size_t i = 1; i < m_coordenades.size(); i++)
        {
            aux.lat = m_coordenades[i].lat;
            aux.lon = m_coordenades[i].lon;
            if (Util::DistanciaHaversine(puntB, puntA) < Util::DistanciaHaversine(aux, puntA))
            {
                puntB.lat = aux.lat;
                puntB.lon = aux.lon;
            }
        }
        std::vector<Coordinate> coordenadesA, coordenadesB;
        for (size_t i = 0; i < m_coordenades.size(); i++)
        {
            if (Util::DistanciaHaversine(m_coordenades[i], puntA) < Util::DistanciaHaversine(m_coordenades[i], puntB))
                coordenadesA.push_back(m_coordenades[i]);
            else
                coordenadesB.push_back(m_coordenades[i]);
        }
        BallTree* fillDret=new BallTree,* fillEsquerre=new BallTree;
        fillDret->m_root = this;
        fillEsquerre->m_root = this;
        m_left = fillEsquerre;
        m_right = fillDret;
        fillEsquerre->construirArbre(coordenadesA);
        fillDret->construirArbre(coordenadesB);
    }
}

void BallTree::inOrdre(std::vector<std::list<Coordinate>>& out) 
{
    if (m_left != nullptr)
        m_left->inOrdre(out);
    std::list<Coordinate> listCoordenades(m_coordenades.begin(), m_coordenades.end()); //PASSEM EL VECTOR DE COORDENADES A LLISTA DE COORDENADES
    out.push_back(listCoordenades);
    if (m_right != nullptr)
        m_right->inOrdre(out);
}
void BallTree::preOrdre(std::vector<std::list<Coordinate>>& out) 
{
    std::list<Coordinate> listCoordenades(m_coordenades.begin(), m_coordenades.end()); //PASSEM EL VECTOR DE COORDENADES A LLISTA DE COORDENADES
    out.push_back(listCoordenades);
    if (m_left != nullptr)
        m_left->preOrdre(out);
    if (m_right != nullptr)
        m_right->preOrdre(out);
}

void BallTree::postOrdre(std::vector<std::list<Coordinate>>& out) 
{
    if (m_left != nullptr)
        m_left->postOrdre(out);
    if (m_right != nullptr)
        m_right->postOrdre(out);
    std::list<Coordinate> listCoordenades(m_coordenades.begin(), m_coordenades.end()); //PASSEM EL VECTOR DE COORDENADES A LLISTA DE COORDENADES
    out.push_back(listCoordenades);
}

Coordinate BallTree::nodeMesProper(Coordinate targetQuery, Coordinate& Q, BallTree* ball) {
    // TODO: TASCA 3
}



