#include "MapaSolucio.h"
#include "Util.h" //!!
#include "GrafSolucio.h"
#include "BallTree.h"
void MapaSolucio::reinciaVectors()
{
	while (!m_puntsDeInteres.empty())
	{
		delete m_puntsDeInteres.front();
		m_puntsDeInteres.erase(m_puntsDeInteres.begin());
	}

	while (!m_camins.empty())
	{
		delete m_camins.front();
		m_camins.erase(m_camins.begin());
	}
}

void MapaSolucio::getCamins(std::vector<CamiBase*>& vCamins)
{
	for (std::vector<CamiBase*>::iterator it = m_camins.begin(); it != m_camins.end(); it++)
	{
		vCamins.push_back((*it)->clone());
	}
}

void MapaSolucio::getPdis(std::vector<PuntDeInteresBase*>& v)
{
	for (std::vector<PuntDeInteresBase*>::const_iterator it = m_puntsDeInteres.begin(); it != m_puntsDeInteres.end(); it++)
	{
		v.push_back(*it);
	}
}

void MapaSolucio::parsejaXmlElements(std::vector<XmlElement>& xmlElements)
{
	reinciaVectors();

	for (int i = 0; i < xmlElements.size(); i++)
	{
		if (xmlElements[i].id_element == "node")
		{
			std::string nom = "";
			bool hiHaNom = false;
			std::string tenda = "";
			std::string cuina = "";
			std::string horari = "";
			std::string wheelchair = "";
			bool esCami = false;
			Coordinate coord = { 0, 0 };

			for (int j = 0; j < xmlElements[i].fills.size(); j++)
			{
				if (xmlElements[i].fills[j].first == "tag")
				{
					std::pair<std::string, std::string> valorTag = Util::kvDeTag(xmlElements[i].fills[j].second);

					if (valorTag.first == "name")
					{
						nom = valorTag.second;
						hiHaNom = true;
					}
					else if (valorTag.first == "shop")
						tenda = valorTag.second;
					else if (valorTag.first == "cuisine")
						cuina = valorTag.second;
					else if (valorTag.first == "opening_hours")
						horari = valorTag.second;
					else if (valorTag.first == "wheelchair")
						wheelchair = valorTag.second;
					else if (valorTag.first == "highway" || valorTag.first == "public_transport" || valorTag.first == "access" || valorTag.first == "entrance")
						esCami = true;
				}
			}

			if (hiHaNom && !esCami) // Els nodes que s�n Punt d�Inter�s s�n nodes que no s�n de cam�, i que tenen un tag amb un atribut name
			{
				for (int j = 0; j < xmlElements[i].atributs.size(); j++)
				{
					if (xmlElements[i].atributs[j].first == "lat")
						coord.lat = std::stod(xmlElements[i].atributs[j].second); //converteix de string a double
					else if (xmlElements[i].atributs[j].first == "lon")
						coord.lon = std::stod(xmlElements[i].atributs[j].second); //converteix de string a double
				}

				if (tenda != "") // distingim entre classes derivades
					m_puntsDeInteres.push_back(new PuntDeInteresBotigaSolucio(coord, nom, tenda, horari, wheelchair));
				else if (cuina != "")
					m_puntsDeInteres.push_back(new PuntDeInteresRestaurantSolucio(coord, nom, cuina, wheelchair));
				else
					m_puntsDeInteres.push_back(new PuntDeInteresBase(coord, nom)); //si no es d'una clase derivada, l'instanciem com a base
			}
		}

		if (xmlElements[i].id_element == "way")
		{
			bool highway = false;
			std::vector<std::string> nodes; //aqu� guardem les "ref" dels "nd" dels camins
			std::vector<Coordinate> vCor;

			for (int j = 0; j < xmlElements[i].fills.size(); j++)
			{
				if (xmlElements[i].fills[j].first == "tag")
				{
					if (xmlElements[i].fills[j].second[0].second == "highway") // si no �s highway no el tenim en compta
						highway = true;
				}
				else if (xmlElements[i].fills[j].first == "nd")
					nodes.push_back(xmlElements[i].fills[j].second[0].second);
			}

			// AMB VECTOR NODES PER ORDRE

			if (highway) //si �s highway afegim al vector de camins les coordenades dels nodes que encara hem de trobar
			{
				int j = 0;

				while (j < xmlElements.size() && !nodes.empty()) //busquem els nodes amb "id" igual a la "ref" dels nodes del vector de nodes 
				{
					if (xmlElements[j].id_element == "node")
					{
						Coordinate coord = { 0,0 };
						bool nodeTrobat = false;

						//revisar primer si els fills tenen tag perque llavors no son nodes de cami
						for (int k = 0; k < xmlElements[j].atributs.size(); k++)
						{
							if (xmlElements[j].atributs[k].first == "id")
							{
								if (xmlElements[j].atributs[k].second == nodes[0]) //el "id" coincideix amb la "ref" que tenim del node del vector de nodes
								{
									nodeTrobat = true;
									nodes.erase(nodes.begin()); //elimina el node del vector de nodes per buscar perqu� ja l'hem trobat
								}
							}
							else if (xmlElements[j].atributs[k].first == "lat")
								coord.lat = std::stod(xmlElements[j].atributs[k].second); //converteix de string a double
							else if (xmlElements[j].atributs[k].first == "lon")
								coord.lon = std::stod(xmlElements[j].atributs[k].second); //converteix de string a double
						}

						if (nodeTrobat)
						{
							vCor.push_back(coord);
							j = -1;
						}
					}

					j++;
				}

				m_camins.push_back(new CamiSolucio(vCor));
			}
		}
	}
}

CamiBase* MapaSolucio::buscaCamiMesCurt(PuntDeInteresBase* desde, PuntDeInteresBase* a)
{
	GrafSolucio graf(this);
	BallTree bola;
	bola.construirArbre(graf.getCoordenades());
	Coordinate puntInici, puntFinal;
	bola.nodeMesProper(desde->getCoord(), puntInici, &bola);
	bola.nodeMesProper(a->getCoord(), puntFinal, &bola);

	////Busquem el camí que té els dos punts
	//for (int i = 0; i < m_camins.size(); i++)
	//{
	//	std::vector<Coordinate> coordenadesCami = m_camins[i]->getCamiCoords();
	//	for (int cami = 0; cami < coordenadesCami.size(); cami++)
	//	{
	//		
	//	}
	//}



	//VERSIÓ 1 DIJKSTRA DES DEL GRAF SOLUCIÓ
	std::vector<Coordinate> coordenadesCami;
	std::stack<Coordinate> pilaCoordenadesCami;
	graf.camiMesCurt(puntInici, puntFinal, pilaCoordenadesCami);
	while (!pilaCoordenadesCami.empty())
	{
		coordenadesCami.push_back(pilaCoordenadesCami.top());
		pilaCoordenadesCami.pop();
	}
	CamiBase* cami = new CamiSolucio(coordenadesCami);
	return cami;
}