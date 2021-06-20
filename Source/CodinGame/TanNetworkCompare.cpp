#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <chrono>
#include <fstream>

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679

struct Stops {
    static int id_gen;
    double latitude, longitude;
    std::string name;
    int id = id_gen++;
    std::string identifier;
    int stopType;

    static void splitString(std::string s, const std::string& delim, std::vector<std::string>& output) {
        size_t pos = 0;
        std::string token;
        while ((pos = s.find(delim)) != std::string::npos) {
            output.emplace_back(s.substr(0, pos));
            s.erase(0, pos + delim.length());
        }
        output.emplace_back(s);
    }

public:

    explicit Stops(std::string nodeString) {

        std::vector<std::string> res;
        splitString(nodeString, ",", res);

        identifier = res[0];
        name = res[1].substr(1, res[1].size() - 2);
        latitude = std::stod(res[3]) * PI / 180.0;
        longitude = std::stod(res[4]) * PI / 180.0;
        stopType = std::stoi(res[7]);

    }

};

int Stops::id_gen = 0;

struct Edge {
    int destination;
    double distance;

    Edge(int destination, double distance) : destination(destination), distance(distance) {}
};

struct Graph {
    int nodeAmount;
    std::vector<Edge>* edges;
    double* latitudes;
    double* longitudes;
    std::string* names;

    Graph(int nodeAmount) : nodeAmount(nodeAmount) {
        edges = new std::vector<Edge>[nodeAmount];
        latitudes = new double[nodeAmount];
        longitudes = new double[nodeAmount];
        names = new std::string[nodeAmount];
    }

    void addNode(const Stops& n) {
        latitudes[n.id] = n.latitude;
        longitudes[n.id] = n.longitude;
        names[n.id] = n.name;
    }

    void addEdge(const Stops& n1, const Stops& n2) {
        double d = distance(n1.id, n2.id);
        edges[n1.id].emplace_back(Edge(n2.id, d));

        if (n1.id == n2.id) {
            std::cerr << n1.name << std::endl;
        }
    }

    class Comp {
        double* h;
    public:

        Comp(double* h) : h(h) {}

        bool operator()(const int i1, const int i2) {
            return h[i1] < h[i2];
        }

    };

    double distance(int start, int destination) const {
        double x = (longitudes[destination] - longitudes[start]) * cos((latitudes[start] + latitudes[destination]) / 2);
        double y = latitudes[destination] - latitudes[start];
        return sqrt(x * x + y * y) * 6371;
    }

    void aStar(int start, int goal) {

        std::cerr << "Start: " << start << " end: " << goal << std::endl;
        if (start == goal) {
            std::cout << names[start] << std::endl;
            return;
        }

        int* prev = new int[nodeAmount];

        auto* f = new double[nodeAmount];
        auto* g = new double[nodeAmount];
        auto* h = new double[nodeAmount];

        bool* closed = new bool[nodeAmount];

        for (int i = 0; i < nodeAmount; i++) {
            prev[i] = 0;
            f[i] = 0.0;
            g[i] = 0.0;
            h[i] = 0.0;
            closed[i] = false;
        }

        std::vector<int> open;


        open.emplace_back(start);


        while (!open.empty()) {

            auto comp = Comp(h);
            auto it = min_element(open.begin(), open.end(), comp);
            int current = *it;
            open.erase(it);

            //            cerr << "Processing:  " << current << endl;

            for (auto& e : edges[current]) {

                int next = e.destination;

                if (next == goal) {
                    prev[next] = current;

                    std::cerr << "Found!!!" << std::endl;
                    std::vector<std::string> path;
                    path.insert(path.begin(), names[next]);

                    //cerr << "Backtracking: " << next << endl;

                    while (next != start) {
                        next = prev[next];

                        //cerr << "Backtracking: " << next << endl;

                        if (next == prev[next]) {
                            std::cerr << "WTF" << std::endl;
                            return;
                        }

                        path.insert(path.begin(), names[next]);
                    }

                    for (auto& name : path) {
                        std::cout << name << std::endl;
                    }

                    return;
                }


                if (!closed[next] && next != current) {

                    double newF = f[current] + e.distance;
                    auto it = find(open.begin(), open.end(), next);

                    if (it != open.end()) {
                        if (newF >= f[*it]) continue;
                    }
                    else {
                        open.emplace_back(next);
                    }

                    if (next == 221) {
                        std::cerr << next << " vs " << current << std::endl;
                    }
                    prev[next] = current;
                    f[next] = newF;
                    h[next] = f[next] + g[next];

                }

            }

            closed[current] = true;

        }


        std::cout << "IMPOSSIBLE" << std::endl;


    }

};


int main()
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
    file >> startPoint; file.ignore();
    std::string endPoint;
    file >> endPoint; file.ignore();
    int N;
    file >> N; file.ignore();

    std::map<std::string, Stops> stops;
    Graph graph{ N };


    for (int i = 0; i < N; i++) {
        std::string stopName;
        std::getline(file, stopName);

        auto node = Stops(stopName);
        if (node.stopType != 1) {
            std::cerr << node.stopType << " " << node.identifier << std::endl;
        }
        stops.insert(make_pair(node.identifier, node));
        graph.addNode(node);

    }
    int M;
    file >> M; file.ignore();
    for (int i = 0; i < M; i++) {
        std::string route;
        std::getline(file, route);

        std::vector<std::string> p;
        Stops::splitString(route, " ", p);

        std::string firstStop = p[0];
        std::string secondStop = p[1];

        if (firstStop == secondStop) {
            //cerr << "This is bizarre:" << stops.at(firstStop).name << " -> " << route << " stop type: " << stops.at(firstStop).stopType << endl;
        }

        if (firstStop != secondStop) {
            graph.addEdge(stops.at(firstStop), stops.at(secondStop));
        }


    }

    auto start = std::chrono::high_resolution_clock::now();

    graph.aStar(stops.at(startPoint).id, stops.at(endPoint).id);

    std::cerr << "Done shortest path in " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() << std::endl;

    std::cin.get();
}