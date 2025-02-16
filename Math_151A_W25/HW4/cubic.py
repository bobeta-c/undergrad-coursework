import numpy as np
from scipy.interpolate import CubicSpline

x = [0.1,0.2,0.3]
y = [-0.29004996, -0.56079734, -0.81401972]

cp = CubicSpline(x,y,bc_type = 'natural')
print(cp.c)
