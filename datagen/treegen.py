#this file has 10 tree structures in the list with malicious node moviing up and down the tree
#produce the bracket tree output format by simulating data transmission between nodes

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

#the root dict has only one member as key but rest dict has many member as key
#so to preserve commonality it must be totally enclosed in brackets

def gettree(t)->str:
	if type(t)==type({}):
		return '('+",".join([str(i)+gettree(t[i]) for i in t])+')'
	else:
		return ""

for i in lst:
	print(gettree(i))

#so how actual trans works
#0 , 1 and 2 sends itself to 3,
#3 packs the different strings it recieves into comma spearated items in a bracket
#3 attaches itself before the bracket and sends the new string to 9 etc
