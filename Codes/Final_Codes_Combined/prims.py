from typing import List, Tuple
import heapq

# Edge structure
class Edge:
    def __init__(self, src: int, dest: int, weight: int):
        self.src = src
        self.dest = dest
        self.weight = weight

# Function to compute the MST using Prim's algorithm
def primMST(edges: List[Edge], n: int) -> List[Edge]:
    # Create an adjacency list for the graph
    adj = {i: [] for i in range(n)}
    for edge in edges:
        adj[edge.src].append((edge.weight, edge.dest))
        adj[edge.dest].append((edge.weight, edge.src))  # Undirected graph

    # Priority queue to select minimum weight edge
    pq = [(0, 0)]  # (weight, node) starting from node 0
    in_mst = [False] * n  # Track visited nodes
    mst = []  # Store edges in the MST
    total_edges = 0

    while pq and total_edges < n - 1:
        weight, node = heapq.heappop(pq)
        if in_mst[node]:
            continue

        in_mst[node] = True
        for neighbor_weight, neighbor in adj[node]:
            if not in_mst[neighbor]:
                heapq.heappush(pq, (neighbor_weight, neighbor))
                mst.append(Edge(node, neighbor, neighbor_weight))
                total_edges += 1

    return mst[:total_edges]  # Return only valid MST edges

# Function to calculate the total weight of the MST
def calculateMSTWeight(mst: List[Edge]) -> int:
    return sum(edge.weight for edge in mst)

# Function to add redundancy
def addRedundancy(edges: List[Edge], mst: List[Edge], n: int, k: int) -> List[Edge]:
    mst_edges_set = {(min(edge.src, edge.dest), max(edge.src, edge.dest)) for edge in mst}
    redundant_edges = [edge for edge in edges if (min(edge.src, edge.dest), max(edge.src, edge.dest)) not in mst_edges_set]

    # Sort redundant edges by weight
    redundant_edges.sort(key=lambda edge: edge.weight)
    return redundant_edges[:k]

# Function to process input and compute MSTs
def processInputFile(filename: str):
    with open(filename, "r") as file:
        lines = file.readlines()

    idx = 0
    graph_count = 1

    while idx < len(lines):
        # Skip empty lines
        if not lines[idx].strip():
            idx += 1
            continue

        # Read n (nodes) and k (redundant edges to add)
        try:
            n, k = map(int, lines[idx].strip().split())
            idx += 1
        except ValueError:
            print(f"Invalid graph header format: {lines[idx].strip()}")
            idx += 1
            continue

        # Read edges until the next header or end of file
        edges = []
        while idx < len(lines) and lines[idx].strip():
            try:
                src, dest, weight = map(int, lines[idx].strip().split())
                edges.append(Edge(src, dest, weight))
                idx += 1
            except ValueError:
                print(f"Invalid edge format: {lines[idx].strip()}")
                idx += 1
                continue

        # Compute MST
        mst = primMST(edges, n)
        total_weight = calculateMSTWeight(mst)

        # Add redundant edges
        redundant_edges = addRedundancy(edges, mst, n, k)

        # Display results
        print(f"Graph {graph_count}:")
        print("Edges in the Minimum Spanning Tree:")
        for edge in mst:
            print(f"{edge.src} - {edge.dest}: {edge.weight}")
        print(f"Total Weight of the Minimum Spanning Tree: {total_weight}")
        print("Additional Redundant Edges for Reliability:")
        for edge in redundant_edges:
            print(f"{edge.src} - {edge.dest}: {edge.weight}")
        print()
        graph_count += 1

# Main function
# if __name__ == "__main__":
#     input_file = "input.txt"  # Specify the input file name
#     try:
#         processInputFile(input_file)
#     except FileNotFoundError:
#         print(f"Error: The file '{input_file}' does not exist in the current directory.")
