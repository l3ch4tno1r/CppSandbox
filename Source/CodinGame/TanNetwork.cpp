#include <iostream>
#include <fstream>
#include <locale>
#include <string>
#include <vector>
#include <unordered_map>
#include <complex>
#include <algorithm>
#include <tuple>
#include <numeric>
#include <queue>
#include <chrono>

#define PI 3.14159265
#define TORAD(A) (PI * A) / 180

struct ComaAsSeparator : std::ctype<char>
{
	ComaAsSeparator(std::size_t refs = 0) : ctype(make_table(), false, refs) {}

	static const mask* make_table()
	{
		// make a copy of the "C" locale table
		static std::vector<mask> v(classic_table(), classic_table() + table_size);

		v[','] |= space;  // comma will be classified as whitespace
		v[' '] &= ~space; // space will not be classified as whitespace

		return &v[0];
	}
};

class NodeData
{
public:
	template<class STR1, class STR2>
	NodeData(STR1&& id, STR2&& name, float lat, float lon, int type) :
		m_Id(std::forward<STR1>(id)),
		m_Name(std::forward<STR2>(name)),
		m_Latitude(lat),
		m_Longitude(lon),
		m_Type(type)
	{}

	const std::string& Id()   const { return m_Id; }
	const std::string& Name() const { return m_Name; }

	float Latitude()  const { return m_Latitude; }
	float Longitude() const { return m_Longitude; }

private:
	std::string m_Id;
	std::string m_Name;
	float m_Latitude, m_Longitude;
	int m_Type;
};

class Network
{
public:
	template<typename ... Args>
	NodeData& CreateNode(Args&& ...args)
	{
		m_NodeData.emplace_back(std::forward<Args>(args)...);

		NodeData& node = m_NodeData.back();

		m_IdNodeIndex[node.Id()] = m_NodeData.size() - 1;

		return m_NodeData.back();
	}

	size_t GetNodeIndex(const std::string& id) const
	{
		auto it = m_IdNodeIndex.find(id);

		if (it == m_IdNodeIndex.end())
			throw std::exception("Node not found !");

		return it->second;
	}

	const NodeData& GetNodeData(size_t idx) const
	{
		return m_NodeData[idx];
	}

	const NodeData& GetNodeData(const std::string& id) const
	{
		return m_NodeData[this->GetNodeIndex(id)];
	}

	void AddEdge(const std::string& id1, const std::string& id2)
	{
		m_Edges.emplace_back(this->GetNodeIndex(id1), this->GetNodeIndex(id2));
	}

	//////////////////////
	class Node;

	class NeighboorList
	{
	public:
		std::tuple<Node, float> operator[](size_t idx) const
		{
			size_t nidx = m_Network.m_Edges[m_Begin + idx].Node2Idx;
			float weight = m_Network.m_Edges[m_Begin + idx].Weight;

			return { m_Network.Nodes(nidx), weight };
		}

		size_t Size() const { return m_End - m_Begin; }

	private:
		NeighboorList(const Network& network, size_t begin, size_t end) :
			m_Network(network),
			m_Begin(begin),
			m_End(end)
		{}

		friend Node;

		const Network& m_Network;
		size_t m_Begin;
		size_t m_End;
	};

	class Node
	{
	public:
		const NodeData* operator->() const { return &m_Network.GetNodeData(m_NodeIdx); }

		const NodeData& operator*() const { return m_Network.GetNodeData(m_NodeIdx); }

		NeighboorList Neighboors() const
		{
			size_t begin = m_Network.m_EdgesNodeLUT[m_NodeIdx];
			size_t end   = m_Network.m_EdgesNodeLUT[m_NodeIdx + 1];

			return { m_Network, begin, end };
		}

		size_t NodeIdx() const { return m_NodeIdx; }

		operator size_t() const { return m_NodeIdx; }

		Node& operator=(const Node& other)
		{
			m_NodeIdx = other.m_NodeIdx;

			return *this;
		}

