

import sys,getopt,re;

def main(argv):
    KmerLength=4
    debug=0
    try:
        opts, args = getopt.getopt(sys.argv[1:],"k:h:v",["KmerLength","deubg","help","verbose"])
    except getopt.GetoptError:
		#print str(err) # will print something like "option -a not recognized"
        usage()
        sys.exit(2)
	verbose=False
	"""for opt, arg in opts:
        if opt == '-h':
            #print 'test.py -i <inputfile> -o <outputfile>'
            print "\n\t Usage: FreqWord.py -k <kmer-length>"
            sys.exit()
        elif opt in ("-k", "--KmerLength"):
            KmerLength=arg
        #print "\n\t Config file is "+str(config)+"\n";
        elif opt in ("-d", "--debug"):
            debug=int(arg)
            print "\n\t Debug option is "+str(debug)+"\n";
        else:
            usage()
    """
    print "\n\t Kmer-length is: "+str(KmerLength)
    
    Input= "CGTGATTCCGGCGGGCGTGGAGAAGCGAGATTCATTCAAGCCGGGAGGCGTGGCGTGGCGTGGCGTGCGGATTCAAGCCGGCGGGCGTGATTCGAGCGGCGGATTCGAGATTCCGGGCGTGCGGGCGTGAAGCGCGTGGAGGAGGCGTGGCGTGCGGGAGGAGAAGCGAGAAGCCGGATTCAAGCAAGCATTCCGGCGGGAGATTCGCGTGGAGGCGTGGAGGCGTGGAGGCGTGCGGCGGGAGATTCAAGCCGGATTCGCGTGGAGAAGCGAGAAGCGCGTGCGGAAGCGAGGAGGAGAAGCATTCGCGTGATTCCGGGAGATTCAAGCATTCGCGTGCGGCGGGAGATTCAAGCGAGGAGGCGTGAAGCAAGCAAGCAAGCGCGTGGCGTGCGGCGGGAGAAGCAAGCGCGTGATTCGAGCGGGCGTGCGGAAGCGAGCGG"  #"CGGAAGCGAGATTCGCGTGG";
    InputLength=len(Input)

    print "\n\t Input-length: "+str(InputLength)
    if(InputLength<80):
        print"\n\t Input: "+str(Input)

    NumKmers=InputLength-KmerLength+1
    KmerComposition=[];
    
    for i in range(NumKmers):
        TmpString=""
        for j in range(KmerLength):
            TmpString+=Input[i+j]
        KmerComposition.append(TmpString)
#print"\t TmpString: "+str(TmpString)+" len: "+str(len(TmpString))

    AComposition=[];CComposition=[];GComposition=[];TComposition=[];
    for i in range(NumKmers):
        #print"\t i: "+str(i)+" kmer: "+str(KmerComposition[i])
        if(KmerComposition[i][0]=="A"):
            AComposition.append(KmerComposition[i])
        elif(KmerComposition[i][0]=="C"):
            CComposition.append(KmerComposition[i])
        elif(KmerComposition[i][0]=="G"):
            GComposition.append(KmerComposition[i])
        elif(KmerComposition[i][0]=="T"):
            TComposition.append(KmerComposition[i])

    print"\n\t NumKmers: "+str(NumKmers)+" Num-A: "+str(len(AComposition))+" Num-C: "+str(len(CComposition))+" Num-G: "+str(len(GComposition))+" Num-T: "+str(len(TComposition))

    TmpA=[];TmpC=[];TmpG=[];TmpT=[];
    #   for k in range(1,KmerLength-1):
    SortIdx=1 ; #This should go in a loop!
    FreqWordA={}
    for j in range(4):
        TmpA=[];TmpC=[];TmpG=[];TmpT=[];
        if(j==0):
            TmpComposition=AComposition;
        elif(j==1):
            TmpComposition=CComposition;
        elif(j==2):
            TmpComposition=GComposition;
        elif(j==3):
            TmpComposition=TComposition;
        for i in range(len(TmpComposition)):
            #print"\n\t j: "+str(j)+" Kmer: "+TmpComposition[i]
            if(TmpComposition[i][SortIdx]=="A"):
                TmpA.append(TmpComposition[i])#[SortIdx])
            if(TmpComposition[i][SortIdx]=="C"):
                TmpC.append(TmpComposition[i])
            if(TmpComposition[i][SortIdx]=="G"):
                TmpG.append(TmpComposition[i])
            if(TmpComposition[i][SortIdx]=="T"):
                TmpT.append(TmpComposition[i])

        TmpComposition=[];
        #FreqWordA={}
        for m in range(4):
            if(m==1):
                TmpA=TmpC;
            elif(m==2):
                TmpA=TmpG;
            elif(m==3):
                TmpA=TmpT;
            for i in range(len(TmpA)):
                Count=0;
                for k in range(j,len(TmpA)):
                    if(TmpA[i]==TmpA[k]):
                        Count+=1;
                if(Count!=0):
                    if (~(TmpA[i] in FreqWordA)):
                        FreqWordA[TmpA[i]]=Count
#print"\n\t i: "+str(i)+" K-mer: "+str(TmpA[i])+" Count: "+str(FreqWordA[TmpA[i]])


        print "\n\t J-C: "+str(j)+" len: "+str(len(TmpC))
        for i in range(len(TmpC)):
            TmpComposition.append(TmpC[i]);
        print "\n\t J-G: "+str(j)+" len: "+str(len(TmpG))
        for i in range(len(TmpG)):
            TmpComposition.append(TmpG[i]);
        print "\n\t J-T: "+str(j)+" len: "+str(len(TmpT))
        for i in range(len(TmpT)):
            TmpComposition.append(TmpT[i]);
#print TmpT[i]
        if(j==0):
            AComposition=TmpComposition;
        elif(j==1):
            CComposition=TmpComposition;
        elif(j==2):
            GComposition=TmpComposition;
        elif(j==3):
            TComposition=TmpComposition;

    print"\n\t NumKmers: "+str(NumKmers)+" Num-A: "+str(len(AComposition))+" Num-C: "+str(len(CComposition))+" Num-G: "+str(len(GComposition))+" Num-T: "+str(len(TComposition))

    MostFreqCount=0
    MostFreqKmer=''
    for keys in FreqWordA:
        print "\n\t K-mer: "+str(keys)+"  "+str(FreqWordA[keys])
        if(MostFreqCount<FreqWordA[keys]):
            MostFreqCount=FreqWordA[keys]
            MostFreqKmer=keys

    print "\n\t Most Frequent k-mer: "+str(MostFreqKmer)+" Count "+str(MostFreqCount)






if __name__=="__main__":
    main(sys.argv[1:])