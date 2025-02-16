import matplotlib as plt
import math

import numpy as np
import matplotlib.pyplot as plt

def P12(x):
    return 0
def P14(x):
    if 0 <= x < 1/4:
        return 4*x
    elif 1/4 <= x < 3/4:
        return -4*x + 2
    elif 3/4 <= x <= 1:
        return 4*x - 4
    else:
        return 0# Outside the domain

def P18(x):
    if 0 <= x < 1/8:
        return x * (8/ (2 ** 0.5))
    elif 1/8 <= x < 1/4:
        return (x-1/8)*8*(1 - (1/(2**0.5))) + 1/(2**0.5)
    elif 1/4 <= x < 3/8:
        return -(x-3/8)*8*(1 - (1/(2**0.5))) + 1/(2**0.5)
    elif 3/8 <= x <= 5/8:
        return -(x-1/2)*(8/(2**.5))
    elif 5/8 <= x < 3/4:
        return -(x-5/8)*8*(1 - (1/(2**0.5))) - 1/(2**0.5)
    elif 3/4 <= x < 7/8:
        return (x-7/8)*8*(1 - (1/(2**0.5))) - 1/(2**0.5)
    elif 7/8 <= x < 1:
        return (x-7/8)*(8/(2**.5)) - (1/(2** 0.5))


def F(x):
    return math.sin(2*math.pi*x)

x_vals = np.linspace(0, 1, 400)
p12_vals = np.array([P12(x) for x in x_vals])
p14_vals = np.array([P14(x) for x in x_vals])
F_vals = np.array([F(x) for x in x_vals])
p18_vals = np.array([P18(x) for x in x_vals])

plt.plot(x_vals, p14_vals, label="$P14(x)$", color='b')
plt.plot(x_vals, p12_vals, label="$P12(x)$", color='r')
plt.plot(x_vals, F_vals, label="$f(x)$", color='g')
plt.plot(x_vals, p18_vals, label="$P18(x)$", color='m')
plt.xlabel("x")
plt.ylabel("P14(x)")
plt.title("Plot of the Piecewise Function $P_{1,4}(x)$")
plt.grid(True)
plt.legend()

plt.savefig("plot.png", dpi = 300)
plt.show()