	private:
		Node(const Network& ref, size_t idx) :
			m_Network(ref),
			m_NodeIdx(idx)
		{}

		friend Network;

		const Network& m_Network;
		size_t m_NodeIdx;
	};

	struct Edge
	{
		size_t Node1Idx, Node2Idx;
		float Weight;

		Edge(size_t node1, size_t node2) :
			Node1Idx(node1),
			Node2Idx(node2),
			Weight(0.0f)
		{}
	};

	//////////////////////
	Node Nodes(size_t idx) const
	{
		return { *this, idx };
	}

	Node Nodes(const std::string& key) const
	{
		return { *this, this->GetNodeIndex(key) };
	}

	size_t NodeCount() const { return m_NodeData.size(); }

	size_t EdgeCount() const { return m_Edges.size(); }

	void ImportNodeData(std::vector<NodeData>&& nodeDataSet)
	{
		m_NodeData = std::move(nodeDataSet);

		size_t idx = 0;

		for (const auto& nodeData : m_NodeData)
			m_IdNodeIndex[nodeData.Id()] = idx++;
	}

	template<class WeightFunc>
	void ImportEdges(std::vector<Edge>&& edgesSet, WeightFunc F)
	{
		m_Edges = std::move(edgesSet);

		for (auto& edge : m_Edges)
		{
			Node node1{ *this, edge.Node1Idx };
			Node node2{ *this, edge.Node2Idx };

			edge.Weight = F(node1, node2);
		}

		GroupEdges();
	}

private:
	std::vector<NodeData> m_NodeData;
	std::unordered_map<std::string, size_t> m_IdNodeIndex;

	std::vector<Edge> m_Edges;
	std::vector<size_t> m_EdgesNodeLUT;

	void GroupEdges()
	{
		if (m_Edges.empty())
			return;

		std::sort(m_Edges.begin(), m_Edges.end(), [](const Edge& e1, const Edge& e2)
		{
			return e1.Node1Idx < e2.Node1Idx;
		});

		auto it = m_Edges.begin();

		size_t lastVal = it->Node1Idx;

		m_EdgesNodeLUT.reserve(m_NodeData.size() + 1);
		m_EdgesNodeLUT.push_back(0);

		while (++it != m_Edges.end())
		{
			if (it->Node1Idx == lastVal)
				continue;

			for (size_t k = lastVal; k < it->Node1Idx - 1; ++k)
				m_EdgesNodeLUT.push_back(m_EdgesNodeLUT.back());

			m_EdgesNodeLUT.push_back(std::distance(m_Edges.begin(), it));

			lastVal = it->Node1Idx;
		}

		m_EdgesNodeLUT.push_back(m_Edges.size());

		while (m_EdgesNodeLUT.size() < m_NodeData.size() + 1)
			m_EdgesNodeLUT.push_back(m_EdgesNodeLUT.back());
	}
};

std::istream& operator>>(std::istream& stream, Network& network)
{
	auto oldLocale = stream.imbue(std::locale(stream.getloc(), new ComaAsSeparator));

	// Import Node data
	int numStop;
	stream >> numStop;

	std::vector<NodeData> nodeData;
	nodeData.reserve(numStop);

	for (int i = 0; i < numStop; ++i)
	{
		std::string id, name;
		float lat, lon;
		int type;

		stream >> id >> name >> lat >> lon >> type; stream.ignore();

		nodeData.emplace_back(std::move(id), name.substr(1, name.size() - 2), lat, lon, type);
	}

	network.ImportNodeData(std::move(nodeData));

	stream.imbue(oldLocale);

	// Import Edges
	int linkCount;
	stream >> linkCount;

	std::vector<Network::Edge> edges;
	edges.reserve(linkCount);

	for (int i = 0; i < linkCount; ++i)
	{
		std::string id1, id2;

		stream >> id1 >> id2; stream.ignore();

		if (id1.empty() || id2.empty())
			continue;

		size_t idx1 = network.GetNodeIndex(id1);
		size_t idx2 = network.GetNodeIndex(id2);

		edges.emplace_back(idx1, idx2);
	}

	network.ImportEdges(std::move(edges), [](Network::Node a, Network::Node b)
	{
		float lonA = TORAD(a->Longitude()), latA = TORAD(a->Latitude());
		float lonB = TORAD(b->Longitude()), latB = TORAD(b->Latitude());

		float x = (lonB - lonA) * std::cos((latA + latB) / 2);
		float y = latB - latA;

		return 6371 * std::abs(std::complex<float>(x, y));
	});

	return stream;
}

