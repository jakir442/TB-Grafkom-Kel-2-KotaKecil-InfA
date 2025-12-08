import networkx as nx
import matplotlib.pyplot as plt

# ==============================
# 1. Membuat graf dan menambah edge
# ==============================
G = nx.Graph()

edges = [
    (1, 2, 10),
    (1, 4, 30),
    (1, 5, 45),

    (4, 6, 20),
    (4, 5, 25),

    (2, 5, 40),
    (2, 3, 50),
    (2, 6, 55),

    (5, 3, 35),
    (5, 6, 55),   # bobot 55 dari gambar
    (3, 6, 15)
]

for u, v, w in edges:
    G.add_edge(u, v, weight=w)

# ==============================
# 2. Menjalankan algoritma Kruskal
# ==============================
mst = nx.minimum_spanning_tree(G, algorithm='kruskal')

print("Edges MST (Kruskal):")
for u, v, data in mst.edges(data=True):
    print(f"{u} -- {v}  (w = {data['weight']})")

# ==============================
# 3. Posisi node (disesuaikan agar mirip gambar)
# ==============================
pos = {
    1: (0, 2),
    2: (2, 2),
    3: (4, 1.5),
    4: (0.2, 0.8),
    5: (2.2, 1),
    6: (1.2, 0)
}

# ==============================
# 4. Gambar graf asli
# ==============================
plt.figure(figsize=(10, 4))
plt.subplot(1, 2, 1)
plt.title("Graf Asli")
nx.draw(G, pos, with_labels=True, node_size=700)
labels = nx.get_edge_attributes(G, 'weight')
nx.draw_networkx_edge_labels(G, pos, edge_labels=labels)

# ==============================
# 5. Gambar MST (hasil Kruskal)
# ==============================
plt.subplot(1, 2, 2)
plt.title("Minimum Spanning Tree (Kruskal)")

# Semua edge digambar tipis
nx.draw(G, pos, alpha=0.3, with_labels=True, node_size=700)

# Edge MST digambar tebal dan berwarna biru
nx.draw_networkx_edges(
    mst, pos,
    edge_color="blue",
    width=3
)

# Tampilkan bobot MST
labels = nx.get_edge_attributes(mst, 'weight')
nx.draw_networkx_edge_labels(mst, pos, edge_labels=labels)

plt.tight_layout()
plt.show()
