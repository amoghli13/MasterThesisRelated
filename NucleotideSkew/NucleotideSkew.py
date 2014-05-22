
#! /usr/bin/python

import sys,getopt,re

def usage():
	print "\n\t NucleotideSkew.py -g <GENOME-ID> \n\t Optional: -h<help>"
	print "\n\t Defaults: Window-size: 9999 Step-size: 3, both of them SHOULD be multiple of 3. "
	FileCheck()
	sys.exit()
	
def FileCheck():
	print "\n\t Following files are expected: *.fna *.ptt *.rnt in this folder \n"
def WhiteSpace(arg):
	tmp=re.sub('^\s*','',arg)
	tmp1=re.sub('\s*$','',tmp)
	#print "\n\t Arg "+str(arg)+" tmp "+str(tmp)+" tmp1 "+str(tmp1)
	return tmp1

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
		CurrLine=re.match('(\d+)*\.\.(\d+)*',GeneFile[LineNum])
		if CurrLine:
			Range=[]
			#print "\n\t LineNum: "+str(LineNum)+" CurrLine "+str(CurrLine.group(0))
			Range.append(int(CurrLine.group(1)))
			Range.append(int(CurrLine.group(2)))
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
			tmp=WhiteSpace(arg)
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


			
	TotalGeneNum=len(TotalGeneRanges)
	print "\n\t TotalGeneNum "+str(TotalGeneNum)

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
		CurrLine=GenomeFile[LineNum]
		Genome+=CurrLine
		#Nucleotides=CurrLine.split('G')
		#print "\n\t CurrLine: "+str(CurrLine)+" length: "+str(len(Nucleotides))
		#break

	del GenomeFile	
	GenomeLen=len(Genome)
	print "\n\t Length of genome: "+str(GenomeLen)+" 8th Nucleotide "+str(Genome[1:5])
	Genome+='-' # Just in case we access " TotalGeneRanges[CurrGene][1]+1
	GeneNucleotides="" #[]
	NonGeneNucleotides="" #[]

	if(TotalGeneRanges[0][0]):
		NonGeneStart=0
		#NonGeneEnd=TotalGeneRanges[0][0]
		for CurrGene in range(TotalGeneNum):
			GeneNucleotides+=(Genome[(TotalGeneRanges[CurrGene][0]-1):(TotalGeneRanges[CurrGene][1])]) # Nucleotide count used in *.ptt etc is index-1.
			NonGeneNucleotides+=(Genome[(NonGeneStart):(TotalGeneRanges[CurrGene][0])])		
			NonGeneStart=TotalGeneRanges[CurrGene][1]	
		NonGeneNucleotides+=(Genome[(NonGeneStart):(GenomeLen-1)])		
		print "	\n\t Gene-length: "+str(len(GeneNucleotides))+" Non-Gene-length "+str(len(NonGeneNucleotides))
	else:
		print "\n\t This Genome seems to be fault since it has a gene which begins at nucleotide 0"
	GeneSkew={};GeneSkew['1']=[];GeneSkew['2']=[];GeneSkew['3']=[]
	NonGeneSkew={};NonGeneSkew['1']=[];NonGeneSkew['2']=[];NonGeneSkew['3']=[];
	
	GeneNucleotideCount={}
	NonGeneNucleotideCount={}
	
	GeneNucleotideCount['1']={};GeneNucleotideCount['1']['G']=0;GeneNucleotideCount['1']['C']=0; NonGeneNucleotideCount['1']={};NonGeneNucleotideCount['1']['G']=0;NonGeneNucleotideCount['1']['C']=0; 
	GeneNucleotideCount['2']={};GeneNucleotideCount['2']['G']=0;GeneNucleotideCount['2']['C']=0; NonGeneNucleotideCount['2']={};NonGeneNucleotideCount['2']['G']=0;NonGeneNucleotideCount['2']['C']=0;
	GeneNucleotideCount['3']={};GeneNucleotideCount['3']['G']=0;GeneNucleotideCount['3']['C']=0; NonGeneNucleotideCount['3']={};NonGeneNucleotideCount['3']['G']=0;NonGeneNucleotideCount['3']['C']=0;
	
	GeneLen=len(GeneNucleotides)
	NonGeneLen=len(NonGeneNucleotides)
	
	WindowLength=9999
	WindowLengthDiv3=int((WindowLength-1)/3)+1.
	StepSize=3
	#WindowNum=0;NumWindows=GeneLen/3
	WindowStart=0

	WindowStart
	WindowEnd=WindowStart+WindowLength

	CirBufNuceotideCount={};
	CirBufNuceotideCount['1']={};CirBufNuceotideCount['1']['G']=[];CirBufNuceotideCount['1']['C']=[];
	CirBufNuceotideCount['2']={};CirBufNuceotideCount['2']['G']=[];CirBufNuceotideCount['2']['C']=[];
	CirBufNuceotideCount['3']={};CirBufNuceotideCount['3']['G']=[];CirBufNuceotideCount['3']['C']=[];
	for CurrNucleotide in range(WindowStart,WindowEnd):
		if(CurrNucleotide%3==0):
			#print "\n\t CurrentNucleotide: "+str(CurrNucleotide)
			if(GeneNucleotides[CurrNucleotide]=='G'):
				GeneNucleotideCount['1']['G']+=1
			elif(GeneNucleotides[CurrNucleotide]=='C'):
				GeneNucleotideCount['1']['C']+=1
			if(GeneNucleotides[CurrNucleotide+1]=='G'):
				GeneNucleotideCount['2']['G']+=1
			elif(GeneNucleotides[CurrNucleotide+1]=='C'):
				GeneNucleotideCount['2']['C']+=1
			if(GeneNucleotides[CurrNucleotide+2]=='G'):
				GeneNucleotideCount['3']['G']+=1
			elif(GeneNucleotides[CurrNucleotide+2]=='C'):
				GeneNucleotideCount['3']['C']+=1

			CirBufNuceotideCount['1']['G'].append(GeneNucleotideCount['1']['G']); 
			CirBufNuceotideCount['2']['G'].append(GeneNucleotideCount['2']['G']);
			CirBufNuceotideCount['3']['G'].append(GeneNucleotideCount['3']['G'])
			CirBufNuceotideCount['1']['C'].append(GeneNucleotideCount['1']['C']);
			CirBufNuceotideCount['2']['C'].append(GeneNucleotideCount['2']['C']);
			CirBufNuceotideCount['3']['C'].append(GeneNucleotideCount['3']['C'])
	
	GeneSkew['1'].append( GeneNucleotideCount['1']['G']-GeneNucleotideCount['1']['C'] )
	GeneSkew['2'].append( GeneNucleotideCount['2']['G']-GeneNucleotideCount['2']['C'] )
	GeneSkew['3'].append( GeneNucleotideCount['3']['G']-GeneNucleotideCount['3']['C'] )
			
	WindowStart=WindowEnd
	WindowEnd=GeneLen-3#100*WindowLength

	SkewOp1=open('CumulativeGeneSkewPos1_Full.log','w');SkewOp2=open('CumulativeGeneSkewPos2_Full.log','w');SkewOp3=open('CumulativeGeneSkewPos3_Full.log','w');
	# Future idea: Can perhaps make numeric '3' as a parameter?		
	CirBufIdx=0
	DummyIdx=0
	for CurrNucleotide in range(WindowStart,WindowEnd):
		CirBufIdx=int(CirBufIdx%WindowLengthDiv3)
		#print "\n\t CurrNucleotide "+str(CurrNucleotide)+" CirBufIdx "+str(CirBufIdx)+" WindowLengthDiv3 "+str(WindowLengthDiv3)

		if(CurrNucleotide%3==0):
			if(GeneNucleotides[CurrNucleotide]=='G'):
				GeneNucleotideCount['1']['G']+=1
			elif(GeneNucleotides[CurrNucleotide]=='C'):
				GeneNucleotideCount['1']['C']+=1
			if(GeneNucleotides[CurrNucleotide+1]=='G'):
				GeneNucleotideCount['2']['G']+=1
			elif(GeneNucleotides[CurrNucleotide+1]=='C'):
				GeneNucleotideCount['2']['C']+=1
			if(GeneNucleotides[CurrNucleotide+2]=='G'):
				GeneNucleotideCount['3']['G']+=1
			elif(GeneNucleotides[CurrNucleotide+2]=='C'):
				GeneNucleotideCount['3']['C']+=1
			
			#print "\n\t CurrNucleotide "+str(CurrNucleotide)+" WindowStart "+str(WindowStart)+" StepSize "+str(StepSize)
			if((CurrNucleotide-WindowStart)%StepSize==0):
				Tmp1=GeneNucleotideCount['1']['G']-GeneNucleotideCount['1']['C']#-CirBufNuceotideCount['1']['G'][CirBufIdx]+CirBufNuceotideCount['1']['C'][CirBufIdx]
				#SkewOp1.write("\n\t DummyIdx "+str(DummyIdx)+" CirBufIdx "+str(CirBufIdx)+" GNC['G'] "+str(GeneNucleotideCount['1']['G'])+" GNC['C'] "+str(GeneNucleotideCount['1']['C'])+" CirBuf['G'] "+str(CirBufNuceotideCount['1']['G'][CirBufIdx])+" CirBuf['C'] "+str(CirBufNuceotideCount['1']['C'][CirBufIdx])+" Tmp1 "+str(Tmp1))
				Tmp2=GeneNucleotideCount['2']['G']-GeneNucleotideCount['2']['C']#-CirBufNuceotideCount['2']['G'][CirBufIdx]+CirBufNuceotideCount['2']['C'][CirBufIdx]				
				Tmp3=GeneNucleotideCount['3']['G']-GeneNucleotideCount['3']['C']#-CirBufNuceotideCount['3']['G'][CirBufIdx]+CirBufNuceotideCount['3']['C'][CirBufIdx]				
				GeneSkew['1'].append(Tmp1);GeneSkew['2'].append(Tmp2);GeneSkew['3'].append(Tmp3)
				#if(CurrNucleotide%100==0): print "\n\t Tmp1 "+str(Tmp1)#+" GS['1'] "+str(GeneSkew['1'])
			CirBufNuceotideCount['1']['G'][CirBufIdx]=GeneNucleotideCount['1']['G']; 
			CirBufNuceotideCount['2']['G'][CirBufIdx]=GeneNucleotideCount['2']['G'];
			CirBufNuceotideCount['3']['G'][CirBufIdx]=GeneNucleotideCount['3']['G'];
			CirBufNuceotideCount['1']['C'][CirBufIdx]=GeneNucleotideCount['1']['C'];
			CirBufNuceotideCount['2']['C'][CirBufIdx]=GeneNucleotideCount['2']['C'];
			CirBufNuceotideCount['3']['C'][CirBufIdx]=GeneNucleotideCount['3']['C'];				
			CirBufIdx+=1
			DummyIdx+=1
	print"\n\t GeneNucleotideCount['1']: "+str(GeneNucleotideCount['1'])+" -- len CirBuf['1']['G'] "+str(len(CirBufNuceotideCount['1']['G']))
	print"\n\t GeneNucleotideCount['2']: "+str(GeneNucleotideCount['2'])+" -- len GeneSkew['1'] "+str(len(GeneSkew['1']))
	print"\n\t GeneNucleotideCount['3']: "+str(GeneNucleotideCount['3'])+" --\n"
		
	SkewLen=len(GeneSkew['1'])
	for Idx in range(SkewLen):
		#SkewOp1.write("\n\t i: "+str(i)+" Skew: "+str(CurrSkew))
		SkewOp1.write("\n\t "+str(GeneSkew['1'][Idx]))
		SkewOp2.write("\n\t "+str(GeneSkew['2'][Idx]))
		SkewOp3.write("\n\t "+str(GeneSkew['3'][Idx]))
		
"""		GeneStart=0
		NonGeneEnd=TotalGeneRanges[1][0] # Assuming that there is atleast 2 genes! 
		#GeneEnd=TotalGeneRanges[0][0]
		for CurrGene in range(TotalGeneNum-1):
			GeneNucleotides+=(Genome[(GeneStart):(TotalGeneRanges[CurrGene][1])])
			NonGeneNucleotides+=(Genome[(TotalGeneRanges[CurrGene][1]):(NonGeneEnd)]) # Nucleotide count used in *.ptt etc is index-1.
		
			GeneStart=TotalGeneRanges[CurrGene][1]
			NonGeneEnd=	TotalGeneRanges[CurrGene+1][0]
		NonGeneNucleotides+=(Genome[(GeneStart):(GenomeLen-1)])		# Abusing the variable "GeneStart" here! """

	
	
	
	

if __name__ == "__main__":
   main(sys.argv[1:])
