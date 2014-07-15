
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

def FindGeneRanges(GeneFile,GeneType,OriCPosition,GeneLength):
	NumLinesGenes=len(GeneFile)
	OriCComplement=GeneLength-OriCPosition
	print "\n\t Number of lines in gene report file is "+str(NumLinesGenes)+" the genes are supposed to code "+str(GeneType)
	print "\n\t GeneLen: "+str(GeneLength)+" and OriC is located at "+str(OriCPosition)+" so OriCComplement is "+str(OriCComplement)
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
			CurrentStart=int(CurrLine.group(1))
			CurrentEnd=int(CurrLine.group(2))
			RelativeStart=((CurrentStart+OriCComplement)%GeneLength)
			RelativeEnd=((CurrentEnd+OriCComplement)%GeneLength)
			#print "\n\t Current start: "+str(CurrentStart)+" CurrentEnd: "+str(CurrentEnd)+" RelativeStart: "+str(RelativeStart)+" RelativeEnd: "+str(RelativeEnd)
			#Range=((int(CurrLine.group(1))),(int(CurrLine.group(2))),(CurrLine.group(3)))
			Range=(RelativeStart,RelativeEnd,( CurrLine.group(3) ) )
			GeneRanges.append(Range)
			NumGene+=1
		else:
			print "\n\t Didnt find nucleotide range in lineNum: "+str(LineNum)+" CurrLine "+str(GeneFile[LineNum])
			
		
	print "\n\t Found "+str(NumGene)+" genes. \n"
	SortedGeneRanges=sorted(GeneRanges, key=itemgetter(0) )
	
	#for Idx,CurrRange in enumerate(SortedGeneRanges):
	#	print "\n\t Idx: "+str(Idx)+" CurrRange-start: "+str(CurrRange[1])
		
	return SortedGeneRanges
	

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
	

	Genome=''
	ActualGenome=''
	GenomeFileName=GenomeID+str('.fna')
	File1=open(GenomeFileName)
	GenomeFile=File1.readlines()
	File1.close()

	GenomeFileOffset=1	
	GenomeFileNumLines=len(GenomeFile)
	print "\n\t Num-lines genome: "+str(GenomeFileNumLines)+" and the genome file offset is: "+str(GenomeFileOffset)
	
	for LineNum in range(GenomeFileOffset,GenomeFileNumLines):
		CurrLine=RemoveWhiteSpace(GenomeFile[LineNum])
		ActualGenome+=CurrLine
		#Nucleotides=CurrLine.split('G')
		#print "\n\t CurrLine: "+str(CurrLine)+" length: "+str(len(Nucleotides))
		#sys.exit() #break
		
	del GenomeFile	
	ActualGenomeLen=len(ActualGenome)
	OriCPosition=3916950
	OriCComplement=ActualGenomeLen-OriCPosition
	
	print "\n\t GenomeLen "+str(ActualGenomeLen)+", so will move bases relative to "+str(OriCPosition)+" i.e, by "+str(OriCComplement)
	Genome=str(ActualGenome[(OriCPosition-1):(ActualGenomeLen-1)])+str(ActualGenome[0:(OriCPosition-1)])
	GenomeLen=len(Genome)
	print "\n\t Length of genome: "+str(GenomeLen)
	
	
	
	ProteinGeneRanges=FindGeneRanges(ProteinGene,"Protein",OriCPosition,GenomeLen) # Assumed that each member will have an array with 1st and 2nd member indicating start and end nucleotide respectively.
	
	ProteinGeneNum=len(ProteinGeneRanges)
	
	RnaGeneFileName=GenomeID+str('.rnt')
	File1=open(RnaGeneFileName)
	RnaGene=File1.readlines()
	File1.close()
	
	RnaGeneRanges=FindGeneRanges(RnaGene,"Rna",OriCPosition,GenomeLen)	
	RnaGeneNum=len(RnaGeneRanges)
	#sys.exit()

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
	MergeGenes(TotalGeneRanges,ProteinGeneRanges,RnaGeneRanges)
	TotalGeneNum=len(TotalGeneRanges)
	print "\n\t TotalGeneNum "+str(TotalGeneNum)
	
	MergeGenes(PGeneRanges,ProteinPGene,RnaPGene)
	print "\n\t PGeneNum "+str(len(PGeneRanges))

	MergeGenes(NGeneRanges,ProteinNGene,RnaNGene)
	print "\n\t NGeneNum "+str(len(NGeneRanges))	

	#for CurrRange in TotalGeneRanges:
	#	print "\n\t Gene-start "+str(CurrRange[0])+" end "+str(CurrRange[1])

	ProteinPGeneNucleotides=''
	
	ObtainNucleotides={}
	ObtainNucleotidesEnum={}
	
	ObtnNuclEnum={}	
	ObtnNuclEnum={'GeneRanges':0,'NucleotidesString':1,'SegmentLookup':2,'NumGenesSegment':3,'CodonCount':4,'CodonCountCumulative':5}

    	
	ObtainNucleotides['ProteinPGene']=[ProteinPGene]
	ObtainNucleotides['ProteinNGene']=[ProteinNGene]
	ObtainNucleotides['RnaPGene']=[RnaPGene]
	ObtainNucleotides['RnaNGene']=[RnaNGene]
	ObtainNucleotides['PGene']=[PGeneRanges]
	ObtainNucleotides['NGene']=[NGeneRanges]

	NucleotideSegmentLength=1000000
	for CurrGene in ObtainNucleotides:
		CurrGeneNucleotides=''
		CodonRangeIdx=0
		NumGenes=0
		SegmentLookup=[]
		NumGenesSegment=[]
		for CurrGeneRanges in ObtainNucleotides[CurrGene][ObtnNuclEnum['GeneRanges']]:
			if( float( CurrGeneRanges[0]/ NucleotideSegmentLength) > CodonRangeIdx):
				CodonRangeIdx+=1
				NucleotidestringLength=len(CurrGeneNucleotides)
				NumCodons=int((NucleotidestringLength-3)/3)
				SegmentNumGenes=NumGenes
				if(NumCodons<0):
					NumCodons=0
				NumGenes=0
				SegmentLookup.append(NumCodons)
				NumGenesSegment.append(SegmentNumGenes)
				#print "\n\t CurrGeneRange start: "+str(CurrGeneRanges[0])+" CodonRangeIdx: "+str(CodonRangeIdx)+" LenNucleotidesString: "+str(NucleotidestringLength)+" NumCodons: "+str(NumCodons)+" NumGenes "+str(SegmentNumGenes)
				
			CurrGeneNucleotides+=str(Genome[(CurrGeneRanges[0]-1):(CurrGeneRanges[1])])
			NumGenes+=1
			#print "\n\t CurrGeneRanges: "+str(CurrGeneRanges)+" Genes: "+str(Genome[(CurrGeneRanges[0]-1):(CurrGeneRanges[1])])
		NucleotidestringLength=len(CurrGeneNucleotides)
		NumCodons=int((NucleotidestringLength-3)/3)
		SegmentLookup.append(NumCodons)
		NumGenesSegment.append(NumGenes)
		#print "\n\t CurrGeneRange start: "+str(CurrGeneRanges[0])+" CodonRangeIdx: "+str(CodonRangeIdx)+" LenNucleotidesString: "+str(NucleotidestringLength)+" NumCodons: "+str(NumCodons)
		ObtainNucleotides[CurrGene].append(CurrGeneNucleotides)
		ObtainNucleotides[CurrGene].append(SegmentLookup)
		ObtainNucleotides[CurrGene].append(NumGenesSegment)
		print "\n\t Length nucleotide string: "+str(len(CurrGeneNucleotides))

	#sys.exit()
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
		
	
	
	CurrCodonFile=open('CodonDistributionPerSegment.log','w')
	CurrCodonFile.write("\n\t Format: Codon")

	CodonCount={}
	for CurrCodon in CodonCombisSet:
		CodonCount[CurrCodon]=0
	
	for CurrGene in ObtainNucleotides:
		Duh=copy.deepcopy(CodonCount)
		ObtainNucleotides[CurrGene].append(Duh)
		#for CurrCodon in ObtainNucleotides[CurrGene][ObtnNuclEnum['CodonCount']]:
			#print "\n\t CurrCodon: "+str(CurrCodon)+" Count: "+str(ObtainNucleotides[CurrGene][ObtnNuclEnum['CodonCount']][CurrCodon])
		PGeneDirCheck=re.match('\s*.*PGene$',CurrGene)
		if PGeneDirCheck:
			print "\n\t PGeneDirCheck: "+str(PGeneDirCheck.group(0))
			CurrCodonFile.write("\n\n\t Gene: "+str(CurrGene))
			#for Idx,CurrNucleotideSet in enumerate():
			CurrNucleotideSet=ObtainNucleotides[CurrGene][ObtnNuclEnum['NucleotidesString']]
			NumNucleotides=len(CurrNucleotideSet)
			NumNucleotidesSet=len(CurrNucleotideSet)
			NumSegments=int(NumNucleotides/NucleotideSegmentLength) 
			print "\n\t NumNucleotides: "+str(NumNucleotides)+" NumNucleotidesSet: "+str(NumNucleotidesSet)+" NumSegments: "+str(NumSegments)
			#sys.exit()
			NumCodons=int((NumNucleotides-3)/3)
			print "\n\t len(Nucleotides): "+str(NumNucleotides)+" NumCodons "+str(NumCodons)
			
			StartCodon=0	
			for Idx,CurrStopCodon in enumerate(ObtainNucleotides[CurrGene][ObtnNuclEnum['SegmentLookup']]):	
				print "\n\t SegmentLookup: "+str(CurrStopCodon)

				EndCodon=CurrStopCodon
				for i in range(StartCodon,EndCodon):
					CurrCodon=CurrNucleotideSet[(i*3):((i*3)+3)]
					temp=ObtainNucleotides[CurrGene][ObtnNuclEnum['CodonCount']][CurrCodon]
					ObtainNucleotides[CurrGene][ObtnNuclEnum['CodonCount']][CurrCodon]+=1
					#print "\n\t CurrCodon: "+str(CurrCodon)+"  CodonCountBef: "+str(temp)+" CodonCountAft: "+str(ObtainNucleotides[CurrGene][ObtnNuclEnum['CodonCount']][CurrCodon])
				#sys.exit()	
				StartCodon=CurrStopCodon
				CodonLength=(Idx+1)*NucleotideSegmentLength
				CurrCodonFile.write("\n\t Codon-length "+str(CodonLength))
				#print "\n\t NumGenes: "+str(ObtainNucleotides[CurrGene][ObtnNuclEnum['NumGenesSegment']][Idx])
				for CurrCodon in CodonCombisSet:
					CurrCodonFile.write("\t "+str(ObtainNucleotides[CurrGene][ObtnNuclEnum['CodonCount']][CurrCodon]))
					ObtainNucleotides[CurrGene][ObtnNuclEnum['CodonCount']][CurrCodon]=0
				
				CurrCodonFile.write("\t "+str((ObtainNucleotides[CurrGene][ObtnNuclEnum['NumGenesSegment']][Idx])))
		else:
			NGeneDirCheck=re.match('\s*.*NGene$',CurrGene)
			if NGeneDirCheck:
				print "\n\t NGeneDirCheck: "+str(NGeneDirCheck.group(0))
				CurrCodonFile.write("\n\n\t Gene: "+str(CurrGene))
