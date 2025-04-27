import numpy as np
import matplotlib.pyplot as plt
def y(t):
    return t**(-1)
def f(t,y):
    return -5*t*(y**2) + 5/t - 1/(t**2)

def f_t(t,y):
    return -5*(y**2) - 5/(t**2) + 2/(t**3)

def f_y(t,y):
    return -10*t*y

def f_tt(t,y):
    return 10/(t**3) - 6/(t**4)

def f_ty(t,y):
    return -10*y

def taylor(y_n, t_n, h, f, f_t, f_y, f_tt, f_ty):
    return y_n + h*f(t_n,y_n) + (h**2)/(2) * (f_t(t_n,y_n) + f(t_n,y_n)*f_y(t_n,y_n))

def FE(y_n,t_n,h,f):
    return y_n + h*f(t_n,y_n)

x = np.arange(1,2.1,0.2)
output = y(x)
taylor_out = np.zeros_like(x)
fe_out = np.zeros_like(x)

taylor_out[0] = x[0]
fe_out[0] = x[0]
h = 0.2

for index, i in enumerate(x[:-1]):
    taylor_out[index+1] = taylor(taylor_out[index],i,h,f,f_t,f_y,f_tt,f_ty)
    fe_out[index+1] = FE(taylor_out[index],i,h,f)

print(taylor_out, fe_out, output, x)

plt.plot(x,taylor_out, label="taylor 2nd order")
plt.plot(x,fe_out, label = "FE")
plt.plot(x,output, label = "y(x)")
plt.legend()
plt.savefig("output.png")

