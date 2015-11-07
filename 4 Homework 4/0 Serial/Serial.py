C = 11
coords = [ ( 1.1*(i**2 %17) , 0.5*(i**3 %23) ) for i in range(C)]

distance = {}
for i in range(C):
	distance[(i,i)] = 0
	p1,p2 = coords[i]
	for j in range(i):
		q1,q2 = coords[j]
		distance[(i,j)] = (abs(p1-q1)**1.5+abs(p2-q2)**1.5)**(2.0/3.0)
		distance[(j,i)] = distance[(i,j)]

paths = [(0.0,[0])]
bound = ([],"_")
while paths:
	dist, path = paths.pop()
	last = path[-1]
	unvisited = [x for x in range(C) if x not in path]
	if unvisited:
		for node in unvisited :
			newdist = dist + distance[(last,node)]
			if newdist <= bound[0]:
				paths.append((newdist,path+[node]))
	else:
		if dist < bound[0]:
			bound = (dist,path)


print coords
print

for i in range(C):
	for j in range(C):
		print "{0:0.2f}".format(distance[(i,j)]).zfill(5),
	print

print bound
best = bound[1]
total = 0.0
for i in range(len(best)-1):
	total += distance[(best[i],best[i+1])]
print total