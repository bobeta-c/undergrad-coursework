import matplotlib as plt
import math

import numpy as np
import matplotlib.pyplot as plt

fp = 2*2*np.log(2) + 2
def fp_approx(h):
    return ((((2+h)**2)*(np.log(2+h)))-(4*np.log(2)))/h

error = np.float64('inf')
n = 0
hs = []
errors = []
while True:
    absolute_error = abs(fp-fp_approx(10**(-n)))
    errors.append(absolute_error)
    hs.append(10**(-n))
    n += 1
    if absolute_error > error:
        break
    else:
        error = absolute_error

print(hs)
print(errors)


plt.loglog(hs, errors)
plt.gca().invert_xaxis()
plt.xlabel("h values")
plt.ylabel("absolute error")
plt.title("part (b)")
plt.savefig("part_b.png")
