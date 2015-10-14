from random import randint
from math import log
# n = 3
# p = 2**n

# A = [randint(1,1000) for i in range(p)]
# print max(A)

def parallel_reducesum(A,verbose = False):
	p = len(A)
	n = int(log(p,2))
	i = 0
	if verbose:
		print " ".join([format(j,"0"+str(n)+"b") for j in range(p)])
		print " ".join(map(str,A))
	for neighbor in range(n):
		mask = 0x01 << neighbor
		# print format(mask,"0"+str(n)+"b")
		B = A[:]
		for pid in range(p):
			B[pid] = max(B[pid],B[pid^mask])
		A = B
		i+=1
		if verbose:
			print " ".join(map(str,A))
	if verbose:
		print " "
		print ("i = ",i,"p = ",p)
	return A


def test_reducesum(f,n=10,verbose=False):

	for i in range(1,n):
		p = 2**i
		if verbose:
			print "========="+str(i)+"========="
		A = [randint(10**(i-1),10**i) for j in range(p)]
		
		assert f(A,verbose) == [max(A) for j in range(p)]
		# print f(A)

if __name__ == '__main__':
	# test_reducesum(parallel_reducesum,10,True)

	for i in range(500):
		print "iteration",i," = OK"
		test_reducesum(parallel_reducesum,14,False)