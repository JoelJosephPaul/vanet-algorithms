#9(3(0,1,2),8(6(4,5),7))

# def ptree(d):
# 	print("stat",d)
# 	if type(d)==type({}):
# 		for i in d:
# 			print(i)
# 			ptree(d[i])
# print(l,type(l))
# ptree(l)

l = { 9:{ 3:{0:None,1:None,2:None},8:{6:{4:None,5:None},7:None} } }
realdata1 = [10, 10, 10, 10, 10, 9, 9, 9, 9, 9, 5, 5, 5, 5, 5, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 8, 8, 8, 8, 8, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 8, 8, 8, 8, 8, 6, 6, 6, 6, 6, 8, 8, 8, 8, 8, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10]
realdata2 = [10]*100
lst = [
{0:{2:{4:None,5:None},3:{6:{1:None,8:None},7:None},9:None}},
{9:{1:{5:{6:None,7:None}},0:{2:None,4:None},3:{8:None}}},
{5:{2:{1:{9:None},4:{3:{0:None,7:None},6:None}},8:None}},
{4:{0:{7:None},5:{9:{1:None,8:None},2:{3:None,6:None}}}},
{3:{4:{5:{7:None},0:None},8:{9:{1:None,2:None},6:None}}},
{6:{2:{8:None,9:None},3:{5:None},1:{7:None,0:None,4:None}}},
{7:{8:{2:None},5:{3:{9:None,6:None},4:None},1:None,0:None}},
{2:{6:{8:None,5:None,1:None,4:None},7:{0:None,9:None,3:None}}},
{1:{6:{0:None,5:None},7:{9:{2:None,8:None,3:None},4:None}}},
{8:{1:{6:None,4:None,0:None},3:{9:None,2:None},7:{5:None}}},
]

malicious=[1,6,9]
realvalue=10

maxval=10
minval=0
avgval = (maxval+minval)/2
def malval(num):
	if num>avgval:
		return minval
	else:
		return maxval

def agg(d):
	if type(d)==type({}):
		n=0.0
		s=0.0
		for i in d:
			if i not in malicious:#malicious only 4,
				rs,rn = agg(d[i])
				s+=(rs+realvalue)/(rn+1)#realvalue is the real transmitted data
			else:
				s+=malval(realvalue)
			n+=1
		return (s,n)
	elif d==None:
		return (0,0)

def op(level,d):
	for i in d:
		print(" "*level*5+str(i))
		if d[i]!=None:
			op(level+1,d[i])
	
aglst = []
for i in lst:
	op(1,i)
	r1,r2 = agg(i)
	aglst.append(r1/r2)
	print(r1/r2)

times = [i/10 for i in range(1,11)]

for i,j in zip(times,aglst):
	print(i,j,sep='\t')