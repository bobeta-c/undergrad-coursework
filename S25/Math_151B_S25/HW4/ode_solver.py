import scipy
import matplotlib.pyplot as plt
import numpy as np

def velocity_field(t, y):
    """
    Compute the velocity field for the restricted three-body problem.

    Parameters:
        t : float
            Time (not used explicitly here since the system is autonomous).
        y : ndarray
            4D vector: [x, x_dot, y, y_dot]

    Returns:
        f : ndarray
            4D vector of derivatives [x_dot, x_ddot, y_dot, y_ddot]
    """
    mu = 0.012277471
    mu0 = 1 - mu

    x, x_dot, y_pos, y_dot = y

    r1 = np.sqrt((x + mu)**2 + y_pos**2)
    r2 = np.sqrt((x - mu0)**2 + y_pos**2)

    f = np.zeros(4)
    f[0] = x_dot
    f[1] = 2*y_dot + x - (mu0 * (x + mu)) / r1**3 - (mu * (x - mu0)) / r2**3
    f[2] = y_dot
    f[3] = - 2*x_dot + y_pos - (mu0 * y_pos) / r1**3 - (mu * y_pos) / r2**3

    return f
T = 17.1
y0 = np.array([0.994, 0.0, 0.0, -2.00158510637908252240537862224])
out = scipy.integrate.solve_ivp(velocity_field, (0,T),y0,'RK45',rtol = 1e-6)

mu = 0.012277471
mu0 = 1 - mu

plt.figure(figsize=(8, 6))
plt.plot(mu, 0, 'ro', markersize=8, label='Planet A')
plt.plot(mu0, 0, 'bo', markersize=8, label='Planet B')
plt.plot(out.y[0], out.y[2], 'k-', linewidth=1.5, label='Trajectory')
plt.xlabel('x')
plt.ylabel('y')
plt.title('Trajectory of the planet using ODE solver RK45')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("ode45.png")
plt.show()
print(len(out.t))
