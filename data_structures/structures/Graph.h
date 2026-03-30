// Original code by Gonçalo Leão
// Updated by DA 2024/2025 Team

#ifndef DA_TP_CLASSES_GRAPH
#define DA_TP_CLASSES_GRAPH

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <unordered_map>

template <class T>
class Edge;

#define INF std::numeric_limits<double>::max()

/************************* Vertex  **************************/

/**
 * @class Vertex
 * @brief Represents a node in the graph.
 * @tparam T The type of the information stored in the vertex.
 */
template <class T>
class Vertex {
public:
    /**
     * @brief Constructor for Vertex.
     * @param in The content or information to be stored in the vertex.
     */
    Vertex(T in);

    /**
     * @brief Gets the content of the vertex.
     * @return The information stored in the vertex.
     */
    T getInfo() const;

    /**
     * @brief Gets the outgoing edges of the vertex.
     * @return A vector of pointers to the outgoing edges.
     */
    std::vector<Edge<T> *> getAdj() const;

    /**
     * @brief Checks if the vertex has been visited during a traversal.
     * @return True if visited, false otherwise.
     */
    bool isVisited() const;

    /**
     * @brief Gets the path edge used to reach this vertex.
     * @return A pointer to the path edge.
     */
    Edge<T> *getPath() const;

    /**
     * @brief Gets the incoming edges of the vertex.
     * @return A vector of pointers to the incoming edges.
     */
    std::vector<Edge<T> *> getIncoming() const;

    /**
     * @brief Sets the content of the vertex.
     * @param info The new information to store.
     */
    void setInfo(T info);

    /**
     * @brief Sets the visited status of the vertex.
     * @param visited Boolean indicating whether the vertex was visited.
     */
    void setVisited(bool visited);

    /**
     * @brief Sets the path edge used to reach this vertex.
     * @param path Pointer to the edge.
     */
    void setPath(Edge<T> *path);

    /**
     * @brief Adds an outgoing edge to a destination vertex.
     * @param dest Pointer to the destination vertex.
     * @param w The weight of the edge.
     * @return Pointer to the newly created edge.
     */
    Edge<T> * addEdge(Vertex<T> *dest, double w);

    /**
     * @brief Removes an outgoing edge given a destination's content.
     * @param in The content of the destination vertex to remove the edge to.
     * @return True if successful, and false if such edge does not exist.
     */
    bool removeEdge(T in);

    /**
     * @brief Removes all outgoing edges from the vertex.
     */
    void removeOutgoingEdges();

protected:
    T info;                // info node
    std::vector<Edge<T> *> adj;  // outgoing edges

    // auxiliary fields
    bool visited = false; // used by DFS, BFS, Prim ...
    Edge<T> *path = nullptr;

    std::vector<Edge<T> *> incoming; // incoming edges

    /**
     * @brief Utility function to safely delete an edge.
     * @param edge Pointer to the edge to be deleted.
     */
    void deleteEdge(Edge<T> *edge);
};

/********************** Edge  ****************************/

/**
 * @class Edge
 * @brief Represents a directed edge connecting two vertices in the graph.
 * @tparam T The type of the information stored in the vertices.
 */
template <class T>
class Edge {
public:
    /**
     * @brief Constructor for Edge.
     * @param orig Pointer to the origin vertex.
     * @param dest Pointer to the destination vertex.
     * @param w The weight (or capacity) of the edge.
     */
    Edge(Vertex<T> *orig, Vertex<T> *dest, double w);

    /**
     * @brief Gets the destination vertex of the edge.
     * @return Pointer to the destination vertex.
     */
    Vertex<T> * getDest() const;

    /**
     * @brief Gets the weight or capacity of the edge.
     * @return The weight value.
     */
    double getWeight() const;

    /**
     * @brief Gets the origin vertex of the edge.
     * @return Pointer to the origin vertex.
     */
    Vertex<T> * getOrig() const;

    /**
     * @brief Gets the reverse edge if this is part of a bidirectional connection.
     * @return Pointer to the reverse edge.
     */
    Edge<T> *getReverse() const;

    /**
     * @brief Gets the current flow of the edge.
     * @return The flow value.
     */
    double getFlow() const;

    /**
     * @brief Sets the flow of the edge.
     * @param flow The new flow value.
     */
    void setFlow(double flow);

