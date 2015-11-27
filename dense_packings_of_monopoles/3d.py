import math

table = {}
def twodb(x):
    global table
    if x in table: return table[x]
    table[x] = int(math.floor(2*x - 2*math.sqrt(x)))
    return table[x]

def onedb(x):
    return x-1

T = {}

T[(1,1)] = 0

n = 1
while 1:
    for k in range(1,n+1):
        T[(n,k)] = twodb(k) + max([0] + [T[(n-k,i)] + min(i,k) for i in range(1,n-k+1)])
    print n,max([T[(n,j)] for j in range(1,n+1)]), 5*n/2 -  math.sqrt(n)
    n += 1

print max([T[(size-1,k)] for k in range(1,size)])