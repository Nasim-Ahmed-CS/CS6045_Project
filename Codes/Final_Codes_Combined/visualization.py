import networkx as nx
import matplotlib.pyplot as plt

def compute_and_visualize_mst(edges, n, k, case_number):
    G = nx.Graph()
    G.add_weighted_edges_from(edges)
    mst_edges = list(nx.minimum_spanning_edges(G, algorithm="kruskal", data=True))
    mst_G = nx.Graph()
    mst_G.add_weighted_edges_from([(u, v, d["weight"]) for u, v, d in mst_edges])
    mst_edge_set = set((u, v) if u < v else (v, u) for u, v, d in mst_edges)
    redundant_edges = [
        (u, v, w)
        for u, v, w in edges
        if (u, v) not in mst_edge_set and (v, u) not in mst_edge_set
    ]
    redundant_edges = sorted(redundant_edges, key=lambda x: x[2])[:k]
    redundant_G = nx.Graph()
    redundant_G.add_weighted_edges_from(redundant_edges)
    pos = nx.spring_layout(G, seed=42)

    plt.figure(figsize=(12, 6))
    plt.subplot(1, 2, 1)
    nx.draw(G, pos, with_labels=True, node_color="lightblue", edge_color="gray", node_size=700, font_weight="bold")
    nx.draw_networkx_edge_labels(G, pos, edge_labels={(u, v): w for u, v, w in edges})
    plt.title(f"Original Graph (Case {case_number})")

    plt.subplot(1, 2, 2)
    nx.draw(G, pos, with_labels=True, node_color="lightblue", edge_color="lightgray", node_size=700, font_weight="bold")
    nx.draw_networkx_edges(mst_G, pos, edge_color="green", width=2, label="MST")
    nx.draw_networkx_edges(redundant_G, pos, edge_color="red", style="dashed", width=2, label="Redundant")
    nx.draw_networkx_edge_labels(G, pos, edge_labels={(u, v): w for u, v, w in edges})
    plt.legend(["MST", "Redundant"], loc="upper left")
    plt.title(f"MST and Redundant Edges (Case {case_number})")
    plt.tight_layout()
    plt.show()

def process_input_file(file_path):
    with open(file_path, 'r', encoding='utf-8') as file:
        lines = file.readlines()

    case_number = 0
    edges = []
    n, k = 0, 0
    reading_edges = False  # Flag to differentiate between n, k and edges

    for line in lines:
        line = line.strip()  # Remove leading/trailing whitespace, including '\n'
        if not line:  # Blank line separates test cases
            if edges:  # Process the accumulated edges
                case_number += 1
                compute_and_visualize_mst(edges, n, k, case_number)
                edges = []  # Reset for the next case
                reading_edges = False
            continue

        if not reading_edges:  # First non-blank line after a blank line
            try:
                n, k = map(int, line.split())
                reading_edges = True
            except ValueError:
                print(f"Invalid format for n, k in case {case_number + 1}: {repr(line)}")
                return
        else:  # Reading edges
            try:
                u, v, w = map(int, line.split())
                edges.append((u, v, w))
            except ValueError:
                print(f"Invalid format for edge in case {case_number + 1}: {repr(line)}")
                return

    # Process the last test case if any
    if edges:
        case_number += 1
        compute_and_visualize_mst(edges, n, k, case_number)

# Check the file and run
