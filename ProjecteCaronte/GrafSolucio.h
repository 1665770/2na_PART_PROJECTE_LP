#pragma once
#include "Util.h"
#include "MapaSolucio.h"
#include <stack>

class GrafSolucio
{
public:
	GrafSolucio() { m_numNodes = 0; m_numArestes = 0; }
	GrafSolucio(MapaBase* mapa); //AQUEST CONSTRUCTOR �S EL M�S IMPORTANT
	~GrafSolucio() { ; }

	int getNumNodes() { return m_numNodes; }
	void inserirAresta(int posNode1, int posNode2, double pes);
	void afegirNode(const Coordinate& node);
	void mostra();

	void crearMatriu(const std::vector<std::vector<double>>& parelles, const std::vector<double>& pesos);

	std::vector<Coordinate> getCoordenades() const { return m_nodes; }

	size_t minDistance(std::vector<double>& dist, std::vector<bool>& visitat);
	void dijkstra(size_t node, std::vector<double>& dist, std::vector<size_t>& anterior);
	void camiMesCurt(const Coordinate& node1, const Coordinate& node2, std::stack<Coordinate>& cami);

	void dijkstraModif(size_t node, size_t node2, std::vector<double>& dist, std::vector<size_t>& anterior);

private:
	std::vector<Coordinate> m_nodes;
	std::vector<std::vector<double>> m_matriuAdj;
	size_t m_numNodes;
	size_t m_numArestes;
};
