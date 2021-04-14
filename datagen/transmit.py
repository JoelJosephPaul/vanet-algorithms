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
{ 9:{ 3:{0:None,1:None,2:None},8:{6:{4:None,5:None},7:None} } },
{ 9:{ 3:{0:None,1:None,2:None},8:{6:{4:None,5:None},7:None} } },
{ 9:{ 3:{0:None,1:None,2:None},8:{4:{6:None,5:None},7:None} } },
{ 9:{ 3:{0:None,1:None,2:None},4:{8:{6:None,5:None},7:None} } },
{ 4:{ 3:{0:None,1:None,2:None},9:{8:{6:None,5:None},7:None} } },
{ 3:{ 4:{0:None,1:None,2:None},9:{8:{6:None,5:None},7:None} } },
{ 3:{ 0:{4:None,1:None,2:None},9:{8:{6:None,5:None},7:None} } },
{ 3:{ 0:{7:None,1:None,2:None},9:{8:{6:None,5:None},4:None} } },
{ 3:{ 0:{7:None,1:None,2:None},9:{8:{6:None,4:None},5:None} } },
{ 3:{ 0:{7:None,1:None,2:None},9:{8:{4:None,6:None},5:None} } }
]

def agg(d):
	if type(d)==type({}):
		n=0
		s=0
		for i in d:
			if i!=4:#malicious
				rs,rn = agg(d[i])
				s+=(rs+10)/(rn+1)#10 is the real transmitted data
			n+=1
		return (s,n)
	elif d==None:
		return (0,0)

def op(level,d):
	for i in d:
		print(" "*level*5+str(i))
		if d[i]!=None:
			op(level+1,d[i])
	
for i in lst:
	op(1,i)
	r1,r2 = agg(i)
	print(r1/r2)