import numpy as np
import matplotlib as plt

#========================================
# normal distribution

npts = 1000

mean = [0,0]
cov = [[1,0], [0,1]]
x,y = np.random.multivariate_normal(mean, cov, npts).T
plt.plot(x,y,'x'); plt.axis('equal'); plt.show()

mean = [1,1]
cov = [[1,0], [0,1]]
x,y = np.random.multivariate_normal(mean, cov, npts).T
plt.plot(x,y,'x'); plt.axis('equal'); plt.show()

mean = [0,0]
cov = [[2,0], [0,2]]
x,y = np.random.multivariate_normal(mean, cov, npts).T
plt.plot(x,y,'x'); plt.axis('equal'); plt.show()

mean = [0,0]
cov = [[1,0.5], [0.5,1]]
x,y = np.random.multivariate_normal(mean, cov, npts).T
plt.plot(x,y,'x'); plt.axis('equal'); plt.show()

mean = [0,0]
cov = [[1,-0.5], [-0.5,1]]
x,y = np.random.multivariate_normal(mean, cov, npts).T
plt.plot(x,y,'x'); plt.axis('equal'); plt.show()

#========================================
# eigenvalues

A = [[1,0],[1,3]]
w, v = np.linalg.eig(A)
print(w, '\n', v)