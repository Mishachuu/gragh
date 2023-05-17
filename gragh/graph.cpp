#include <vector>
#include <list>
#include <queue>
#include <algorithm>
#include <optional>
#include <memory>
#include <iostream>
using namespace std;
class Graph {
public:
    struct Edge {
        double distance;
        int to;
        shared_ptr<Edge> next;
        Edge(int to, double dist, shared_ptr<Edge> edge) : to(to),distance(dist),next(edge) {}
    };

    struct Vertex
    {
        int id;
        shared_ptr<Edge> edge;
        Vertex(int id): id(id),edge(nullptr) {}
    };

    vector<Vertex> vertus;
    Graph() = default;
    //проверка-добавление-удаление вершин
    int has_vertex(const int id) const {
        for (int i = 0; i < vertus.size(); i++) {
            if (vertus[i].id == id) return i;
        }
        return -1;
    }
    void add_vertex(const int id) {
        if (has_vertex(id)==-1) vertus.push_back(Vertex(id));
        else throw "Vertex exist";
    }

    bool remove_vertex(const int id) {
        for (auto i = vertus.begin(); i != vertus.end(); ++i) {
            if (i->id == id) {
                vertus.erase(i);
                return true;
            }
        }
        return false;
    }


    //проверка-добавление-удаление ребер
    bool has_edge(const int& from, const int& to) const {
        if (has_vertex(from)==-1) throw "Vertex from noy exist";
        auto tmp = vertus[from].edge;
        while (tmp) {
            if (tmp->to == to) return true;
            tmp = tmp->next;
        }
        return false;
    }

    void add_edge(const int from, const int to,
        const double& d) {
        if (has_vertex(from)==-1) throw "Vertex from noy exist";
        if (has_vertex(to)==-1) throw "Vertex to not exist";
        if (!has_edge(from, to)) vertus[from].edge = make_shared<Edge>(to, d, vertus[from].edge);
    }
    bool remove_edge(const int from, const int to) {
        if (has_vertex(from)==-1) throw "Vertex from not exist";
        shared_ptr<Edge> edge = vertus[from].edge;
        shared_ptr<Edge> pred = nullptr;
        while (edge) {
            if (edge->to == to) {
                if (pred) {
                    pred->next = edge->next;
                }
                else {
                    vertus[from].edge = edge->next;
                }
                edge = nullptr;
                return true;
            }
            pred = edge;
            edge = edge->next;
        }
        return false;
    }
    bool remove_edge(const Edge& e); //c учетом расстояния
    bool has_edge(const Edge& e); //c учетом расстояния в Edge

    //получение всех ребер, выходящих из вершины
    vector<Edge> edges(const Vertex& vertex);

    size_t order() const; //порядок
    size_t degree() const; //степень

    bool belman(const int start, vector<double>& dist, vector<int>& previous) {
        dist.assign(vertus.size(), INFINITY);
        previous.assign(vertus.size(), -1);
        int start_ind = has_vertex(start);
        if (start_ind == -1) throw "Vertex start not exist";
        dist[start_ind] = 0;
        for (int i = 0; i < vertus.size() - 1; i++) {
            for (auto i : vertus) {
                shared_ptr<Edge> tmp = i.edge;
                while (tmp) {
                    int from_ind = has_vertex(i.id);
                    int to_ind = has_vertex(tmp->to);
                    if (from_ind != -1 && to_ind != -1) {
                        if (dist[from_ind] + tmp->distance < dist[to_ind]) {
                            dist[to_ind] = dist[from_ind] + tmp->distance;
                            previous[to_ind] = from_ind;
                        }
                    }
                    tmp = tmp->next;
                }
            }
        }
        for (auto i : vertus) {
            shared_ptr<Edge> tmp= i.edge;
            while (tmp) {
                int from_ind = has_vertex(i.id);
                int to_ind = has_vertex(tmp->to);
                if (from_ind != -1 && to_ind != -1) {
                    if (dist[from_ind] + tmp->distance < dist[to_ind]) {
                        return false; 
                    }
                }
            }
        }
        return true;
    }
    //поиск кратчайшего пути
    //обход
    void  walk(const int start_vertex)const {
        int start_ind = has_vertex(start_vertex);
        if (start_ind == -1) throw "Vertex start not exist";
        queue<int> q;
        vector<bool> vis(vertus.size(), false);
        q.push(start_vertex);
        vis[start_ind] = true;
        while (!q.empty()) {
           int tmp_ind = q.front();
           q.pop();
           cout << vertus[tmp_ind].id;
           shared_ptr<Edge> tmp = vertus[tmp_ind].edge;
           while (tmp) {
               int tmp_next_ind = has_vertex(tmp->to);
               if (tmp_next_ind != -1 && !vis[tmp_next_ind]) {
                   q.push(tmp_next_ind);
                   vis[tmp_next_ind] = true;
               }
               tmp = tmp->next;
           }
        }
    }

    vector<Edge> shortest_path(const int from_id, int to_id) {
        vector<double> dist;
        vector<int> previous;
        vector<Edge> path;

        if (!belman(from_id, dist, previous)) {
            throw "Graph have a negative weight";
        }

        int to_ind = has_vertex(to_id);
        if (to_ind==-1) {
            throw "Vertex to not exist";
        }

        for (int v = to_ind; v != -1; v = previous[v]) {
            if (v == previous[v]) {
                path.push_back(Edge(from_id, dist[v], nullptr));
            }
            else {
                path.push_back(Edge(vertus[v].id, dist[v] - dist[previous[v]], nullptr));
            }
        }

        reverse(path.begin(), path.end());

        return path;
    }
};

int main() {

    return 0;
}

/*В ширину
Беллмана-Форда

*/