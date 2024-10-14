#include <iostream>   // For input and output
#include <vector>     // For using vector to store edges
#include <cstdlib>    // For rand() function
#include <ctime>      // For seeding random number generator

using namespace std;

// Structure to represent an edge with source, destination, and weight
struct Edge {
    int src, dest, weight;
};

// Function to generate random graph edges
vector<Edge> generateRandomGraph(int n, int numEdges, int minWeight, int maxWeight) {
    vector<Edge> edges;  // Store all generated edges
    srand(time(0));  // Seed the random number generator with the current time

    // Loop to generate the required number of edges
    for (int i = 0; i < numEdges;) {
        int src = rand() % n;   // Random source node
        int dest = rand() % n;  // Random destination node

        // Make sure the edge is valid (no self-loops)
        if (src != dest) {
            int weight = minWeight + rand() % (maxWeight - minWeight + 1);  // Random weight
            edges.push_back({src, dest, weight});  // Add edge to the list
            i++;  // Only increment if we added a valid edge
        }
    }
    return edges;  // Return the generated edges
}

// Function to print the edges of the graph
void printGraph(const vector<Edge>& edges) {
    cout << "Generated Graph Edges:" << endl;
    for (const auto& edge : edges) {
        cout << edge.src << " - " << edge.dest << ": " << edge.weight << endl;
    }
}

int main() {
    int n, numEdges, minWeight, maxWeight;

    // Ask the user for the number of nodes and edges
    cout << "Enter the number of nodes: ";
    cin >> n;
    cout << "Enter the number of edges: ";
    cin >> numEdges;

    // Ask for the range of weights
    cout << "Enter the minimum edge weight: ";
    cin >> minWeight;
    cout << "Enter the maximum edge weight: ";
    cin >> maxWeight;

    // Validate the input
    if (numEdges > n * (n - 1) / 2) {  // Too many edges for the given nodes
        cout << "Error: Too many edges for the given number of nodes." << endl;
        return 1;  // Exit the program
    }

    if (minWeight > maxWeight) {  // Invalid weight range
        cout << "Error: Minimum weight cannot be greater than maximum weight." << endl;
        return 1;  // Exit the program
    }

    // Generate the random graph
    vector<Edge> edges = generateRandomGraph(n, numEdges, minWeight, maxWeight);

    // Print the generated graph
    printGraph(edges);

    // Program finished successfully
    return 0;
}
