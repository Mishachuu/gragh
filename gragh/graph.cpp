#include <vector>
#include <list>
#include <queue>
#include <algorithm>
#include <optional>
#include <memory>
#include <iostream>
#include <conio.h>
using namespace std;
class Graph {
public:
    struct Edge {
        double distance;
        int to;
        shared_ptr<Edge> next;
        Edge(int to, double dist, shared_ptr<Edge> edge) : to(to), distance(dist), next(edge) {}
    };

    struct Vertex
    {
        int id;
        shared_ptr<Edge> edge;
        Vertex(int id) : id(id), edge(nullptr) {}
    };

    vector<Vertex> vertus;

    Graph() = default;

    int has_vertex(const int id) const {
        for (int i = 0; i < vertus.size(); i++) {
            if (vertus[i].id == id) return i;
        }
        return -1;
    }

    void add_vertex(const int id) {
        if (has_vertex(id) == -1) vertus.push_back(Vertex(id));
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

    bool has_edge(const int& from, const int& to) const {
        if (has_vertex(from) == -1) throw "Vertex from not exist";
        auto tmp = vertus[from].edge;
        while (tmp) {
            if (tmp->to == to) return true;
            tmp = tmp->next;
        }
        return false;
    }

    void add_edge(const int from, const int to, const double& d) {
        if (has_vertex(from) == -1) throw "Vertex from noy exist";
        if (has_vertex(to) == -1) throw "Vertex to not exist";
        if (!has_edge(from, to)) vertus[from].edge = make_shared<Edge>(to, d, vertus[from].edge);
    }

    bool remove_edge(const int from, const int to) {
        if (has_vertex(from) == -1) throw "Vertex from not exist";
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
            shared_ptr<Edge> tmp = i.edge;
            while (tmp) {
                int from_ind = has_vertex(i.id);
                int to_ind = has_vertex(tmp->to);
                if (from_ind != -1 && to_ind != -1) {
                    if (dist[from_ind] + tmp->distance < dist[to_ind]) {
                        return false;
                    }
                }
                tmp = tmp->next;
            }
        }
        return true;
    }

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
        if (to_ind == -1) {
            throw "Vertex to not exist";
        }

        for (int v = to_ind; v != -1; v = previous[v]) {
            if (v == from_id) {
                path.push_back(Edge(from_id, dist[v], nullptr));
            }
            else {
                path.push_back(Edge(vertus[v].id, dist[v] - dist[previous[v]], nullptr));
            }
        }

        reverse(path.begin(), path.end());

        return path;
    }

    int task()
    {
        if (vertus.size() == 0) throw "Graph is empty";

        double max_sum = -INFINITY;
        int max_id = -1;
        for (auto& vertex : vertus) {
            double sum = 0;
            shared_ptr<Edge> tmp = vertex.edge;
            while (tmp) {
                sum += tmp->distance;
                tmp = tmp->next;
            }

            for (auto& v : vertus) {
                shared_ptr<Edge> tmp = v.edge;
                while (tmp) {
                    if (tmp->to == vertex.id) {
                        sum += tmp->distance;
                        break;
                    }
                    tmp = tmp->next;
                }
            }
            if (sum > max_sum) {
                max_sum = sum;
                max_id = vertex.id;
            }
        }

        return max_id;
    }
};


int get_key_T()
{
    int key = _getch();
    if ((key == 0) || (key == 224))
        key = _getch();
    return key;
}


int menu_T() {
    cout << "\nWhat are you want to do?\n\n"
        "1 - add vertex\n"
        "2 - add edge\n"
        "3 - check vertex\n"
        "4 - check edge\n"
        "5 - walk\n"
        "6 - task\n"
        "7 - walk\n"
        "8 -  shortest_path\n"
        "go out: Esc\n";


    while (true)
    {
        int key = get_key_T();
        if ((key == 27) || (key >= '0' && key <= '8'))
            return key;
    }
}


int Check()
{
    int number = -1;
    while (number <= 0)
    {
        while (!(cin >> number) || (cin.peek() != '\n'))
        {
            cin.clear();
            while (cin.get() != '\n');
            cout << "Error. Repeat input\n";
        }
        if (number <= 0) cout << "Error. Enter a positive number\n";

    }

    return number;
}


double Check_double()
{
    double number;
    while (!(cin >> number) || (cin.peek() != '\n'))
    {
        cin.clear();
        while (cin.get() != '\n');
        cout << "Error. Repeat input\n";
    }
    if (number <= 0) cout << "Error. Enter a positive number\n";
    return number;
}


void add_vertex(Graph* graph) {
    system("cls");
    cout << "Enter vertex id:";
    int v = Check();
    while (graph->has_vertex(v) != -1) {
        cout << "Error. Enter vertex id:";
        int v = Check();
    }
    graph->add_vertex(v);
}


void add_edge(Graph* graph) {
    system("cls");
    cout << "Enter vertex id from:";
    int from_v = Check();
    while (graph->has_vertex(from_v) == -1) {
        cout << "Error. Enter vertex id:";
        int from_v = Check();
    }
    cout << "Enter vertex id to:";
    int to_v = Check();
    while (graph->has_vertex(to_v) == -1) {
        cout << "Error. Enter vertex id:";
        int to_v = Check();
    }
    cout << "Enter edge weight:";
    double weight;
    cin >> weight;
    while (graph->has_edge(from_v, to_v) == true) {
        system("cls");
        cout << "Edge eist. Repeat input";
        cout << "Enter vertex id from:";
        int from_v = Check();
        cout << "Enter vertex id to:";
        int to_v = Check();
        cout << "Enter edge weight:";
        double edge = Check_double();
    }
    graph->add_edge(from_v, to_v, weight);
}


void check_vertex(Graph* graph) {
    system("cls");
    cout << "Enter vertex id from:";
    int v = Check();
    if (graph->has_vertex(v) != -1) cout << "Vertex exist";
    else cout << "Vertex not exist";
}


void check_edge(Graph* graph) {
    system("cls");
    cout << "Enter vertex id from:";
    int from_v = Check();
    cout << "Enter vertex id to:";
    int to_v = Check();
    if (graph->has_edge(from_v, to_v) == true) cout << "Edge exist";
    else cout << "Edge not exist";
}


void print(Graph graph) {
    for (auto i : graph.vertus) {
        cout << "Vertex " << i.id << " -> ";
        shared_ptr<Graph::Edge> tmp = i.edge;
        while (tmp) {
            cout << tmp->to << " (weight " << tmp->distance << "), ";
            tmp = tmp->next;
        }
    }
}


void task(Graph graph) {
    system("cls");
    cout << "The vertex is farthest from its neighbors:" << graph.task();
}


void walk(Graph graph) {
    cout << "Enter the vertex to walk:";
    int i = Check();
    graph.walk(i);
}


void shortest_path(Graph graph) {
    cout << "write start";
    int start = Check();
    cout << "write end";
    int to = Check();
    vector<Graph::Edge> path = graph.shortest_path(start, to);
    for (auto i : path) {
        cout << i.to << ' ' << i.distance;
    }
}


int main() {
    Graph graph;
    while (true) {
        int m = menu_T();
        if (m == 27) break;
        switch (m)
        {
        case '1':
            add_vertex(&graph);
            break;
        case '2':
            add_edge(&graph);
            break;
        case '3':
            check_vertex(&graph);
            break;
        case '4':
            check_edge(&graph);
            break;
        case '5':
            print(graph);
            break;
        case '6':
            task(graph);
            break;
        case '7':
            walk(graph);
            break;
        case '8':
            shortest_path(graph);
            break;
        }
    }
    return 0;
}