    /**
     * @brief Sets the weight or capacity of the edge.
     * @param weight The new weight value.
     */
    void setWeight(double weight);
protected:
    Vertex<T> * dest; // destination vertex
    double weight; // edge weight, can also be used for capacity

    // used for bidirectional edges
    Vertex<T> *orig;
    Edge<T> *reverse = nullptr;

    double flow; // for flow-related problems
};

/********************** Graph  ****************************/

/**
 * @class Graph
 * @brief Represents a generic graph data structure used to model assignment flows.
 * @tparam T The type of the information stored in the vertices.
 */
template <class T>
class Graph {
public:
    ~Graph() = default;

    /**
     * @brief Auxiliary function to find a vertex with a given content.
     * @param in The content to search for.
     * @return Pointer to the vertex if found, nullptr otherwise.
     */
    Vertex<T> *findVertex(const T &in) const;

    /**
     * @brief Adds a vertex with a given content to the graph.
     * @param in The content to add.
     * @return True if successful, and false if a vertex with that content already exists.
     */
    bool addVertex(const T &in);

    /**
     * @brief Removes a vertex with a given content from the graph.
     * @param in The content of the vertex to remove.
     * @return True if successful, false otherwise.
     */
    bool removeVertex(const T &in);

    /**
     * @brief Function that applies the Ford-Fulkerson algorithm to the graph.
     * @param source The content identifying the source node.
     * @param target The content identifying the target (sink) node.
     * @return The maxflow accumulated.
     */
    double fordFulkerson(int source, int target);

    /**
     * @brief Resumes the Ford-Fulkerson algorithm without resetting existing flows.
     * @param source The content identifying the source node.
     * @param target The content identifying the target (sink) node.
     * @param currentFlow The current total flow in the network.
     * @return The new maxflow accumulated.
     */
    double resumeFordFulkerson(int source, int target, double currentFlow);

    /**
     * @brief Adds an edge to the graph.
     * @param sourc The content of the source vertex.
     * @param dest The content of the destination vertex.
     * @param w The edge weight (or capacity).
     * @return True if successful, and false if the source or destination vertex does not exist.
     */
    bool addEdge(const T &sourc, const T &dest, double w);

    /**
     * @brief Removes an edge from the graph.
     * @param source The content of the source vertex.
     * @param dest The content of the destination vertex.
     * @return True if successful, false otherwise.
     */
    bool removeEdge(const T &source, const T &dest);

    /**
     * @brief Adds a bidirectional edge to the graph.
     * @param sourc The content of the source vertex.
     * @param dest The content of the destination vertex.
     * @param w The edge weight (or capacity).
     * @return True if successful, false otherwise.
     */
    bool addBidirectionalEdge(const T &sourc, const T &dest, double w);

    /**
     * @brief Gets the total number of vertices in the graph.
     * @return The count of vertices.
     */
    int getNumVertex() const;

    /**
     * @brief Gets the list of all vertices in the graph.
     * @return A vector of pointers to the vertices.
     */
    std::vector<Vertex<T> *> getVertexSet() const;

protected:
    std::vector<Vertex<T> *> vertexSet;    // vertex set
    std::unordered_map<T, Vertex<T> *> vertexMap; // O(1) vertex lookups

    /**
     * @brief Finds the index of the vertex with a given content.
     * @param in The content to look up.
     * @return The index of the vertex, or -1 if not found.
     */
    int findVertexIdx(const T &in) const;

    /**
     * @brief Auxiliary DFS function for the Ford-Fulkerson algorithm to find an augmenting path.
     * @param v The current vertex.
     * @param t The target vertex.
     * @param flow Reference to the flow accumulator.
     * @return True if an augmenting path is found, false otherwise.
     */
    bool dfsFindAugmentingPath(Vertex<T>* v, Vertex<T>* t, double& flow);
};

/************************* Vertex  **************************/

template <class T>
Vertex<T>::Vertex(T in): info(in) {}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
Edge<T> * Vertex<T>::addEdge(Vertex<T> *d, double w) {
    auto newEdge = new Edge<T>(this, d, w);
    adj.push_back(newEdge);
    d->incoming.push_back(newEdge);
    return newEdge;
}

/*
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * Returns true if successful, and false if such edge does not exist.
 */
