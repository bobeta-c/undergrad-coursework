l = [1,3,5,7,4,2,3,7,8,9]
n = len(l)
opt = [[0 for i in range(n)] for i in range(n)]
pick = [['0' for i in range(n)] for i in range(n)]
sig = [[0 for i in range(n)] for i in range(n)]

print(l)

for i in range(n):
    opt[i][i] = l[i]
    pick[i][i] = 'l'
    sig[i][i] = l[i]

for i in range(n):
    for j in range(n-i):
        sig[j][j+i] = sig[j][j+i-1] + l[j+i] 


for i in range(1,n):
    for j in range(n-i):
        # calculate opt[j][j+i]
        if opt[j+1][j+i] < opt[j][j+i-1]:
            pick[j][j+i] = 'l'
            opt[j][j+i] = sig[j][j+i] - opt[j+1][j+i]
        else:
            pick[j][j+i] = 'r'
            opt[j][j+i] = sig[j][j+i] - opt[j][j+i-1]

for i in range(n):
    for j in range(n):
        print(f"({opt[i][j]}, {pick[i][j]})", end = '')
    print()

print(opt[0][n-1])

i = 0
j = n-1
player = 0
while j-i >= 0:
    print(f"player: {player}, list {l[i:j+1]}, ", end = '')
    if(pick[i][j] == 'l'):
        print(f"{l[i]}")
        i += 1
    else:
        print(f"{l[j]}")
        j -= 1
    player = int(not player)

