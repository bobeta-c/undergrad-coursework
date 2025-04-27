import numpy as np

nodes = np.array([
    [2,6],[2,7],[3,7],[4,8],
    [5,1],[5,4],[5,9],[7,2],
    [7,3],[8,3],[8,4],[8,8]
])

A = np.array([
    [1,5],  # a1
    [4,7],  # a2
    [5,5],  # a3
    [6,2],  # a4
    [9,5]   # a5
])

# Normalize nodes and A for cosine similarity
node_norms = np.linalg.norm(nodes, axis=1)
A_norms = np.linalg.norm(A, axis=1)

# Compute cosine similarity matrix: shape (5, 12)
# Each row corresponds to one a vector, each column to one node
dot_products = A @ nodes.T                   # shape (5, 12)
cos_sim_matrix = dot_products / (A_norms[:, None] * node_norms[None, :])

# Find most similar node for each a vector
best_indices = np.argmax(cos_sim_matrix, axis=1)

# Output
for i, idx in enumerate(best_indices, start=1):
    print(f"Best match for a{i} is node[{idx}] with cosine similarity {cos_sim_matrix[i-1, idx]:.4f}")

# Optionally: print the full similarity matrix
print("\nFull cosine similarity matrix (rows = a vectors, cols = nodes):")
print(cos_sim_matrix)
