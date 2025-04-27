# %%

import numpy as np
data = np.array(((4,0),(1,1),(0,1),(-2,-2),(-2,1),(1,0),(5,2),(3,0)))
labels = np.array((1,-1,-1,1,-1,1,-1,-1))
# %%

def voted(x):
    w1 = np.array(((4,0),))
    w2 = np.array(((3,-1),))
    w3 = np.array(((1,-3),))
    w4 = np.array(((-2,-3),))
    return np.sign(2*np.sign(x @ w1.T)
                    + 2*np.sign(x @ w2.T)
                      + 4*np.sign(x @ w3.T)
                        + np.sign(x @ w4.T)
                        )


def average(x):
    w1 = np.array(((4,0),))
    w2 = np.array(((3,-1),))
    w3 = np.array(((1,-3),))
    w4 = np.array(((-2,-3),))
    return np.sign(2*(x @ w1.T)
                    + 2*(x @ w2.T)
                      + 4*(x @ w3.T)
                        + (x @ w4.T)
                        )
# %%

out = average(data).reshape(-1)
print(out)
print(out == labels)

# %%