std::vector<Network::Node> DijkstraAlgorithm(
	const Network& network,
	Network::Node start,
	Network::Node end)
{
	struct Status
	{
		int   Predecessor;
		float Distance;
		bool  Visited;
	};

	std::vector<Status> nodeStatus(network.NodeCount(), { -1, std::numeric_limits<float>::infinity(), false });

	nodeStatus[start] = { -1, 0.0f, true };

	auto compare = [&](size_t n1, size_t n2)
	{
		return nodeStatus[n2].Distance < nodeStatus[n1].Distance;
	};

	std::priority_queue<
		size_t,
		std::vector<size_t>,
		decltype(compare)> pqueue(compare);

	pqueue.push(start);

	while (!pqueue.empty() && end != pqueue.top())
	{
		Network::Node currentNode = network.Nodes(pqueue.top());
		pqueue.pop();

		Network::NeighboorList neighboors = currentNode.Neighboors();

		float currentDistance = nodeStatus[currentNode].Distance;

		// Add non visited neighboors
		for (size_t i = 0; i < neighboors.Size(); ++i)
		{
			auto [node, distance] = neighboors[i];

			Status& status = nodeStatus[node];

			if (status.Visited)
				continue;

			float cumulatedDistance = currentDistance + distance;

			if (cumulatedDistance >= status.Distance)
				continue;

			status.Distance    = cumulatedDistance;
			status.Predecessor = currentNode;

			pqueue.push(node);
		}

		nodeStatus[currentNode].Visited = true;
	}

	// Backtrack the path
	if (start != end && nodeStatus[end].Predecessor == -1)
		return {};

	size_t idx = end;
	std::vector<Network::Node> result;

	while (nodeStatus[idx].Predecessor != -1)
	{
		result.push_back(network.Nodes(idx));

		idx = nodeStatus[idx].Predecessor;
	}

	result.push_back(network.Nodes(idx));

	return result;
}

int main()
{
	try
	{
		//std::ifstream file("Ressources/CodinGame/TAN Network - Custom Dataset.txt", std::ios::in);
		//std::ifstream file("Ressources/CodinGame/TAN Network - Exemple Test.txt", std::ios::in);
		//std::ifstream file("Ressources/CodinGame/TAN Network - Big Dataset.txt", std::ios::in);
		//std::ifstream file("Ressources/CodinGame/TAN Network - Small Dataset.txt", std::ios::in);
		//std::ifstream file("Ressources/CodinGame/TAN Network - One stop only.txt", std::ios::in);
		//std::ifstream file("Ressources/CodinGame/TAN Network - Impossible.txt", std::ios::in);
		std::ifstream file("Ressources/CodinGame/TAN Network - Big number of steps.txt", std::ios::in);

		if (!file)
			throw std::exception("File not found !");

		std::string startPoint;
		std::getline(file, startPoint);

		std::string endPoint;
		std::getline(file, endPoint);

		Network network;

		file >> network;

		auto start = std::chrono::high_resolution_clock::now();

		auto result = DijkstraAlgorithm(
			network,
			network.Nodes(startPoint),
			network.Nodes(endPoint));

		std::cerr << "Done shortest path in " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() << std::endl;

		if (result.empty())
			std::cout << "IMPOSSIBLE" << std::endl;
		else
		{
			std::reverse(result.begin(), result.end());

			for (const Network::Node& node : result)
				std::cout << node->Name() << std::endl;
		}

	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception : " << e.what() << std::endl;
	}

	std::cout << "Done !" << std::endl;

	std::cin.get();
}