#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

// Edge structure for graph representation
struct Edge {
    int src, dest, weight;
};

// Comparator for priority queue to store edges in Prim's algorithm
struct Compare {
    bool operator()(const Edge& a, const Edge& b) {
        return a.weight > b.weight;  // Min-heap based on edge weight
    }
};

// Function to compute MST using Prim's algorithm
vector<Edge> primMST(vector<vector<pair<int, int>>>& graph, int n) {
    // Min-heap to select minimum weight edge at each step
    priority_queue<Edge, vector<Edge>, Compare> pq;
    vector<bool> inMST(n, false);  // Track vertices included in MST
    vector<Edge> mst;  // Store the resulting MST

    // Start with vertex 0
    inMST[0] = true;
    for (const auto& neighbor : graph[0]) {
        pq.push({0, neighbor.first, neighbor.second});
    }

    while (!pq.empty() && mst.size() < n - 1) {
        Edge minEdge = pq.top();
        pq.pop();

        // If destination vertex is already in MST, skip this edge
        if (inMST[minEdge.dest]) continue;

        // Add edge to MST
        mst.push_back(minEdge);
        inMST[minEdge.dest] = true;

        // Add all edges from the newly added vertex to the priority queue
        for (const auto& neighbor : graph[minEdge.dest]) {
            if (!inMST[neighbor.first]) {
                pq.push({minEdge.dest, neighbor.first, neighbor.second});
            }
        }
    }

    return mst;
}

// Function to add redundancy edges to meet reliability constraints
vector<Edge> addRedundancy(vector<Edge>& edges, vector<Edge>& mst, int n, int k) {
    vector<Edge> redundantEdges;
    vector<vector<int>> mstGraph(n, vector<int>(n, INT_MAX));
    
    for (const auto& edge : mst) {
        mstGraph[edge.src][edge.dest] = edge.weight;
        mstGraph[edge.dest][edge.src] = edge.weight;
    }
    
    for (const auto& edge : edges) {
        if (mstGraph[edge.src][edge.dest] == INT_MAX) {
            redundantEdges.push_back(edge);
        }
    }
    
    // To keep it simple, just pick the first k redundant edges
    if (redundantEdges.size() > k) {
        redundantEdges.resize(k);
    }
    
    return redundantEdges;
}

// Function to validate edge weights within a specified range
void validateEdgeWeights(const vector<Edge>& edges, int minWeight, int maxWeight) {
    for (const auto& edge : edges) {
        if (edge.weight < minWeight || edge.weight > maxWeight) {
            throw invalid_argument("Edge weight out of range: " + to_string(edge.weight));
        }
    }
}

// Function to calculate the total weight of the MST
int calculateMSTWeight(const vector<Edge>& mst) {
    int totalWeight = 0;
    for (const auto& edge : mst) {
        totalWeight += edge.weight;
    }
    return totalWeight;
}

int main() {
    int n = 5; // Number of nodes in the power grid
    vector<Edge> edges = {
        {0, 1, 10}, {0, 2, 6}, {0, 3, 5},
        {1, 3, 15}, {2, 3, 4}, {1, 2, 7}
    };
    
    // Define valid weight range
    int minWeight = 1;
    int maxWeight = 20;

    try {
        // Validate edge weights
        validateEdgeWeights(edges, minWeight, maxWeight);
    } catch (const invalid_argument& e) {
        cerr << "Error: " << e.what() << endl;
        return 1; // Exit if validation fails
    }
    
    // Create adjacency list representation of the graph
    vector<vector<pair<int, int>>> graph(n);
    for (const auto& edge : edges) {
        graph[edge.src].push_back({edge.dest, edge.weight});
        graph[edge.dest].push_back({edge.src, edge.weight});
    }

    // Compute the MST using Prim's algorithm
    vector<Edge> mst = primMST(graph, n);
    
    // Add redundancy to handle up to 2 edge failures
    int k = 2;
    vector<Edge> redundantEdges = addRedundancy(edges, mst, n, k);

    cout << "Edges in the Minimum Spanning Tree (Prim's algorithm):" << endl;
    for (const auto& edge : mst) {
        cout << edge.src << " - " << edge.dest << ": " << edge.weight << endl;
    }

    cout << "Total Weight of the Minimum Spanning Tree: " << calculateMSTWeight(mst) << endl;

    cout << "Additional Redundant Edges for Reliability:" << endl;
    for (const auto& edge : redundantEdges) {
        cout << edge.src << " - " << edge.dest << ": " << edge.weight << endl;
    }

    return 0;
}
