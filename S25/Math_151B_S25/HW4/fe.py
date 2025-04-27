import numpy as np
import matplotlib.pyplot as plt

def velocity_field(t, y):
    return -100 * (y- np.sin(t))

# Initial condition
y0 = 1

# Simulation parameters
T = 1
N = 11  # You can adjust this
tvec = np.linspace(0, T, N)
h = tvec[1] - tvec[0]

# Storage for results
y_stored = np.zeros(N)
y_stored[0] = y0
y = y0

for n in range(1,N):
    t = tvec[n]
    y += h * velocity_field(t,y)
    y_stored[n] = y

# Plotting the results

plt.figure(figsize=(8, 6))
plt.plot(tvec, y_stored, 'k-', linewidth=1.5, label='Trajectory')
plt.xlabel('x')
plt.ylabel('y')
plt.title(f'numerical approximation using step size: {h}')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig(f"FE_Step{h}.png")
plt.show()

