#include "BallTree.h"
#include <limits> //CUIDAO!!!!
#include <stack>

BallTree::~BallTree()
{
    if (m_right != nullptr)
    {
        delete m_right;
        m_right = nullptr;
    }

    if (m_left != nullptr)
    {
        delete m_left;
        m_left = nullptr;
    }

    m_root = nullptr;
}


//BallTree::~BallTree() 
//{
//    ;
//}

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

    if (m_coordenades.size() > 1)
    {
        Coordinate puntA;
        puntA.lat = m_coordenades[posPuntMesLlunyC].lat;
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
        std::vector<Coordinate> coordenadesA, coordenadesB; //ASSIGNEM CADA PUNT A UN BALLTREE FILL
        for (size_t i = 0; i < m_coordenades.size(); i++) //SI EL PUNT EST� M�S A PROP DE 'A' QUE DE 'B', L'ASSIGNEM AL FILL ESQUERRE I AL REV�S
        {
            if (Util::DistanciaHaversine(m_coordenades[i], puntA) < Util::DistanciaHaversine(m_coordenades[i], puntB))
                coordenadesA.push_back(m_coordenades[i]);
            else
                coordenadesB.push_back(m_coordenades[i]);
        }
        BallTree* fillDret = new BallTree, * fillEsquerre = new BallTree;
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



Coordinate BallTree::nodeMesProper(Coordinate targetQuery, Coordinate& Q, BallTree* ball)
{
    if (ball->getArrel() == nullptr)
    {
        Q.lat = 0;
        Q.lon = 0;
    }
    double D1 = Util::DistanciaHaversine(targetQuery, ball->m_pivot);
    double D2 = Util::DistanciaHaversine(Q, ball->m_pivot);
    if ((D1 - ball->m_radi) >= D2)
        return Q;
    if (ball->m_left == nullptr && ball->m_right == nullptr)
    {
        for (auto it = m_coordenades.begin(); it != m_coordenades.end(); it++)
        {
            if (Util::DistanciaHaversine(targetQuery, *it) < Util::DistanciaHaversine(targetQuery, Q))
            {
                Q.lat = (*it).lat;
                Q.lon = (*it).lon;
            }
        }
        return Q;
    }
    else
    {
        double Da = DBL_MAX; //AIX� �S PER ESTAR SEGUR QUE ALG�N DELS DOS FILLS NO SIGUI NULLPTR
        double Db = DBL_MAX; //AIX� �S PER ESTAR SEGUR QUE ALG�N DELS DOS FILLS NO SIGUI NULLPTR
        if (ball->m_left != nullptr) //AIX� �S PER ESTAR SEGUR QUE ALG�N DELS DOS FILLS NO SIGUI NULLPTR
            Da = Util::DistanciaHaversine(targetQuery, ball->m_left->m_pivot);
        if (ball->m_right != nullptr) //AIX� �S PER ESTAR SEGUR QUE ALG�N DELS DOS FILLS NO SIGUI NULLPTR
            Db = Util::DistanciaHaversine(targetQuery, ball->m_right->m_pivot);

        if (Da < Db)
        {
            if (ball->m_left != nullptr) //AIX� �S PER ESTAR SEGUR QUE ALG�N DELS DOS FILLS NO SIGUI NULLPTR
                nodeMesProper(targetQuery, Q, ball->m_left);
            if (ball->m_right != nullptr) //AIX� �S PER ESTAR SEGUR QUE ALG�N DELS DOS FILLS NO SIGUI NULLPTR
                nodeMesProper(targetQuery, Q, ball->m_right);
        }
        else
        {
            if (ball->m_right != nullptr) //AIX� �S PER ESTAR SEGUR QUE ALG�N DELS DOS FILLS NO SIGUI NULLPTR
                nodeMesProper(targetQuery, Q, ball->m_right);
            if (ball->m_left != nullptr) //AIX� �S PER ESTAR SEGUR QUE ALG�N DELS DOS FILLS NO SIGUI NULLPTR
                nodeMesProper(targetQuery, Q, ball->m_left);
        }
    }
    return Q;
}