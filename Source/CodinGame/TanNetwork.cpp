#include <iostream>
#include <fstream>
#include <locale>
#include <string>
#include <vector>
#include <unordered_map>
#include <complex>
#include <algorithm>

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
	NodeData(STR1&& id, STR2&& name, float x, float y, int type) :
		m_Id(std::forward<STR1>(id)),
		m_Name(std::forward<STR2>(name)),
		m_Pos(x, y),
		m_Type(type)
	{}

	const std::string& Id()   const { return m_Id; }
	const std::string& Name() const { return m_Name; }

	float X() const { return m_Pos.real(); }
	float Y() const { return m_Pos.imag(); }

	const std::complex<float>& Position() const { return m_Pos; }

private:
	std::string m_Id;
	std::string m_Name;
	std::complex<float> m_Pos;
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

	size_t GetNodeId(const std::string& id) const
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
		return m_NodeData[this->GetNodeId(id)];
	}

	void AddEdge(const std::string& id1, const std::string& id2)
	{
		m_Edges.emplace_back(this->GetNodeId(id1), this->GetNodeId(id2));
	}

	void GroupEdges()
	{
		if (m_Edges.empty())
			return;

		std::sort(m_Edges.begin(), m_Edges.end());

		auto it = m_Edges.begin();

		size_t lastVal = it->first;

		m_EdgesByNode.push_back(0);

		while (++it != m_Edges.end())
		{
			if (it->first == lastVal)
				continue;

			for (size_t k = lastVal; k < it->first - 1; ++k)
				m_EdgesByNode.push_back(m_EdgesByNode.back());

			m_EdgesByNode.push_back(std::distance(m_Edges.begin(), it));

			lastVal = it->first;
		}

		m_EdgesByNode.push_back(m_Edges.size());

		while (m_EdgesByNode.size() < m_NodeData.size() + 1)
			m_EdgesByNode.push_back(m_EdgesByNode.back());
	}

	class Node
	{
	public:
		const NodeData* operator->() const { return &m_Network.GetNodeData(m_NodeIdx); }

		const NodeData& operator*() const { return m_Network.GetNodeData(m_NodeIdx); }

		//GetFirstNeighboor()

	private:
		Node(const Network& ref, size_t idx) :
			m_Network(ref),
			m_NodeIdx(idx)
		{}

		friend Network;

		const Network& m_Network;
		size_t m_NodeIdx;
	};

	Node GetNode(size_t idx)
	{
		return { *this, idx };
	}

private:
	std::vector<NodeData> m_NodeData;
	std::unordered_map<std::string, size_t> m_IdNodeIndex;

	std::vector<std::pair<size_t, size_t>> m_Edges;
	std::vector<size_t> m_EdgesByNode;
};

int main()
{
	try
	{
		std::ifstream file("Ressources/CodinGame/TAN Network - Exemple Test.txt", std::ios::in);
		//std::ifstream file("Ressources/CodinGame/TAN Network - Big Dataset.txt", std::ios::in);
		//std::ifstream file("Ressources/CodinGame/TAN Network - Small Dataset.txt", std::ios::in);

		if (!file)
			throw std::exception("File not found !");

		std::string startPoint;
		std::getline(file, startPoint);

		std::string endPoint;
		std::getline(file, endPoint);

		int numStop;
		file >> numStop;

		auto oldLocale = file.imbue(std::locale(file.getloc(), new ComaAsSeparator));

		Network network;

		for (int i = 0; i < numStop; ++i)
		{
			std::string id, name;
			float lat, lon;
			int type;

			file >> id >> name >> lat >> lon >> type; file.ignore();

			NodeData& node = network.CreateNode(std::move(id), name.substr(1, name.size() - 2), lat, lon, type);
		}

		file.imbue(oldLocale);

		int linkCount;
		file >> linkCount;

		for (int i = 0; i < linkCount; ++i)
		{
			std::string id1, id2;

			file >> id1 >> id2; file.ignore();

			network.AddEdge(id1, id2);
		}

		network.GroupEdges();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception : " << e.what() << std::endl;
	}

	std::cin.get();
}