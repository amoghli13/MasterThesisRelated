import sys,re,getopt

def main():
	print "\n\t Well! Hello there "
	Input=open('Input.txt')
	Edges=Input.readlines()
	Input.close()

	print"\n\t Go and read "+str(len(Edges))+" lines "
	LineCount=0
	EdgeHash={}
	for CurrLine in Edges:
		BreakLine=re.match('\s*(\d+)+\s*\-\>(.*)',CurrLine)
		if BreakLine:
			#LineCount+=1	
			Tmp=re.sub('^\s*','',BreakLine.group(1))
			LHS=re.sub('\s*$','',Tmp)
			if ~(LHS in EdgeHash):
				Len=EdgeHash[LHS]=[]
			
			RHS=re.split(',',BreakLine.group(2))
			if RHS:
				for RHSVertices in RHS:
					Tmp=re.sub('^\s*','',RHSVertices) #print "\n\t RHS: "+str(RHSVertices)
					CleanRhs=re.sub('\s*$','',Tmp)
					EdgeHash[LHS].append(CleanRhs)

	IndexHash={}
	for keys in EdgeHash:
		IndexHash[keys]=0 # Since we need to compare this to length, which counts from 1.
		print "\n\t Key: -"+str(keys)+"-" #+" len "+str(len(EdgeHash
	
	traverse=1
	JumpFromNode=keys
	JumpToNode=EdgeHash[keys][0]
	IndexHash[JumpFromNode]+=1

	while(traverse):	
		if(1&(JumpToNode in EdgeHash)):	
			if(IndexHash[JumpToNode] < len(EdgeHash[JumpToNode])):
				print "\n\t Jumped from node: "+str(JumpToNode)+" next will jump to "+str(EdgeHash[JumpToNode][IndexHash[JumpToNode]])
				IndexHash[JumpToNode]+=1
				JumpToNode=EdgeHash[JumpToNode][IndexHash[JumpToNode]-1]
			else:
				print "\n\t Since IndexHash[JumpToNode] is "+str(IndexHash[JumpToNode])+" is same as len(EdgeHash[JumpToNode]) "+str(len(EdgeHash[JumpToNode]))+" we have to stop traversing"
				traverse=0
		else:
			print "\n\t Well this is unfortunate, JumpToNode: "+str(JumpToNode)+" does not have anything edge out of it. Perhaps this graph is not balanced! \n "
			traverse=0


if __name__=="__main__":
	main()

