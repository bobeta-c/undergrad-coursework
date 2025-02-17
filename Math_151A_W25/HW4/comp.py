import numpy as np
from scipy.interpolate import CubicSpline
from scipy.interpolate import lagrange
import matplotlib.pyplot as plt

def f(x):
    return 1 / (1 + 25 * x**2)


x_vals = np.linspace(-1,1,800)
y_vals_f = f(x_vals)

n_values = [7,11,20] # interpolation points

fig, axs = plt.subplots(3,1,figsize=(8,12))
titles = ["Lagrange (Equispaced)", "Lagrage (Chebyshev)", "Cubic Spline"]
colors = ["r","g","b"]


for i,n in enumerate(n_values):
    #Equispaced points
    x_equi_space = np.linspace(-1,1,n)
    y_equi_space = f(x_equi_space)

    #Lagrange with equispaced points
    lagrange_equi = lagrange(x_equi_space,y_equi_space)
    y_lagrange_equi = lagrange_equi(x_vals)

    # Lagrange with Chebyshev nodes
    x_cheby = np.cos([((2*k+1)/(2*n))*np.pi for k in range(0,n)])
    y_cheby = f(x_cheby)
    lagrange_cheby = lagrange(x_cheby,y_cheby)
    y_lagrange_cheby = lagrange_cheby(x_vals)

    #cubic Spline with equispaced points
    cubic_equi = CubicSpline(x_equi_space,y_equi_space, bc_type = ((1, 0.0739644970414),(1,-0.0739644970414)))
    y_cubic = cubic_equi(x_vals)

    #Plot for each interpolation method
    axs[0].plot(x_vals, y_vals_f, 'k', label="F(x)" if i == 0 else "")
    axs[0].plot(x_vals, y_lagrange_equi, color=colors[i], label=f"n={n}")

    axs[1].plot(x_vals, y_vals_f, 'k', label="F(x)" if i == 0 else "")
    axs[1].plot(x_vals, y_lagrange_cheby, color=colors[i], label=f"n={n}")

    axs[2].plot(x_vals, y_vals_f, 'k', label="F(x)" if i == 0 else "")
    axs[2].plot(x_vals, y_cubic, color=colors[i], label=f"n={n}")

for i, ax in enumerate(axs):
    ax.set_title(titles[i])
    ax.set_xlabel("x")
    ax.grid(True)
    ax.legend()

plt.tight_layout()

plt.savefig("multi-interpolant.png")

plt.show()
