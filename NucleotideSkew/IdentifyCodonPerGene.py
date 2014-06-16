
#! /usr/bin/python

import sys,getopt,re,copy
from operator import itemgetter, attrgetter

def usage():
	print "\n\t NucleotideSkew.py -g <GENOME-ID> \n\t Optional: -h<help>"
	print "\n\t Defaults: Window-size: 9999 Step-size: 3, both of them SHOULD be multiple of 3. "
	FileCheck()
	sys.exit()
	
def FileCheck():
	print "\n\t Following files are expected: *.fna *.ptt *.rnt in this folder \n"

def RemoveWhiteSpace(Input):
	temp=re.sub('^\s*','',Input)
	Output=re.sub('\s*$','',temp)

	return Output
		

def SeperateStrand(Gene,PGene,NGene):
	TotalGeneNum=len(Gene)
	for CurrStrand in Gene: #TotalGeneNum:
		if(CurrStrand[2]=='+'):
			PGene.append(CurrStrand)
			#print "\n\t Positive: "+str(CurrStrand[2])
		else:
			NGene.append(CurrStrand)
			#print "\n\t Negative: "+str(CurrStrand[2])		

def MergeGenes(TotalGeneRanges,ProteinGeneRanges,RnaGeneRanges):

	TotalMaxGeneNum=0
	TotalMinGeneNum=0
	ProteinGeneNum=len(ProteinGeneRanges)
	RnaGeneNum=len(RnaGeneRanges)
	if(ProteinGeneNum > RnaGeneNum):
		MaxGene=ProteinGeneRanges
		MinGene=RnaGeneRanges
		TotalMaxGeneNum=ProteinGeneNum
		TotalMinGeneNum=RnaGeneNum
	else:
		MaxGene=ProteinGeneRanges
		MinGene=RnaGeneRanges	
		TotalMaxGeneNum=RnaGeneNum
		TotalMinGeneNum=ProteinGeneNum		

	TotalGeneNum=RnaGeneNum+ProteinGeneNum
	MinGeneNum=0
	MaxGeneNum=0
		
	for CurrRangeNum in range(TotalGeneNum):
		# Since Gene ranges are assumed to be non-overlapping, hence it must be sufficient to just compare start ranges! 
		#print "\n\t MaxGeneNum "+str(MaxGeneNum)+" gene "+str(MaxGene[MaxGeneNum]) +" MinGeneNum "+str(MinGeneNum)+" gene "+str(MinGene[MinGeneNum])
		if ( (MaxGene[MaxGeneNum][0] >= MinGene[MinGeneNum][0]) ):
			if ((MaxGene[MaxGeneNum][0] >= MinGene[MinGeneNum][1]) ):
				TotalGeneRanges.append(MinGene[MinGeneNum])
				MinGeneNum+=1
				#print "\n\t MinGeneNum "+str(MinGeneNum)+" TotalMinGeneNum "+str(TotalMinGeneNum)
				if( MinGeneNum >= TotalMinGeneNum):
					for RangeNum in range(MaxGeneNum,TotalGeneNum-MinGeneNum):
						TotalGeneRanges.append(MaxGene[RangeNum])
					break
			else:
				MergeGene=[]
				MergeGene.append(MinGene[MinGeneNum][0])
				if(MaxGene[MaxGeneNum][1] >= MinGene[MinGeneNum][1] ):
					MergeGene.append(MaxGene[MaxGeneNum][1])
				else:
					MergeGene.append(MinGene[MinGeneNum][1])

				if(MaxGene[MaxGeneNum][2]==MinGene[MinGeneNum][2]):
					MergeGene.append(MaxGene[MaxGeneNum][2])
				else:
					print "\n\t ERROR: MinGene: "+str(MinGene[MinGeneNum])+" MaxGene: "+str(MaxGene[MaxGeneNum])+" are notof same direction!! "
					sys.exit()

				TotalGeneRanges.append(MergeGene)
				MinGeneNum+=1
				MaxGeneNum+=1
				#print "\n\t MinGeneNum "+str(MinGeneNum)+" TotalMinGeneNum "+str(TotalMinGeneNum)
				if( MinGeneNum >= TotalMinGeneNum):
					for RangeNum in range(MaxGeneNum,TotalGeneNum-MinGeneNum):
						TotalGeneRanges.append(MaxGene[RangeNum])
					break
				if(MaxGeneNum >= TotalMaxGeneNum):
					for RangeNum in range(MinGeneNum,TotalGeneNum-MaxGeneNum):
						TotalGeneRanges.append(MinGene[RangeNum])
					break					
				
		elif ((MaxGene[MaxGeneNum][1] >= MinGene[MinGeneNum][0]) ):
		    if ((MaxGene[MaxGeneNum][1] >= MinGene[MinGeneNum][1]) ):
				TotalGeneRanges.append(MaxGene[MaxGeneNum])
				MaxGeneNum+=1
				MinGeneNum+=1
				#print "\n\t MaxGeneNum "+str(MaxGeneNum)+" TotalMaxGeneNum "+str(TotalMaxGeneNum)
				if( MinGeneNum >= TotalMinGeneNum):
					for RangeNum in range(MaxGeneNum,TotalGeneNum-MinGeneNum):
						TotalGeneRanges.append(MaxGene[RangeNum])
					break				
				if( MaxGeneNum >= TotalMaxGeneNum):
					for RangeNum in range(MinGeneNum,TotalGeneNum-MaxGeneNum):
						TotalGeneRanges.append(MinGene[RangeNum])
					break
		    else:
				MergeGene=[]
				MergeGene.append(MaxGene[MaxGeneNum][0])
				MergeGene.append(MinGene[MinGeneNum][1])
				if(MaxGene[MaxGeneNum][2]==MinGene[MinGeneNum][2]):
					MergeGene.append(MaxGene[MaxGeneNum][2])
				else:
					print "\n\t ERROR: MinGene: "+str(MinGene[MinGeneNum])+" MaxGene: "+str(MaxGene[MaxGeneNum])+" are notof same direction!! "
					sys.exit()
				TotalGeneRanges.append(MergeGene)
				MinGeneNum+=1
				MaxGeneNum+=1
				#print "\n\t MinGeneNum "+str(MinGeneNum)+" TotalMinGeneNum "+str(TotalMinGeneNum)
				if( MinGeneNum >= TotalMinGeneNum):
					for RangeNum in range(MaxGeneNum,TotalGeneNum-MinGeneNum):
						TotalGeneRanges.append(MaxGene[RangeNum])
					break
				if(MaxGeneNum >= TotalMaxGeneNum):
					for RangeNum in range(MinGeneNum,TotalGeneNum-MaxGeneNum):
						TotalGeneRanges.append(MinGene[RangeNum])
					break					
		else:
				TotalGeneRanges.append(MaxGene[MaxGeneNum])
				MaxGeneNum+=1
				if( MaxGeneNum >= TotalMaxGeneNum):
					for RangeNum in range(MinGeneNum,TotalGeneNum-MaxGeneNum):
						TotalGeneRanges.append(MinGene[RangeNum])
					break			
	print "\n\t Inside MergeGene "+str(len(TotalGeneRanges))