template <class T>
bool Vertex<T>::removeEdge(T in) {
    bool removedEdge = false;
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge<T> *edge = *it;
        Vertex<T> *dest = edge->getDest();
        if (dest->getInfo() == in) {
            it = adj.erase(it);
            deleteEdge(edge);
            removedEdge = true; // allows for multiple edges to connect the same pair of vertices (multigraph)
        }
        else {
            it++;
        }
    }
    return removedEdge;
}

/*
 * Auxiliary function to remove an outgoing edge of a vertex.
 */
template <class T>
void Vertex<T>::removeOutgoingEdges() {
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge<T> *edge = *it;
        it = adj.erase(it);
        deleteEdge(edge);
    }
}

template <class T>
T Vertex<T>::getInfo() const {
    return this->info;
}

template <class T>
std::vector<Edge<T>*> Vertex<T>::getAdj() const {
    return this->adj;
}

template <class T>
bool Vertex<T>::isVisited() const {
    return this->visited;
}

template <class T>
Edge<T> *Vertex<T>::getPath() const {
    return this->path;
}

template <class T>
std::vector<Edge<T> *> Vertex<T>::getIncoming() const {
    return this->incoming;
}

template <class T>
void Vertex<T>::setInfo(T in) {
    this->info = in;
}

template <class T>
void Vertex<T>::setVisited(bool visited) {
    this->visited = visited;
}

template <class T>
void Vertex<T>::setPath(Edge<T> *path) {
    this->path = path;
}

template <class T>
void Vertex<T>::deleteEdge(Edge<T> *edge) {
    Vertex<T> *dest = edge->getDest();
    // Remove the corresponding edge from the incoming list
    auto it = dest->incoming.begin();
    while (it != dest->incoming.end()) {
        if ((*it)->getOrig()->getInfo() == info) {
            it = dest->incoming.erase(it);
        }
        else {
            it++;
        }
    }
    delete edge;
}

/********************** Edge  ****************************/

template <class T>
Edge<T>::Edge(Vertex<T> *orig, Vertex<T> *dest, double w): orig(orig), dest(dest), weight(w) {}

template <class T>
Vertex<T> * Edge<T>::getDest() const {
    return this->dest;
}

template <class T>
double Edge<T>::getWeight() const {
    return this->weight;
}

template <class T>
Vertex<T> * Edge<T>::getOrig() const {
    return this->orig;
}

template <class T>
Edge<T> *Edge<T>::getReverse() const {
    return this->reverse;
}

template <class T>
double Edge<T>::getFlow() const {
    return flow;
}

template <class T>
void Edge<T>::setFlow(double flow) {
    this->flow = flow;
}

template <class T>
void Edge<T>::setWeight(double weight) {
    this->weight = weight;
}

/********************** Graph  ****************************/

template <class T>
int Graph<T>::getNumVertex() const {
    return vertexSet.size();
}

template <class T>
std::vector<Vertex<T> *> Graph<T>::getVertexSet() const {
    return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 * REFACTORED for O(1) lookups.
 */
template <class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
    auto it = vertexMap.find(in);
    if (it != vertexMap.end()) {
        return it->second;
    }
    return nullptr;
}

/*
 * Finds the index of the vertex with a given content.
 */
template <class T>
int Graph<T>::findVertexIdx(const T &in) const {
    for (unsigned i = 0; i < vertexSet.size(); i++)
        if (vertexSet[i]->getInfo() == in)
            return i;
    return -1;
}

/*
 * Adds a vertex with a given content or info (in) to a graph (this).
 * Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool Graph<T>::addVertex(const T &in) {
    if (findVertex(in) != nullptr)
        return false;
    auto newVertex = new Vertex<T>(in);
    vertexSet.push_back(newVertex);
    vertexMap[in] = newVertex; // O(1) map insertion
    return true;
}

/*
 * Removes a vertex with a given content (in) from a graph (this), and
 * all outgoing and incoming edges.
 * Returns true if successful, and false if such vertex does not exist.
 */
template <class T>
bool Graph<T>::removeVertex(const T &in) {
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        if ((*it)->getInfo() == in) {
            auto v = *it;
            v->removeOutgoingEdges();
            for (auto u : vertexSet) {
                u->removeEdge(v->getInfo());
            }
            vertexSet.erase(it);
            vertexMap.erase(in); // O(1) map deletion
            delete v;
            return true;
        }
    }
    return false;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, w);
    return true;
}

