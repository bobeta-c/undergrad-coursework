x = "101"
y = "00"
s = "100010101"

val = [[-1 for i in range(len(s) )], [-1 for i in range(len(s))]]
val[0][0] = 0 if x[0] == s[0] else -1
val[1][0] = 0 if y[0] == s[0] else -1
print(val)

for i, c in enumerate(s):
    if i == 0:
        continue

