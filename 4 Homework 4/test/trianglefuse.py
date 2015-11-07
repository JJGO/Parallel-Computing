import numpy as np
from math import sin,cos
C = 15
distance = np.zeros(shape=(C,C))
# coords = [ ( 1.1*(i**2 %17) , 0.5*(i**3 %23) ) for i in range(C)]
coords = [ ( 100.0*sin(i) , 101.0*cos(i*i) ) for i in range(C)]
for z in range(C*(C+1)/2):
	i,j = z/C, z%C
	if i == j:
		distance[i][i] = 0
	else:
		if i > j:
			i,j = C-i,C-1-j
		p1,p2 = coords[i]
		q1,q2 = coords[j]
		distance[i][j] = (abs(p1-q1)**1.5+abs(p2-q2)**1.5)**(2.0/3.0)
		distance[j][i] = (abs(p1-q1)**1.5+abs(p2-q2)**1.5)**(2.0/3.0)

for i in range(C):
	for j in range(C):
		print "{0:0.2f}".format(distance[(i,j)]).zfill(5),
	print
	
print sum(sum(distance))