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
lsttree = [
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


#new random tree
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

"""old tree fenout
(9(3(0,1,2),8(6(4,5),7)))
(9(3(0,1,2),8(6(4,5),7)))
(9(3(0,1,2),8(4(6,5),7)))
(9(3(0,1,2),4(8(6,5),7)))
(4(3(0,1,2),9(8(6,5),7)))
(3(4(0,1,2),9(8(6,5),7)))
(3(0(4,1,2),9(8(6,5),7)))
(3(0(7,1,2),9(8(6,5),4)))
(3(0(7,1,2),9(8(6,4),5)))
(3(0(7,1,2),9(8(4,6),5)))
"""