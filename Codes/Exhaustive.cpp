#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <stdexcept>

using namespace std;

// Edge structure for representing a graph edge
struct Edge {
    int src, dest, weight;
};

// Union-Find data structure to check for connected components
class UnionFind {
    vector<int> parent, rank;
public:
    UnionFind(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }
    int find(int u) {
        if (parent[u] != u) parent[u] = find(parent[u]);
        return parent[u];
    }
    void unionSets(int u, int v) {
        int rootU = find(u);
        int rootV = find(v);
        if (rootU != rootV) {
            if (rank[rootU] > rank[rootV]) parent[rootV] = rootU;
            else if (rank[rootU] < rank[rootV]) parent[rootU] = rootV;
            else {
                parent[rootV] = rootU;
                ++rank[rootU];
            }
        }
    }
};

// Function to check if a subset of edges forms a valid spanning tree
bool isValidSpanningTree(const vector<Edge>& edges, int n) {
    UnionFind uf(n);
    int edgeCount = 0;
    
    for (const auto& edge : edges) {
        if (uf.find(edge.src) != uf.find(edge.dest)) {
            uf.unionSets(edge.src, edge.dest);
            edgeCount++;
        }
    }

    // Valid MST should connect all nodes and have exactly n-1 edges
    return (edgeCount == n - 1);
}

// Exhaustive search function to find the MST
vector<Edge> exhaustiveSearchMST(const vector<Edge>& edges, int n) {
    int minWeight = INT_MAX;
    vector<Edge> bestMST;

    // There are 2^m subsets of edges where m is the number of edges
    int m = edges.size();
    for (int i = 1; i < (1 << m); ++i) {
        vector<Edge> subset;
        for (int j = 0; j < m; ++j) {
            if (i & (1 << j)) {
                subset.push_back(edges[j]);
            }
        }

        // Check if the subset is a valid spanning tree
        if (subset.size() == n - 1 && isValidSpanningTree(subset, n)) {
            // Calculate the total weight of the spanning tree
            int totalWeight = 0;
            for (const auto& edge : subset) {
                totalWeight += edge.weight;
            }
            
            // Update minimum weight and best MST if found a better one
            if (totalWeight < minWeight) {
                minWeight = totalWeight;
                bestMST = subset;
            }
        }
    }

    return bestMST;
}

// Function to add redundancy edges to meet reliability constraints
vector<Edge> addRedundancy(const vector<Edge>& allEdges, const vector<Edge>& mst, int k) {
    vector<Edge> mstEdges = mst;
    vector<Edge> unusedEdges;

    // Find edges that are not part of the MST
    for (const auto& edge : allEdges) {
        bool isInMST = false;
        for (const auto& mstEdge : mst) {
            if ((edge.src == mstEdge.src && edge.dest == mstEdge.dest) ||
                (edge.src == mstEdge.dest && edge.dest == mstEdge.src)) {
                isInMST = true;
                break;
            }
        }
        if (!isInMST) {
            unusedEdges.push_back(edge);
        }
    }

    // Sort the unused edges by weight to find the smallest k redundant edges
    sort(unusedEdges.begin(), unusedEdges.end(), [](const Edge& a, const Edge& b) {
        return a.weight < b.weight;
    });

    // Add the first k smallest unused edges for redundancy
    vector<Edge> redundantEdges;
    for (int i = 0; i < min(k, (int)unusedEdges.size()); ++i) {
        redundantEdges.push_back(unusedEdges[i]);
    }

    return redundantEdges;
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

    // Perform exhaustive search to find the MST
    vector<Edge> mst = exhaustiveSearchMST(edges, n);

    if (!mst.empty()) {
        cout << "Edges in the Minimum Spanning Tree:" << endl;
        for (const auto& edge : mst) {
            cout << edge.src << " - " << edge.dest << ": " << edge.weight << endl;
        }
        cout << "Total Weight of the Minimum Spanning Tree: " << calculateMSTWeight(mst) << endl;

        // Add redundancy edges to improve reliability (e.g., 2 redundant edges)
        int k = 2;
        vector<Edge> redundantEdges = addRedundancy(edges, mst, k);

        cout << "Additional Redundant Edges for Reliability:" << endl;
        for (const auto& edge : redundantEdges) {
            cout << edge.src << " - " << edge.dest << ": " << edge.weight << endl;
        }
    } else {
        cout << "No Minimum Spanning Tree found!" << endl;
    }

    return 0;
}
