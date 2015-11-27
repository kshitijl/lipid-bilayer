from gurobipy import *
from collections import defaultdict
import math

#n_verts = 8
#edges = [(0,6), (1,6), (2,6), (3,6), (4,6), (5,6), (7,6), (1,2), (2,3), (1,3), (4,7), (7,3)]

subsetSize = 150

width,height,depth = 9,9,6
n_verts = width*height*depth
edges = []

for i in range(width):
	for j in range(height):
            for k in range(depth):
                name1 = k + j*depth + i*depth*height
		if j < height-1:
                    name2 = k + (j+1)*depth + i*depth*height
                    edges.append((name1,name2))
		if i < width-1:
                    name2 = k + j*depth + (i+1)*depth*height
                    edges.append((name1,name2))
                if k < depth-1:
                    name2 = k+1 + j*depth + i*depth*height
                    edges.append((name1,name2))

nbrs_of = defaultdict(list)
edges_of = defaultdict(list)

for i in range(len(edges)):
    u,v = edges[i]
    nbrs_of[u].append(v)
    nbrs_of[v].append(u)
    edges_of[u].append(i)
    edges_of[v].append(i)

m = Model("DkS+1")

# x[i] = 1 if node i is selected to be red
x = {}
for i in range(n_verts):
    x[i] = m.addVar(vtype=GRB.BINARY, name="x"+str(i))

# y[(u,v)] = 1 if both u and v are red
y = {}
for i in range(len(edges)):
    y[i] = m.addVar(vtype=GRB.BINARY, name="y"+str(i), obj=1)

z = {}
for i in range(len(edges)):
    z[i] = m.addVar(vtype=GRB.BINARY, name="z"+str(i))

m.modelSense = GRB.MAXIMIZE

m.update()

# select exactly k nodes
m.addConstr(quicksum(x[i] for i in range(n_verts)) == subsetSize, "k items")

# at most one tail per vertex, every red must have a tail
for i in range(n_verts):
    m.addConstr(x[i] <= quicksum(z[e] for e in edges_of[i]) <= 1)

# an edge can EITHER be a tail OR it can be counted
for i in range(len(edges)):
    m.addConstr(y[i] + z[i] <= 1)

# for each edge, y[(u,v)] can be 1 only if both
# u and v are one (= red)
for i in range(len(edges)):
    u,v = edges[i]
    m.addConstr(y[i] <= x[u], "y0_"+str(i))
    m.addConstr(y[i] <= x[v], "y1_"+str(i))
    m.addConstr(y[i] >= x[u]+x[v]-1, "y2_"+str(i))

# at least one free space next to each node
for i in range(n_verts):
    m.addConstr(quicksum(x[v] for v in nbrs_of[i]) <= len(nbrs_of[i]) - x[i])
    m.addConstr(quicksum(y[e] for e in edges_of[i]) <= len(edges_of[i]) - 1)

# for each edge (u,v), sum(x[i] : i nbr of u or v) <= |{nbrs of u,v}| - 2 - 2*y[u,v]
for i in range(len(edges)):
    u,v = edges[i]
    nbrlist = list(set(nbrs_of[u] + nbrs_of[v]))
    m.addConstr(quicksum(x[j] for j in nbrlist if j != u and j != v) <= len(nbrlist) - 2*y[i])

# fix one guy
for i in range(2):
    for j in range(height):
        for k in range(2):
            m.addConstr(x[k + j*depth + i*depth*height] == 0)

for i in range(2):
    for j in range(2):
        for k in range(depth):
            m.addConstr(x[k + j*depth + i*depth*height] == 0)

for i in range(width):
    for j in range(2):
        for k in range(2):
            m.addConstr(x[k + j*depth + i*depth*height] == 0)

m.addConstr(x[2 + 2*depth + 2*depth*height] == 1)
m.addConstr(x[3 + 2*depth + 2*depth*height] == 1)

m.params.MIPFocus = 1
m.params.Symmetry = 2
m.params.ZeroHalfCuts = 2
m.params.GomoryCuts = 2
#m.params.BranchDir = 1
m.params.VarBranch = 0
#m.params.CliqueCuts = 2
#m.params.PreDual = 2
#m.params.PreSparsify = 1
#m.params.PreDepRow = 1
m.update()

m.optimize()

for v in m.getVars():
    if v.x > 0.1 and "x" in v.varName:
        print v.varName, v.x

print 'Obj:', m.objVal

def f(x):
	return 2*x - math.ceil(2*math.sqrt(x))

def g(x):
	return x + 2*f(x)

print "Predicted: ", g(subsetSize/2)
