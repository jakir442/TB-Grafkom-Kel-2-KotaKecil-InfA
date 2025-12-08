import networkx as nx
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# ============================
# Data graf (sesuai gambar kamu)
# ============================
edges = [
    (1, 2, 10),
    (3, 6, 15),
    (4, 6, 20),
    (4, 5, 25),
    (1, 4, 30),
    (5, 3, 35),
    (2, 5, 40),
    (1, 5, 45),
    (2, 3, 50),
    (5, 6, 55),
    (2, 6, 55)
]

G = nx.Graph()
for u, v, w in edges:
    G.add_edge(u, v, weight=w)

# ============================
# Posisi node
# ============================
pos = {
    1: (0, 2),
    2: (2, 2),
    3: (4, 1.5),
    4: (0.2, 0.8),
    5: (2.2, 1),
    6: (1.2, 0)
}

# ============================
# Kruskal Manual
# ============================
parent = {}
rank = {}

def make_set(v):
    parent[v] = v
    rank[v] = 0

def find(v):
    if parent[v] != v:
        parent[v] = find(parent[v])
    return parent[v]

def union(u, v):
    root1 = find(u)
    root2 = find(v)
    if root1 != root2:
        if rank[root1] > rank[root2]:
            parent[root2] = root1
        else:
            parent[root1] = root2
            if rank[root1] == rank[root2]:
                rank[root2] += 1

for node in G.nodes():
    make_set(node)

# Urutkan edge
sorted_edges = sorted(G.edges(data=True), key=lambda x: x[2]['weight'])

steps = []   # untuk animasi

mst_edges = []
rejected_edges = []

for u, v, data in sorted_edges:
    w = data['weight']
    if find(u) != find(v):
        union(u, v)
        mst_edges.append((u, v))
        steps.append(("accept", (u, v), w))
    else:
        rejected_edges.append((u, v))
        steps.append(("reject", (u, v), w))

# ============================
# Animasi
# ============================
fig, ax = plt.subplots(figsize=(8, 5))

def update(frame):
    ax.clear()
    ax.set_title(f"Langkah {frame + 1}: Kruskal")

    # Gambar graph dasar
    nx.draw(G, pos, with_labels=True, node_size=700, edge_color="lightgray", ax=ax)
    nx.draw_networkx_edge_labels(G, pos, ax=ax, 
        edge_labels=nx.get_edge_attributes(G, 'weight'))

    step_type, (u, v), w = steps[frame]

    # Edge-edge yg sudah diterima sebelumnya digambar biru
    accepted_so_far = [
        steps[i][1] for i in range(frame)
        if steps[i][0] == "accept"
    ]
    nx.draw_networkx_edges(G, pos, edgelist=accepted_so_far, width=3, edge_color="blue", ax=ax)

    # Edge sedang dianimasikan
    color = "blue" if step_type == "accept" else "red"
    nx.draw_networkx_edges(G, pos, edgelist=[(u, v)], width=4, edge_color=color, ax=ax)

    # Keterangan
    if step_type == "accept":
        text = f"✓ Ditambahkan ke MST: {u} -- {v} (bobot {w})"
    else:
        text = f"✗ Ditolak (membuat cycle): {u} -- {v} (bobot {w})"

    ax.text(0.1, -0.1, text, transform=ax.transAxes, fontsize=12)

ani = FuncAnimation(fig, update, frames=len(steps), interval=1500, repeat=True)

plt.show()
