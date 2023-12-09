#include "pch.h"
#include "GrafSolucio.h"

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

void GrafSolucio::dijkstra(size_t node, std::vector<double>& dist, std::vector<size_t>& anterior)
{
	dist.resize(m_numNodes, DBL_MAX);
	std::vector<bool> visitat;
	visitat.resize(m_numNodes, false);
	anterior.resize(m_numNodes, -1);
	dist[node] = 0;
	anterior[node] = node;
	for (size_t count = 0; count < m_numNodes - 1; count++)
	{
		size_t posVeiAct = minDistance(dist, visitat);

		visitat[posVeiAct] = true;
		for (size_t posVei=0;posVei<m_numNodes;posVei++)
			if (m_matriuAdj[posVeiAct][posVei]!=0)
				if (!visitat[posVei])
					if (dist[posVeiAct] + m_matriuAdj[posVeiAct][posVei] < dist[posVei])
					{
						dist[posVei] = dist[posVeiAct] + m_matriuAdj[posVeiAct][posVei];
						anterior[posVei] = posVeiAct;
					}
	}
}

void GrafSolucio::camiMesCurt(const Coordinate node1, const Coordinate& node2, std::vector<Coordinate>& cami)
{
	std::vector<Coordinate>::iterator itNode1 = std::find(m_nodes.begin(), m_nodes.end(), node1);
	std::vector<Coordinate>::iterator itNode2 = std::find(m_nodes.begin(), m_nodes.end(), node2); 
	if ((itNode1 != m_nodes.end()) && (itNode2 != m_nodes.end()))
	{
		size_t pos1 = std::distance(m_nodes.begin(), itNode1);
		size_t pos2 = std::distance(m_nodes.begin(), itNode2);
		std::vector<size_t> anterior;
		std::vector<double> vDist;
		dijkstra(pos1, vDist, anterior);
		size_t it = pos2;
		cami.push_back(m_nodes[pos2]);
		while (it != pos1)
		{
			cami.push_back(m_nodes[anterior[it]]);
			it = anterior[it];
		}
	}
}

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

	for (int i = 0; i < camins.size(); i++)
	{
		std::vector<Coordinate> temporal=camins[i]->getCamiCoords();
		for (int i = 0; i < temporal.size(); i++)
			if (!jaEstaAlVector(m_nodes,temporal[i]))
				m_nodes.push_back(temporal[i]);
	}

	m_numNodes = m_nodes.size();
	m_matriuAdj.resize(m_numNodes, std::vector<double>(m_numNodes));
	m_numArestes = m_numNodes * (m_numNodes - 1) / 2;

	for (int i = 0; i < m_numNodes; i++)
	{
		for (int j = 0; j < m_numNodes; j++)
		{
			m_matriuAdj[i][j] = Util::DistanciaHaversine(m_nodes[i], m_nodes[j]);
			m_matriuAdj[j][i] = Util::DistanciaHaversine(m_nodes[i], m_nodes[j]);
		}
	}

	//VERSIÓ ANTIGA
	/*m_nodes = nodes;
	m_numNodes = m_nodes.size();
	m_matriuAdj.resize(m_numNodes, std::vector<int>(m_numNodes));
	m_numArestes = m_numNodes * (m_numNodes - 1) / 2;
	for (int i = 0; i < m_numNodes; i++)
	{
		for (int j = 0; j < m_numNodes; j++)
		{
			m_matriuAdj[i][j] = Util::DistanciaHaversine(nodes[i], nodes[j]);
			m_matriuAdj[j][j] = Util::DistanciaHaversine(nodes[i], nodes[j]);
		}
	}*/
}

GrafSolucio::GrafSolucio(const std::vector<Coordinate>& nodes, const std::vector<std::vector<double>>& matriu_adj)
{
	m_nodes = nodes;
	m_matriuAdj = matriu_adj;
	for (int i = 0; i < m_numNodes; i++)
		for (int j = i+1; j < m_numNodes; j++)
			if (m_matriuAdj[i][j] != 0)
				m_numArestes++;
}

GrafSolucio::GrafSolucio(const std::vector<Coordinate>& nodes, const std::vector<std::vector<double>>& parelles_nodes,
	const std::vector<double>& pesos)
{
	m_nodes = nodes;
	m_numNodes = m_nodes.size();
	m_numArestes = parelles_nodes.size();
	crearMatriu(parelles_nodes, pesos);
}

void GrafSolucio::crearMatriu(const std::vector<std::vector<double>>& parelles,const std::vector<double>& pesos)
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
	m_nodes.push_back(node);
	m_matriuAdj.push_back(std::vector<double>(m_numNodes));
	m_numNodes++;
	for (int i = 0; i < m_numNodes; i++) 
		m_matriuAdj[i].push_back(0);
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