from math import log,ceil

def parallel_scanor(A):
	p = len(A)
	j = 0
	

	while 2**j < p:
		B = A[:]
		for ID in range(p):
			if ID + 2**j < p:
				B[ID+2**j] = B[ID+2**j] or A[ID]
		# print j
		# print "A",A
		# print "B",B
		# print
		A = B[:]
		j+=1
	return (A,j)


def test_scanor(n=20):
	for i in range(1,n+1):
		print "========="+str(i)+"========="
		for j in range(2**i):
			data = [x == '1' for x in bin(j)[2:]]
			data = [False]*(i-len(data)) + data
			try:
				pos = data.index(True)
			except:
				pos = i
			solution = [False]*pos+[True]*(i-pos)
			it = ceil(log(i,2))
			assert parallel_scanor(data) == (solution,it)
			print it
			print data
			print parallel_scanor(data)[0]

	return True

test_scanor(8)
