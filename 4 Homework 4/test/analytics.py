from math import factorial
Paths = [
8638870,
8728781,
9195219,
8866298,
9215391,
8428092,
8696884,
]

av =  sum(Paths)/len(Paths)
for p in Paths:
	print av-p