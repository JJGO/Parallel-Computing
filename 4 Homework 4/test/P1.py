from matplotlib import pyplot
from math import sin,cos
from math import pi
Cs = []
Cs.append([0, 13, 9, 1, 8, 7, 2, 3, 4, 11, 6, 10, 12, 5])
Cs.append([0, 13, 9, 1, 8, 14, 7, 2, 3, 4, 11, 6, 10, 12, 5])
Cs.append([0, 13, 9, 15, 7, 1, 8, 14, 2, 3, 4, 11, 6, 10, 12, 5])
Cs.append(range(100))

for C in Cs:
	labels = map(str,C)
	X = map(lambda x: 100.0*sin(x),C)
	Y = map(lambda x: 101.0*cos(x*x),C)
	# xoffsets = [-3,+3,+5,5,10,7,7,-75]
	# yoffsets = [-30,-20,-10,-7,-4,-10,-10,0]
	# xoffsets = [10,10,10,10,10,10,10,10,10,-10]
	# yoffsets = [-5,-5,-5,-5,-5,-5,-5,-5,-5,-15]
	# xoffsets = [-50,-92,-87,-83,-80,-90,-65,-73]
	# yoffsets = [-20,-10,-2,0,3,0,0,0]
	xoffsets = [0]*len(C)
	yoffsets = [0]*len(C)
	fig = pyplot.figure()

	pyplot.plot(X,Y, 'ro-')
	pyplot.title(' C = '+str(len(C)))
	pyplot.xlabel('X')
	pyplot.ylabel('Y')
	# pyplot.yscale('log')
	# pyplot.xscale('log')


	for label,x,y,xoffset,yoffset in zip(labels,X,Y,xoffsets,yoffsets):
	    pyplot.annotate(label, xy=(x,y), textcoords='offset points',xytext = (xoffset, yoffset))

	pyplot.savefig('C%d.png' % len(C))
	# pyplot.show()


