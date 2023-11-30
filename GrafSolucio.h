#pragma once
#include "Util.h"
#include "MapaSolucio.h"


class GrafSolucio
{
public:
	GrafSolucio() { m_numNodes = 0; m_numArestes = 0; }
	GrafSolucio(const std::vector<CamiBase*>& camins); //AQUEST CONSTRUCTOR ÉS EL MÉS IMPORTANT
	GrafSolucio(const std::vector<Coordinate>& nodes, const std::vector<std::vector<int>>& matriu_adj);
	GrafSolucio(const std::vector<Coordinate>& nodes, const std::vector<std::vector<int>>& parelles_nodes, 
		const std::vector<int>& pesos);
	~GrafSolucio() { ; }

	int getNumNodes() { return m_numNodes; }
	void inserirAresta(int posNode1, int posNode2, int pes);
	void afegirNode(const Coordinate& node);
	void mostra();

	void crearMatriu(const std::vector<std::vector<int>>& parelles, const std::vector<int>& pesos);

private:
	std::vector<Coordinate> m_nodes;
	std::vector<std::vector<int>> m_matriuAdj;
	size_t m_numNodes;
	size_t m_numArestes;
};

