#include <iostream>   // for input-output operations
#include <vector>     // for storing edges and other lists
#include <algorithm>  // for sorting edges
#include <climits>    // for using INT_MAX (maximum integer value)

using namespace std;  // for easier access to standard library functions

// Structure to represent an edge with source, destination, and weight
struct Edge {
    int src, dest, weight;
};

// Class to implement Union-Find (Disjoint Set) to check connectivity between nodes
class UnionFind {
    vector<int> parent, rank;  // 'parent' keeps track of root nodes, 'rank' helps optimize merging

public:
    // Constructor to initialize Union-Find with 'n' elements
    UnionFind(int n) : parent(n), rank(n, 0) {
        // Each node is its own parent initially
        for (int i = 0; i < n; ++i) parent[i] = i;
    }

    // Function to find the root of a node with path compression
    int find(int u) {
        if (parent[u] != u) {
            parent[u] = find(parent[u]);  // Recursively find the root and compress path
        }
        return parent[u];
    }

    // Function to union two sets (merge two components)
    void unionSets(int u, int v) {
        int rootU = find(u);  // Find root of u
        int rootV = find(v);  // Find root of v

        if (rootU != rootV) {  // If they are in different components
            // Merge based on rank to keep the tree flat
            if (rank[rootU] > rank[rootV]) parent[rootV] = rootU;
            else if (rank[rootU] < rank[rootV]) parent[rootU] = rootV;
            else {
                parent[rootV] = rootU;
                ++rank[rootU];  // Increase rank if both components have the same rank
            }
        }
    }
};

// Function to check if a subset of edges forms a valid Minimum Spanning Tree (MST)
bool isValidSpanningTree(const vector<Edge>& edges, int n) {
    UnionFind uf(n);  // Create Union-Find structure for 'n' nodes
    int edgeCount = 0;  // Counter to track the number of edges in the spanning tree

    // Try to add each edge to the spanning tree
    for (const auto& edge : edges) {
        // If the edge connects two different components, add it to the spanning tree
        if (uf.find(edge.src) != uf.find(edge.dest)) {
            uf.unionSets(edge.src, edge.dest);
            edgeCount++;
        }
    }

    // A valid MST for 'n' nodes must have exactly 'n-1' edges
    return (edgeCount == n - 1);
}

// Function to find the MST using an exhaustive search approach
vector<Edge> exhaustiveSearchMST(const vector<Edge>& edges, int n) {
    int minWeight = INT_MAX;  // Store the minimum weight found so far
    vector<Edge> bestMST;  // Store the best MST found

    int m = edges.size();  // Total number of edges

    // Iterate over all subsets of edges (2^m possibilities)
    for (int i = 1; i < (1 << m); ++i) {
        vector<Edge> subset;  // Current subset of edges

        // Check which edges are included in the current subset
        for (int j = 0; j < m; ++j) {
            if (i & (1 << j)) {  // If j-th bit is set, include the edge
                subset.push_back(edges[j]);
            }
        }

        // Check if the subset forms a valid MST
        if (subset.size() == n - 1 && isValidSpanningTree(subset, n)) {
            // Calculate the total weight of the current MST
            int totalWeight = 0;
            for (const auto& edge : subset) {
                totalWeight += edge.weight;
            }

            // Update the best MST if the current one has a lower weight
            if (totalWeight < minWeight) {
                minWeight = totalWeight;
                bestMST = subset;
            }
        }
    }

    return bestMST;  // Return the best MST found
}

// Function to add redundant edges to improve reliability
vector<Edge> addRedundancy(const vector<Edge>& allEdges, const vector<Edge>& mst, int k) {
    vector<Edge> unusedEdges;  // Store edges that are not part of the MST

    // Find edges that are not in the MST
    for (const auto& edge : allEdges) {
        bool isInMST = false;
        for (const auto& mstEdge : mst) {
            // Check if the edge is already in the MST (ignoring direction)
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

    // Sort the unused edges by weight (ascending order)
    sort(unusedEdges.begin(), unusedEdges.end(), [](const Edge& a, const Edge& b) {
        return a.weight < b.weight;
    });

    // Add the first 'k' smallest unused edges for redundancy
    vector<Edge> redundantEdges;
    for (int i = 0; i < min(k, (int)unusedEdges.size()); ++i) {
        redundantEdges.push_back(unusedEdges[i]);
    }

    return redundantEdges;  // Return the redundant edges
}

// Function to calculate the total weight of an MST
int calculateMSTWeight(const vector<Edge>& mst) {
    int totalWeight = 0;
    for (const auto& edge : mst) {
        totalWeight += edge.weight;
    }
    return totalWeight;  // Return the total weight
}

int main() {
    int n = 5;  // Number of nodes in the graph

    // List of all edges with their source, destination, and weight
    vector<Edge> edges = {
        {0, 1, 10}, {0, 2, 6}, {0, 3, 5},
        {1, 3, 15}, {2, 3, 4}, {1, 2, 7}
    };

    // Find the MST using exhaustive search
    vector<Edge> mst = exhaustiveSearchMST(edges, n);

    if (!mst.empty()) {
        cout << "Edges in the Minimum Spanning Tree:" << endl;
        for (const auto& edge : mst) {
            cout << edge.src << " - " << edge.dest << ": " << edge.weight << endl;
        }
        cout << "Total Weight of the Minimum Spanning Tree: " << calculateMSTWeight(mst) << endl;

        // Add redundant edges to improve reliability (e.g., 2 redundant edges)
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
