#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

// Edge structure for Kruskal's algorithm
struct Edge {
    int src, dest, weight;
};

// Union-Find data structure for Kruskal's algorithm
class UnionFind {
    vector<int> parent, rank;
public:
    UnionFind(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }

    int find(int u) {
        if (parent[u] != u) {
            parent[u] = find(parent[u]);  // Path compression
        }
        return parent[u];
    }

    void unionSets(int u, int v) {
        int rootU = find(u);
        int rootV = find(v);

        if (rootU != rootV) {
            if (rank[rootU] > rank[rootV]) {
                parent[rootV] = rootU;
            } else if (rank[rootU] < rank[rootV]) {
                parent[rootU] = rootV;
            } else {
                parent[rootV] = rootU;
                rank[rootU]++;
            }
        }
    }
};

// Function to compute the MST using Kruskal's algorithm
vector<Edge> kruskalMST(vector<Edge>& edges, int n) {
    // Sort edges by weight
    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.weight < b.weight;
    });

    UnionFind uf(n);
    vector<Edge> mst;

    // Build the MST
    for (const auto& edge : edges) {
        if (uf.find(edge.src) != uf.find(edge.dest)) {
            uf.unionSets(edge.src, edge.dest);
            mst.push_back(edge);
        }
    }
    return mst;
}

// Function to add redundant edges
vector<Edge> addRedundancy(const vector<Edge>& edges, const vector<Edge>& mst, int n, int k) {
    vector<Edge> redundantEdges;
    vector<vector<int>> mstGraph(n, vector<int>(n, INT_MAX));

    // Fill the MST graph matrix
    for (const auto& edge : mst) {
        mstGraph[edge.src][edge.dest] = edge.weight;
        mstGraph[edge.dest][edge.src] = edge.weight;
    }

    // Find redundant edges
    for (const auto& edge : edges) {
        if (mstGraph[edge.src][edge.dest] == INT_MAX) {
            redundantEdges.push_back(edge);
        }
    }

    // Limit to k redundant edges
    if (redundantEdges.size() > k) {
        redundantEdges.resize(k);
    }

    return redundantEdges;
}

// Function to validate edge weights
bool validateEdgeWeights(const vector<Edge>& edges, int minWeight, int maxWeight) {
    for (const auto& edge : edges) {
        if (edge.weight < minWeight || edge.weight > maxWeight) {
            cout << "Error: Edge weight out of range: " << edge.weight << endl;
            return false;
        }
    }
    return true;
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
    int n = 5;  // Number of nodes
    vector<Edge> edges = {
        {0, 1, 10}, {0, 2, 6}, {0, 3, 5},
        {1, 3, 15}, {2, 3, 4}, {1, 2, 7}
    };

    // Define valid weight range
    int minWeight = 1;
    int maxWeight = 20;

    // Validate edge weights
    if (!validateEdgeWeights(edges, minWeight, maxWeight)) {
        return 1;  // Exit if validation fails
    }

    // Compute the MST
    vector<Edge> mst = kruskalMST(edges, n);

    // Add redundancy to handle up to 2 edge failures
    int k = 2;
    vector<Edge> redundantEdges = addRedundancy(edges, mst, n, k);

    // Print the MST
    cout << "Edges in the Minimum Spanning Tree:" << endl;
    for (const auto& edge : mst) {
        cout << edge.src << " - " << edge.dest << ": " << edge.weight << endl;
    }

    // Print total weight of the MST
    cout << "Total Weight of the Minimum Spanning Tree: " << calculateMSTWeight(mst) << endl;

    // Print redundant edges for reliability
    cout << "Additional Redundant Edges for Reliability:" << endl;
    for (const auto& edge : redundantEdges) {
        cout << edge.src << " - " << edge.dest << ": " << edge.weight << endl;
    }

    return 0;
}
