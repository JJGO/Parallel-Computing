def printsq(M):
		for sublist in M:
			print sublist
		print
def mesh_scanor(A):
	p = len(A)
	s = int(p**0.5)
	M = [A[i*s:i*s+s] for i in range(s)]
	

	printsq(M)
	for j in range(s-1):
		for i in range(s):
			M[i][j+1] = M[i][j+1] or M[i][j]
	printsq(M)

	for i in range(s-1):
		M[i+1][s-1] = M[i+1][s-1] or M[i][s-1]
	printsq(M)

	for j in range(s-1,0,-1):
		for i in range(1,s):
			M[i][j-1] = M[i][j-1] or M[i-1][s-1]
	printsq(M)


	return [item for sublist in M for item in sublist]
	


def test_scanor(n=20):
	for i in range(1,n+1):
		k = i**2
		print "========="+str(k)+"========="
		for j in range(2**k):
			data = [x == '1' for x in bin(j)[2:]]
			data = [False]*(k-len(data)) + data
			try:
				pos = data.index(True)
			except:
				pos = k
			solution = [False]*pos+[True]*(k-pos)
			assert mesh_scanor(data) == solution
			print data
			print mesh_scanor(data)
			print

	return True

# test_scanor(4)
# print mesh_scanor([False, False, False, True])
print mesh_scanor([False, False, False, False, False, False, True, False, False, False, False, False, False, False, False, False])