def normalize(lst,nmin,nmax):
	omin = min(lst)
	omax = max(lst)
	return [((x-omin)/(omax-omin))*(nmax-nmin) + nmin for x in lst]

bstr = """0.1     0.769516
0.2     2.49121
0.3     0.742126
0.4     0.740282
0.5     0.763479
0.6     2.48955
0.7     0.763082
0.8     1.21099
0.9     2.48861
1       0.764735"""
st = bstr.split()

data = [float(i) for i in st[1::2]]
print(data)
nlist = normalize(data,0,1)
print(nlist)
keys = [i for i in st[::2]]
for i in range(len(keys)):
	print(keys[i],nlist[i],sep='\t')