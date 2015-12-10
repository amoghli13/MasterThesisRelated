def generateGraph(n=100, avgNeigbors=10):
    from random import randint
    edges_list=[]
    edges_count=0;
    n_plus1=n;
    for i in range(n):
        dummy=str(i)
        dummy_plus1=i+1
        dummy_plus1=str(dummy_plus1)
        dummy1=0
        for m in range(avgNeigbors-randint(-avgNeigbors/2,avgNeigbors/2)):
            dummy1= (randint(1,n_plus1))
            if dummy1==dummy_plus1:
            	dummy1=(dummy_plus1+1)%(n_plus1)
            #print str((randint(0,n)))+','+str(dummy)+','+str(dummy1)
            
            duh=str(int(dummy_plus1)-1)+' '+str(int(dummy1)-1)+' '+str(randint(1,n))+' '+str(randint(0,1))
            edges_list.append(duh);
            edges_count+=1;
        

    edge_count=0
    dummy=randint(1,n_plus1)
    dummy1=randint(1,n_plus1)
    print str(n)+' '+str(edges_count)+' '+str(dummy)+' '+str(dummy1)
    for i in range(edges_count):
      	print "\n "+str(edges_list[i])
    print ' Edges count is : '+str(edges_count)       	
if __name__ == "__main__":
    generateGraph(200,95)           
