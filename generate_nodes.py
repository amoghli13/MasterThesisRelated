def generateGraph(n=100, avgNeigbors=10):
    from random import randint
    for i in range(n):
        dummy="N"+str(i)
        for m in range(avgNeigbors-randint(-avgNeigbors/2,avgNeigbors/2)):
            print str((randint(0,n)))+','+str(dummy)
            
if __name__ == "__main__":
    generateGraph()           
