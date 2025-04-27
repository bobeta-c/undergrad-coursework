import numpy as np
a = np.array(((1, 0),(1,3)))
values, vectors = np.linalg.eig(a)
i = np.argmax(values)
print(vectors[:,i])
