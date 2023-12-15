#include "pch.h"
#include "GrafSolucio.h"
#include <algorithm>

bool jaEstaAlVector(const std::vector<Coordinate>& nodes, const Coordinate& c)
{
	int i = 0;
	while (i != nodes.size())
	{
		if (nodes[i].lat == c.lat && nodes[i].lon == c.lon)
			return true;
		else
			i++;
	}
	return false;
}


GrafSolucio::GrafSolucio(MapaBase* mapa)
{
	std::vector<CamiBase*> camins;
	mapa->getCamins(camins);
	m_numNodes=0;
	m_numArestes=0;

	for (int i = 0; i < camins.size(); i++)
	{
		std::vector<Coordinate> temporal = camins[i]->getCamiCoords();

		for (int i = 0; i < temporal.size(); i++)
		{
			afegirNode(temporal[i]);
			if (i > 0)
			{
				size_t pos1 = 0, pos2=0;
				while (pos1 <= m_nodes.size())
				{
					if (m_nodes[pos1] == temporal[i])
						break;
					else
						pos1++;
				}
				while (pos2 <= m_nodes.size())
				{
					if (m_nodes[pos2] == temporal[i - 1])
						break;
					else
						pos2++;
				}
				inserirAresta(pos1, pos2, Util::DistanciaHaversine(temporal[i], temporal[i - 1]));
			}
		}
	}
}

void GrafSolucio::crearMatriu(const std::vector<std::vector<double>>& parelles, const std::vector<double>& pesos)
{
	m_matriuAdj.resize(m_numNodes);
	for (size_t i = 0; i < m_numNodes; i++)
	{
		m_matriuAdj[i].resize(m_numNodes, 0);
	}
	for (size_t i = 0; i < parelles.size(); i++)
	{
		m_matriuAdj[parelles[i][0]][parelles[i][1]] = pesos[i];
		m_matriuAdj[parelles[i][1]][parelles[i][0]] = pesos[i];
	}
}

void GrafSolucio::inserirAresta(int posNode1, int posNode2, double pes)
{
	m_matriuAdj[posNode1][posNode2] = pes;
	m_matriuAdj[posNode2][posNode1] = pes;
}

void GrafSolucio::afegirNode(const Coordinate& node)
{
	if (!jaEstaAlVector(m_nodes,node))
	{
		m_nodes.push_back(node);
		m_matriuAdj.push_back(std::vector<double>(m_numNodes));
		m_numNodes++;
		for (int i = 0; i < m_numNodes; i++)
			m_matriuAdj[i].push_back(0);
	}
}

void GrafSolucio::mostra()
{
	for (int i = 0; i < m_numNodes; i++)
	{
		for (int j = 0; j < m_numNodes; j++)
		{
			std::cout << m_matriuAdj[i][j] << "   ";
		}
		std::cout << std::endl;
	}
}

size_t GrafSolucio::minDistance(std::vector<double>& dist, std::vector<bool>& visitat)
{
	// Initialize min value
	double min = DBL_MAX;
	size_t minIndex = -1;
	for (size_t posVei = 0; posVei < m_numNodes; posVei++)
		if (!visitat[posVei] && dist[posVei] <= min)
		{
			min = dist[posVei];
			minIndex = posVei;
		}
	return minIndex;
}

void GrafSolucio::dijkstraModif(size_t node, size_t node2, std::vector<double>& dist, std::vector<size_t>& anterior)
{
	dist.resize(m_numNodes, DBL_MAX);
	std::vector<bool> visitat;
	visitat.resize(m_numNodes, false);
	anterior.resize(m_numNodes, -1);
	dist[node] = 0;
	anterior[node] = node;
	for (size_t count = 0; count < m_numNodes - 1; count++) {
		size_t posVeiAct = minDistance(dist, visitat);
		visitat[posVeiAct] = true;
		if (posVeiAct == node2) return;
		for (size_t posVei = 0; posVei < m_numNodes; posVei++)
			if (m_matriuAdj[posVeiAct][posVei] != 0)
				if (!visitat[posVei])
					if (dist[posVeiAct] + m_matriuAdj[posVeiAct][posVei] < dist[posVei]) {
						dist[posVei] = dist[posVeiAct] + m_matriuAdj[posVeiAct][posVei]; anterior[posVei] = posVeiAct;
					}
	}
}

void GrafSolucio::camiMesCurt(const Coordinate& node1, const Coordinate& node2, std::stack<Coordinate>& cami) 
{
	std::vector<Coordinate>::iterator itNode1 = std::find(m_nodes.begin(), m_nodes.end(), node1); 
	std::vector<Coordinate>::iterator itNode2 = std::find(m_nodes.begin(), m_nodes.end(), node2); 
	if ((itNode1 != m_nodes.end()) && (itNode2 != m_nodes.end()))
	{
		size_t pos1 = distance(m_nodes.begin(), itNode1);
		size_t pos2 = distance(m_nodes.begin(), itNode2);
		std::vector<size_t> anterior;
		std::vector<double> vDist;
		dijkstraModif(pos1, pos2, vDist, anterior);
		size_t it = pos2;
		cami.push(m_nodes[pos2]);
		while (it != pos1)
		{
			cami.push(m_nodes[anterior[it]]);
			it = anterior[it];
		}
	}
}
