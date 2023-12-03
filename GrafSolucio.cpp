#include "pch.h"
#include "GrafSolucio.h"


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
	m_matriuAdj.resize(m_numNodes, std::vector<int>(m_numNodes));
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

GrafSolucio::GrafSolucio(const std::vector<Coordinate>& nodes, const std::vector<std::vector<int>>& matriu_adj)
{
	m_nodes = nodes;
	m_matriuAdj = matriu_adj;
	for (int i = 0; i < m_numNodes; i++)
		for (int j = i+1; j < m_numNodes; j++)
			if (m_matriuAdj[i][j] != 0)
				m_numArestes++;
}

GrafSolucio::GrafSolucio(const std::vector<Coordinate>& nodes, const std::vector<std::vector<int>>& parelles_nodes,
	const std::vector<int>& pesos)
{
	m_nodes = nodes;
	m_numNodes = m_nodes.size();
	m_numArestes = parelles_nodes.size();
	crearMatriu(parelles_nodes, pesos);
}

void GrafSolucio::crearMatriu(const std::vector<std::vector<int>>& parelles,const std::vector<int>& pesos)
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

void GrafSolucio::inserirAresta(int posNode1, int posNode2, int pes)
{
	m_matriuAdj[posNode1][posNode2] = pes;
	m_matriuAdj[posNode2][posNode1] = pes;
}

void GrafSolucio::afegirNode(const Coordinate& node)
{
	m_nodes.push_back(node);
	m_matriuAdj.push_back(std::vector<int>(m_numNodes));
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