##
				CurrNucleotideSet=ObtainNucleotides[CurrGene][ObtnNuclEnum['NucleotidesString']]
				NumNucleotides=len(CurrNucleotideSet)
				NumNucleotidesSet=len(CurrNucleotideSet)
				NumSegments=int(NumNucleotides/NucleotideSegmentLength) 
				#print "\n\t NumNucleotides: "+str(NumNucleotides)+" NumNucleotidesSet: "+str(NumNucleotidesSet)+" NumSegments: "+str(NumSegments)
				#sys.exit()
				NumCodons=int((NumNucleotides-3)/3)
				print "\n\t len(Nucleotides): "+str(NumNucleotides)+" NumCodons "+str(NumCodons)
			
				StartCodon=0	
				for Idx,CurrStopCodon in enumerate(ObtainNucleotides[CurrGene][ObtnNuclEnum['SegmentLookup']]):	
					print "\n\t SegmentLookup: "+str(CurrStopCodon)

					EndCodon=CurrStopCodon
##
					for i in range(StartCodon,EndCodon):
						End=NumNucleotides-(i*3)
						Start=NumNucleotides-((i+1)*3)
						CurrCodon=CurrNucleotideSet[Start:End]
						CurrCodon=CurrCodon[::-1]
						ObtainNucleotides[CurrGene][ObtnNuclEnum['CodonCount']][CurrCodon]+=1

					StartCodon=CurrStopCodon
					CodonLength=(Idx+1)*NucleotideSegmentLength
					CurrCodonFile.write("\n\t Codon-length "+str(CodonLength))
					for CurrCodon in CodonCombisSet:
						CurrCodonFile.write("\t "+str(ObtainNucleotides[CurrGene][ObtnNuclEnum['CodonCount']][CurrCodon]))
						ObtainNucleotides[CurrGene][ObtnNuclEnum['CodonCount']][CurrCodon]=0
					CurrCodonFile.write("\t "+str((ObtainNucleotides[CurrGene][ObtnNuclEnum['NumGenesSegment']][Idx])))

					
			else:
				print "\n\t ERROR: Neither PGene or NGene "+str(CurrGene)+" \n"
				sys.exit()

	
 	"""for CurrGene in ObtainNucleotides: 
 		CurrCodonFile.write("\t "+str(CurrGene) )
 		CodonCountCumulative=0
		ObtainNucleotides[CurrGene].append(CodonCountCumulative)
	
	for CurrCodon in CodonCombisSet:
 		CurrCodonFile.write("\n\t CurrCodon: "+str(CurrCodon))
 		for CurrGene in ObtainNucleotides:
 			CurrCodonFile.write("\t "+str(ObtainNucleotides[CurrGene][ObtnNuclEnum['CodonCount']][CurrCodon]))
 			ObtainNucleotides[CurrGene][ObtnNuclEnum['CodonCountCumulative']]+=ObtainNucleotides[CurrGene][ObtnNuclEnum['CodonCount']][CurrCodon]
 	
	CurrCodonFile.write("\n\n\t Cumulative: ")
	for CurrGene in ObtainNucleotides:
 		CurrCodonFile.write("\t "+str(ObtainNucleotides[CurrGene][ObtnNuclEnum['CodonCountCumulative']]))"""
 	
 	CurrCodonFile.write("\n\n") 
    
if __name__ == "__main__":
   main(sys.argv[1:])
