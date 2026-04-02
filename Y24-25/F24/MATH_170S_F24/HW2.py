x=[5.28,5.40,4.65,4.76,5.55,5.73,5.84,4.97,5.50,6.24]
y=[52.36,52.58,51.07,52.28,53.02,52.10,52.61,52.21,52.39,53.77]

xavg = sum(x)/len(x)
yavg = sum(y)/len(y)

cov = 0
for i in range(len(x)):
    cov += (x[i]-xavg)*(y[i]-yavg)

xvar = 0
for i in range(len(x)):
    xvar += (x[i]-xavg)**2

beta = cov/xvar

alpha = yavg - beta*xavg

print(f"alpha = {alpha}, beta = {beta}")
