import matplotlib.pyplot as plt
import numpy as np
mean = np.array((0.0,0.0))
cov = np.array(((1.0,0.5),(0.5,1.0)))

output = np.random.multivariate_normal(mean,cov, size = 1000)

plt.plot(output[:,0], output[:,1], '.', alpha=0.5)
plt.axis('equal')
plt.grid()
plt.savefig('9-d.png')