def FindGeneRanges(GeneFile,GeneType):
	NumLinesGenes=len(GeneFile)
	print "\n\t Number of lines in gene report file is "+str(NumLinesGenes)+" the genes are supposed to code "+str(GeneType)
	HeaderLineNum=0

	for CurrLine in GeneFile:
		HeaderLineNum+=1
		HeaderLine=re.match('\s*Location\s*Strand',CurrLine)
		#print "\n\t Checking line: "+str(HeaderLineNum)
		if HeaderLine:
			print "\n\t Header for protein gene found at line "+str(HeaderLineNum)+"\n\t Header: "+str(CurrLine)
			break
		
	
	GeneRanges=[]
	NumGene=0
	for LineNum in range(HeaderLineNum,NumLinesGenes):
		CurrLine=re.match('(\d+)*\.\.(\d+)*\s*([\+\-])*',GeneFile[LineNum])
		if CurrLine:
			#print "\n\t LineNum: "+str(LineNum)+" CurrLine "+str(CurrLine.group(0))
			Range=((int(CurrLine.group(1))),(int(CurrLine.group(2))),(CurrLine.group(3)))
			GeneRanges.append(Range)
			NumGene+=1
		else:
			print "\n\t Didnt find nucleotide range in lineNum: "+str(LineNum)+" CurrLine "+str(GeneFile[LineNum])
			
		
	print "\n\t Found "+str(NumGene)+" genes. \n"
	return GeneRanges
	

