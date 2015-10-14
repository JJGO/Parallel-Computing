from math import *

IT = 1


for N in [7]:#[1000,2000,4000]:
	A = [[0]*N for i in range(N)]
	B = [[0]*N for i in range(N)]
	print "============== "+str(N)+ " =============="
	for i in range(N):
		for j in range(N):
			if i == j:
				A[i][j] = i*sin(sqrt(i))
			else:
				A[i][j] = (i+j)**1.1

	for it in range(IT):
		for i in range(N):
			ip = (i+1) % N
			ipp = (i-1) % N
			for j in range(N-1):
				if i != j:
					x = 0
					for k in range(1,11):
						x += abs(0.5 + A[ip][j])**(1.0/k) - abs(A[ipp][j])**(1.0/(k+1)) * abs(A[i][j+1])**(1.0/(k+2))
					B[i][j] = max(-10,min(10,x))
				else:
					B[i][j] = A[i][j]

		for i in range(N):
			for j in range(N-1):
				A[i][j] = B[i][j]

	minA = A[0][0]
	sumA = 0

	for i in range(N):
			for j in range(N):
				sumA += abs(A[i][j])
				minA = min(minA,A[i][j])
				print "{0:.5f}".format( A[i][j] ),
			print
	print
	print "Sum: {0}\nMin: {1}".format(sumA,minA);
	print


