import numpy as np
x = "10111"
i = len(x)
y = "0100"
j=len(y)
s = "100101110100101110100101110100101110100101110100101110100101110101111011110111"
k = len(s)

opt = np.zeros((k,i,j))


for q in range(k):
    for a in range(i):
        for b in range(j):
            if q == 0:
                if a == 0 and b == 1:
                    opt[0][a][b] = y[0] == s[0]
                elif a == 1 and b == 0:
                    opt[0][a][b] = x[0] == s[0]
                else:
                    opt[0][a][b] = False
            else:
                opt[q][a][b] = (opt[q-1][a-1][b] and s[q] == x[a-1]) or (opt[q-1][a][b-1]
                                    and s[q] == y[b-1])
print(opt[k-1].any())
