from collections import defaultdict

def app(s):
	return lambda z: s+str(z)

width,height = 20,20
n_verts = width*height
edges = []
numba = 10

for i in range(width):
	for j in range(height):
		if j < height-1:
		   edges.append((i*height+j,i*height+j+1))
		if i < width-1:
		   edges.append((i*height+j,(i+1)*height + j))

#n_verts = 8
#numba = 4
#edges = [(0,6), (1,6), (2,6), (3,6), (4,6), (5,6), (7,6), (1,2), (2,3), (1,3), (4,7), (7,3)]

nejjes = defaultdict(list)

for i in range(len(edges)):
	u,v = edges[i]
	nejjes[u].append(i)
	nejjes[v].append(i)

yvars = map(app("y"), range(len(edges)))

print "MAXIMIZE"
print "obj:", " + ".join(yvars)
print "\nSUBJECT TO"

n_c = 0

print "r"+str(n_c) + ":", " + ".join(map(app("x"), range(n_verts))), " = "+str(numba)
n_c += 1

for k,incidents in nejjes.items():
	print "r"+str(n_c) + ":", " + ".join(map(app("z"), incidents)), " <= 1"
	n_c += 1

for k,incidents in nejjes.items():
	print "r"+str(n_c) + ":", " + ".join(map(app("z"), incidents)), " -", "x"+str(k), ">= 0"
	n_c += 1

for k,incidents in nejjes.items():
	print "r"+str(n_c) + ":", " + ".join(map(app("y"), incidents)) + " <= " + str(len(incidents)-1)
	n_c += 1

for i in range(len(edges)):
	u,v = edges[i]
	print "r"+str(n_c) + ":", "y"+str(i)+" - x" + str(u) + " <= 0"
	print "r"+str(n_c+1) + ":", "y"+str(i)+" - x" + str(v) + " <= 0"
	print "r"+str(n_c+2) + ":", "y"+str(i)+" - x" + str(v) + " - x" + str(u) + " >= -1"
	n_c += 3
	
for i in range(len(edges)):
	print "r"+str(n_c) + ":", "z" + str(i) + " + y" + str(i) + " <= 1"
	n_c += 1

print "\nBINARY"

print "\n".join(map(app("y"), range(len(edges))))
print "\n".join(map(app("z"), range(len(edges))))
print "\n".join(map(app("x"), range(n_verts)))

print "\nEND"