/*
 * Removes an edge from a graph (this).
 * The edge is identified by the source (sourc) and destination (dest) contents.
 * Returns true if successful, and false if such edge does not exist.
 */
template <class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
    Vertex<T> * srcVertex = findVertex(sourc);
    if (srcVertex == nullptr) {
        return false;
    }
    return srcVertex->removeEdge(dest);
}

template <class T>
bool Graph<T>::addBidirectionalEdge(const T &sourc, const T &dest, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    auto e1 = v1->addEdge(v2, w);
    auto e2 = v2->addEdge(v1, w);
    e1->setReverse(e2);
    e2->setReverse(e1);
    return true;
}

// Function to perform DFS and find an augmenting path
template <class T>
bool Graph<T>::dfsFindAugmentingPath(Vertex<T>* v, Vertex<T>* t, double& flow) {
    if (v == t) return true;
    v->setVisited(true);

    // Forward edges
    for (auto e : v->getAdj()) {

        Vertex<T>* w = e->getDest();
        double residual = e->getWeight() - e->getFlow();
        if (!w->isVisited() && residual > 0) {
            w->setPath(e);
            if (dfsFindAugmentingPath(w, t, flow)) {
                 //Keep edge to backtrack
                flow = std::min(flow, residual);
                return true;
            }
        }
    }

    // Backward edges
    for (auto e : v->getIncoming()) {
        double residual = e->getFlow();
        Vertex<T>* w = e->getOrig();

        if (!w->isVisited() && residual > 0) {
            w->setPath(e); //keeps edge; destination for backtracking
            if (dfsFindAugmentingPath(w, t, flow)) {
                flow = std::min(flow, residual);

                return true;
            }
        }
    }
    return false;
}

// Ford-Fulkerson algorithm
template <class T>
double Graph<T>::fordFulkerson(int source, int target) {
    Vertex<T>* s = findVertex(source);
    Vertex<T>* t = findVertex(target);

    if (s == nullptr || t == nullptr || s == t)
        throw std::logic_error("Invalid source and/or target vertex");

    double maxFlow = 0;

    // Initialize all flows to 0
    for (auto v : vertexSet) {
        for (auto e : v->getAdj()) {
            e->setFlow(0);
        }
    }
    double flow=INF;
    for (Vertex<T>* v : vertexSet)
    {
        v->setVisited(false);
        v->setPath(nullptr);
    }
    while (dfsFindAugmentingPath(s, t, flow))
    {
        Vertex<T>* vertex = t;
        maxFlow += flow;

        for (Vertex<T>* v = t; v != s; )
        {
            Edge<T>* e = v->getPath();
            double floww = e->getFlow();

            if (e->getDest() == v) //If forward edge
            {
                e->setFlow(floww + flow);
                v = e->getOrig();
            } else //Backward edge
            {
                e->setFlow(floww - flow);
                v = e->getDest();
            }
        }

        flow = INF;

        for (Vertex<T>* v : vertexSet)
        {
            v->setVisited(false);
            v->setPath(nullptr);
        }

    }
    return maxFlow;
}

template <class T>
double Graph<T>::resumeFordFulkerson(int source, int target, double currentFlow) {
    Vertex<T>* s = findVertex(source);
    Vertex<T>* t = findVertex(target);

    if (s == nullptr || t == nullptr || s == t)
        throw std::logic_error("Invalid source and/or target vertex");

    double maxFlow = currentFlow;
    double flow = INF;

    for (Vertex<T>* v : vertexSet) {
        v->setVisited(false);
        v->setPath(nullptr);
    }

    while (dfsFindAugmentingPath(s, t, flow)) {
        Vertex<T>* vertex = t;
        maxFlow += flow;

        for (Vertex<T>* v = t; v != s; ) {
            Edge<T>* e = v->getPath();
            double floww = e->getFlow();

            if (e->getDest() == v) { // Forward edge
                e->setFlow(floww + flow);
                v = e->getOrig();
            } else { // Backward edge
                e->setFlow(floww - flow);
                v = e->getDest();
            }
        }

        flow = INF;

        for (Vertex<T>* v : vertexSet) {
            v->setVisited(false);
            v->setPath(nullptr);
        }
    }
    return maxFlow;
}

#endif /* DA_TP_CLASSES_GRAPH */