import matplotlib as plt
import math

import numpy as np
import matplotlib.pyplot as plt

fp = 2*2*np.log(2) + 2
def fp_approx(h):
    return ((((2+h)**2)*(np.log(2+h)))-(4*np.log(2)))/h

n = 8
hs = []
errors = []
for m in range(200):
    h = 10**(-n-1 + 0.01*m )
    absolute_error = abs(fp-fp_approx(h))
    errors.append(absolute_error)
    hs.append(h)

print(hs)
print(errors)


plt.loglog(hs, errors)
plt.gca().invert_xaxis()
plt.xlabel("h values")
plt.ylabel("absolute error")
plt.title("part (b)")
plt.savefig("part_c.png")
