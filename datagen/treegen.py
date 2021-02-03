#9(3(0,1,2),8(6(4,5),7))

# def ptree(d):
# 	print("stat",d)
# 	if type(d)==type({}):
# 		for i in d:
# 			print(i)
# 			ptree(d[i])
# print(l,type(l))
# ptree(l)

#l = { 9:{ 3:{0:None,1:None,2:None},8:{6:{4:None,5:None},7:None} } }
#10 second interval outlier node moves up and down node 4
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