def main(argv):
	GenomeID=''
	debug=0
	try:
		opts, args = getopt.getopt(argv,"hg:v:",["genomeID=","verbose="])
	except getopt.GetoptError:
		print "\n\t ##$$ \n"
		usage()
		sys.exit()	
	for opt, arg in opts:
		if opt == '-h':
			usage()
		elif opt in ("-g", "--genomeID"):
			tmp=RemoveWhiteSpace(arg)
			GenomeID=str(tmp)
			print "\n\t GenomeID option is "+str(GenomeID);
		else:
			usage()
	
	FileCheck()
	ProteinGeneFileName=GenomeID+str('.ptt')
	File1=open(ProteinGeneFileName)
	ProteinGene=File1.readlines()
	File1.close()
	
	ProteinGeneRanges=FindGeneRanges(ProteinGene,"Protein") # Assumed that each member will have an array with 1st and 2nd member indicating start and end nucleotide respectively.
	
	ProteinGeneNum=len(ProteinGeneRanges)
	
	RnaGeneFileName=GenomeID+str('.rnt')
	File1=open(RnaGeneFileName)
	RnaGene=File1.readlines()
	File1.close()
	
	RnaGeneRanges=FindGeneRanges(RnaGene,"Rna")	
	RnaGeneNum=len(RnaGeneRanges)

	TotalMaxGeneNum=0
	TotalMinGeneNum=0
	if(ProteinGeneNum > RnaGeneNum):
		MaxGene=ProteinGeneRanges
		MinGene=RnaGeneRanges
		TotalMaxGeneNum=ProteinGeneNum
		TotalMinGeneNum=RnaGeneNum
	else:
		MaxGene=ProteinGeneRanges
		MinGene=RnaGeneRanges	
		TotalMaxGeneNum=RnaGeneNum
		TotalMinGeneNum=ProteinGeneNum		

	TotalGeneRanges=[]
	TotalGeneNum=RnaGeneNum+ProteinGeneNum
	MinGeneNum=0
	MaxGeneNum=0	
		
	print "\n\t TotalGeneNum "+str(TotalGeneNum)+" ProteinGeneNum "+str(ProteinGeneNum)+" RnaGeneNum "+str(RnaGeneNum)
	
	ProteinPGene=[]; RnaPGene=[];PStrand=[]
	ProteinNGene=[]; RnaNGene=[];NStrand=[]
	
	SeperateStrand(ProteinGeneRanges,ProteinPGene,ProteinNGene)	
	print "\n\t 1. PGene-len: "+str(len(ProteinPGene))+" NGene-len: "	+str(len(ProteinNGene))
	SeperateStrand(RnaGeneRanges,RnaPGene,RnaNGene)			
	print "\n\t 2. PGene-len: "+str(len(RnaPGene))+" NGene-len: "	+str(len(RnaNGene))
		
	PGeneRanges=[];NGeneRanges=[];
	#MergeGenes(TotalGeneRanges,ProteinGeneRanges,RnaGeneRanges)
	TotalGeneNum=len(TotalGeneRanges)
	print "\n\t TotalGeneNum "+str(TotalGeneNum)
	
	MergeGenes(PGeneRanges,ProteinPGene,RnaPGene)
	print "\n\t PGeneNum "+str(len(PGeneRanges))

	MergeGenes(NGeneRanges,ProteinNGene,RnaNGene)
	print "\n\t NGeneNum "+str(len(NGeneRanges))	

	#for CurrRange in TotalGeneRanges:
	#	print "\n\t Gene-start "+str(CurrRange[0])+" end "+str(CurrRange[1])
	
	GenomeFileName=GenomeID+str('.fna')
	File1=open(GenomeFileName)
	GenomeFile=File1.readlines()
	File1.close()

	GenomeFileOffset=1	
	GenomeFileNumLines=len(GenomeFile)
	print "\n\t Num-lines genome: "+str(GenomeFileNumLines)+" and the genome file offset is: "+str(GenomeFileOffset)

	Genome=""
	for LineNum in range(GenomeFileOffset,GenomeFileNumLines):
		CurrLine=RemoveWhiteSpace(GenomeFile[LineNum])
		Genome+=CurrLine
		#Nucleotides=CurrLine.split('G')
		#print "\n\t CurrLine: "+str(CurrLine)+" length: "+str(len(Nucleotides))
		#sys.exit() #break
		
	del GenomeFile	
	GenomeLen=len(Genome)
	print "\n\t Length of genome: "+str(GenomeLen)+" 8th Nucleotide "+str(Genome[1:5])

	ProteinPGeneNucleotides=''
	
	ObtainNucleotides={}
	ObtainNucleotidesEnum={}
	
	ObtnNuclEnum={}	
	ObtnNuclEnum={'GeneRanges':0,'NucleotidesString':1,'CodonCount':2,'CodonCountCumulative':3}
	GeneRangesCollection=['ProteinPGene','ProteinNGene','RnaPGene','RnaNGene','PGene','NGene']

	GeneRangesIdxBase={}
	GeneRangesIdxBaseEnum={}
	GeneRangesIdxBaseEnum={'Base':0,'GeneRanges':1,'Len':2}
	GeneRangesBase=0
	for CurrGene in GeneRangesCollection:
		GeneRangesIdxBase[CurrGene]=[]
		GeneRangesBase+=10000
		GeneRangesIdxBase[CurrGene].append(GeneRangesBase)
		print "\n\t GeneRangesBase: "+str(GeneRangesBase)+" GeneRangesIdxBase[CurrGene]: "+str(GeneRangesIdxBase[CurrGene])+" CurrGene: "+str(CurrGene)
		
	GeneRangesIdxBase['ProteinPGene'].append(ProteinPGene)
	GeneRangesIdxBase['ProteinNGene'].append(ProteinNGene)
	GeneRangesIdxBase['RnaPGene'].append(RnaPGene)
	GeneRangesIdxBase['RnaNGene'].append(RnaNGene)
	GeneRangesIdxBase['PGene'].append(PGeneRanges)
	GeneRangesIdxBase['NGene'].append(NGeneRanges)

	for CurrGene in GeneRangesIdxBase:
		Base=GeneRangesIdxBase[CurrGene][GeneRangesIdxBaseEnum['Base']]
		Len=0
		for Idx,CurrRange in enumerate(GeneRangesIdxBase[CurrGene][GeneRangesIdxBaseEnum['GeneRanges']]):
			Tag= ( Base + Idx )
			ObtainNucleotides[Tag]=[CurrRange]
			Len+=1
			#print "\n\t CurrRange: "+str(CurrRange)+" Temp: "+str(Temp)+" Idx: "+str(Idx)+" Tag "+str(Tag)
		print "\n\t Done with this range : "+str(CurrGene)+" !"
		GeneRangesIdxBase[CurrGene].append(Len)
	#sys.exit()	
	"""ObtainNucleotides['ProteinPGene']=[ProteinPGene]
	ObtainNucleotides['ProteinNGene']=[ProteinNGene]
	ObtainNucleotides['RnaPGene']=[RnaPGene]
	ObtainNucleotides['RnaNGene']=[RnaNGene]
	ObtainNucleotides['PGene']=[PGeneRanges]
	ObtainNucleotides['NGene']=[NGeneRanges]"""

	for CurrGene in GeneRangesIdxBase:
		MaxLen= GeneRangesIdxBase[CurrGene][GeneRangesIdxBaseEnum['Base']] + GeneRangesIdxBase[CurrGene][GeneRangesIdxBaseEnum['Len']] 
		Base= GeneRangesIdxBase[CurrGene][GeneRangesIdxBaseEnum['Base']]
		for CurrGeneTag in range(Base,MaxLen):
			CurrGeneNucleotides=''
			CurrGeneNucleotides=str(Genome[ (ObtainNucleotides[CurrGeneTag][ObtnNuclEnum['GeneRanges']][0]-1): (ObtainNucleotides[CurrGeneTag][ObtnNuclEnum['GeneRanges']][1]) ])
			#print "\n\t CurrGeneR "+str(CurrGeneTag)+" len: "+str(len(CurrGeneNucleotides))
			ObtainNucleotides[CurrGeneTag].append(CurrGeneNucleotides)
		#break
		
	Nucleotides=['A','T','G','C']
	CodonCombisSet=[]
	Temp=''
	CodonCombisSet.append('')
	for CurrVar in range(3):
		TempCodonCombisSet=[]
		for CurrCodonCombiSet in CodonCombisSet:
			for CurrNucleotide in Nucleotides:
				Temp=str(CurrCodonCombiSet)+str(CurrNucleotide)
				TempCodonCombisSet.append(Temp)
				#print "\n\t Temp: "+str(Temp)
		CodonCombisSet=copy.deepcopy(TempCodonCombisSet)
		print "\n\t CurrVar: "+str(CurrVar)+" len(CurrCodonCombiSet): "+str(len(CodonCombisSet))
		
	CodonCount={}
	
	for CurrCodon in CodonCombisSet:
		CodonCount[CurrCodon]=0
	
	for CurrGene in GeneRangesIdxBase:
		MaxLen= GeneRangesIdxBase[CurrGene][GeneRangesIdxBaseEnum['Base']] + GeneRangesIdxBase[CurrGene][GeneRangesIdxBaseEnum['Len']] 
		Base= GeneRangesIdxBase[CurrGene][GeneRangesIdxBaseEnum['Base']]
 	#for CurrGene in ObtainNucleotides:
 		print "\n\t CurrGene: "+str(CurrGene)+" Base: "+str(Base)+" MaxLen: "+str(MaxLen)
	 	for CurrGeneTag in range(Base,MaxLen):
			Duh=copy.deepcopy(CodonCount)
			ObtainNucleotides[CurrGeneTag].append(Duh)
			if(not(len(ObtainNucleotides[CurrGeneTag][ObtnNuclEnum['GeneRanges']])>2)):
				print "\n\t CurrGeneTag: "+str(CurrGeneTag)+" (ObtainNucleotides[CurrGeneTag][ObtnNuclEnum['GeneRanges']]): "+str(ObtainNucleotides[CurrGeneTag][ObtnNuclEnum['GeneRanges']])+" len: "+str(len(ObtainNucleotides[CurrGeneTag][ObtnNuclEnum['GeneRanges']]))
				sys.exit()
			if (ObtainNucleotides[CurrGeneTag][ObtnNuclEnum['GeneRanges']][2]=='+'):
				
				NumNucleotides=(len(ObtainNucleotides[CurrGeneTag][ObtnNuclEnum['NucleotidesString']]))
				NumCodons=int((NumNucleotides-3)/3)
				#print "\n\t len(Nucleotides): "+str(len(ObtainNucleotides[CurrGeneTag][ObtnNuclEnum['NucleotidesString']]))+" NumCodons "+str(NumCodons)+" CurrGeneTag: "+str(CurrGeneTag)+" MaxLen: "+str(MaxLen)

				for i in range(NumCodons):
					CurrCodon=ObtainNucleotides[CurrGeneTag][ObtnNuclEnum['NucleotidesString']][(i*3):((i*3)+3)]
					#print "\n\t CurrCodon: "+str(CurrCodon)
					ObtainNucleotides[CurrGeneTag][ObtnNuclEnum['CodonCount']][CurrCodon]+=1
				#sys.exit()	
			
			else:
				
				if (ObtainNucleotides[CurrGeneTag][ObtnNuclEnum['GeneRanges']][2]=='-'): #NGeneDirCheck:
					
					NumNucleotides=(len(ObtainNucleotides[CurrGeneTag][ObtnNuclEnum['NucleotidesString']]))
					NumCodons=int((NumNucleotides-3)/3)
					#print "\n\t len(Nucleotides): "+str(len(ObtainNucleotides[CurrGeneTag][ObtnNuclEnum['NucleotidesString']]))+" NumCodons "+str(NumCodons)
					NumNucleotides-=1
					for i in range(NumCodons):
						End=NumNucleotides-(i*3)
						Start=NumNucleotides-((i+1)*3)
						CurrCodon=ObtainNucleotides[CurrGeneTag][ObtnNuclEnum['NucleotidesString']][Start:End]
						CurrCodon=CurrCodon[::-1]
						ObtainNucleotides[CurrGeneTag][ObtnNuclEnum['CodonCount']][CurrCodon]+=1
					#sys.exit()
					
				else:
					print "\n\t ERROR: Neither PGene or NGene "+str(CurrGeneTag)+" \n"
					sys.exit()
	
 
 		 	
	CurrCodonFile=open('CodonDistributionPerGene.log','w')
	CurrCodonFile.write("\n\t Format: Codon")
	
 	for CurrGene in GeneRangesIdxBase:
		MaxLen= GeneRangesIdxBase[CurrGene][GeneRangesIdxBaseEnum['Base']] + GeneRangesIdxBase[CurrGene][GeneRangesIdxBaseEnum['Len']] 
		Base= GeneRangesIdxBase[CurrGene][GeneRangesIdxBaseEnum['Base']]
		for CurrGeneTag in range(Base,MaxLen):
	 		#CurrCodonFile.write("\t "+str(CurrGene) )
	 		CodonCountCumulative=0
			ObtainNucleotides[CurrGeneTag].append(CodonCountCumulative)
	


 	for CurrGene in GeneRangesIdxBase:
		MaxLen= GeneRangesIdxBase[CurrGene][GeneRangesIdxBaseEnum['Base']] + GeneRangesIdxBase[CurrGene][GeneRangesIdxBaseEnum['Len']] 
		Base= GeneRangesIdxBase[CurrGene][GeneRangesIdxBaseEnum['Base']]
		CurrCodonFile.write("\n\n\t CurrGene: "+str(CurrGene)+"\n\n")
		for CurrCodon in CodonCombisSet:
	 		CurrCodonFile.write("\n\t CurrCodon: "+str(CurrCodon))
			for CurrGeneTag in range(Base,MaxLen):
	 			CurrCodonFile.write("\t "+str(ObtainNucleotides[CurrGeneTag][ObtnNuclEnum['CodonCount']][CurrCodon]))
	 			ObtainNucleotides[CurrGeneTag][ObtnNuclEnum['CodonCountCumulative']]+=ObtainNucleotides[CurrGeneTag][ObtnNuclEnum['CodonCount']][CurrCodon]
	 	CurrCodonFile.write("\n\n\t Cumulative: ")
	 	for CurrGeneTag in range(Base,MaxLen):
		 	CurrCodonFile.write("\t "+str(ObtainNucleotides[CurrGeneTag][ObtnNuclEnum['CodonCountCumulative']]))
 	
 	CurrCodonFile.write("\n\n") 
    
if __name__ == "__main__":
   main(sys.argv[1:])
