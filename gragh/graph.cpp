#include <vector>
#include <list>
#include <algorithm>
using namespace std;
class Graph {
private:
    struct Edge {
        double distance;
        int to;
        Edge* next;
        Edge(int to, double dist, Edge* edge) : to(to),distance(dist),next(edge) {}
    };

    struct Vertex
    {
        int id;
        Edge* edge;
        Vertex(int id): id(id),edge(nullptr) {}
    };

    vector<Vertex> vertus;
public:
    //проверка-добавление-удаление вершин
    int has_vertex(const int id) const {
        for (auto i : vertus) {
            if (i.id == id) return true;
        }
        return false;
    }
    void add_vertex(const int id) {
        if (!has_vertex(id)) vertus.push_back(Vertex(id));
        else throw "Vertex exist";
    }
    bool remove_vertex(const int id) {
        auto it = find_if(vertus.begin(), vertus.end(), has_vertex(id));
        if (it != vertus.end()) {
            vertus.erase(it);
        }
    }
    std::vector<Vertex> vertices() const;


    //проверка-добавление-удаление ребер
    bool has_edge(const int& from, const int& to) const {
        if (!has_vertex(from)) throw "Vertex from noy exist";
        auto tmp = vertus[from].edge;
        while (tmp) {
            if (tmp->to == to) return true;
            tmp = tmp->next;
        }
        return false;
    }

    void add_edge(const int from, const int to,
        const double& d) {
        if (!has_vertex(from)) throw "Vertex from noy exist";
        if (!has_vertex(to)) throw "Vertex to not exist";

    }
    bool remove_edge(const Vertex& from, const Vertex& to);
    bool remove_edge(const Edge& e); //c учетом расстояния
    bool has_edge(const Edge& e); //c учетом расстояния в Edge

    //получение всех ребер, выходящих из вершины
    vector<Edge> edges(const Vertex& vertex);

    size_t order() const; //порядок
    size_t degree() const; //степень


    //поиск кратчайшего пути
    vector<Edge> shortest_path(const Vertex& from,
        const Vertex& to) const;
    //обход
    vector<Vertex>  walk(const Vertex& start_vertex)const;
};

int main() {

    return 0;
}
