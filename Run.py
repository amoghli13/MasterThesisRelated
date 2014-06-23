import sys,re,subprocess,commands

def main():
	FaultConfig={}
	FaultConfig['unsymmetric']=(0.0001,0)
	FaultConfig['bimodal']=(0.0001,1)
	FaultConfig['trimodal']=(0.000001,2)
	FaultConfig['unimodal']=(0.000001,3)
	FaultConfig['bitflip']=(0.000001,4)

	SetSize=50
	Objects=['FloydWarshall','FloydWarshallTMR']
	Inputs=[(500,'Inputs500.txt'),(50,'Inputs50.txt'),(200,'Inputs200.txt')]
	for CurrInput in Inputs:
		CpFile='cp '+str(CurrInput[1])+' inputs.txt'
		commands.getoutput(CpFile)
		SetSize=int(CurrInput[0]/4)
		for CurrObj in Objects:
			for Key in FaultConfig:
				RunCommand='./'+str(CurrObj)+' '+str(SetSize)+' '+str(FaultConfig[Key][0])+' '+str(FaultConfig[Key][1])+' > Meh.log'
				print "\n\t pin -t ../../obj-intel64/InstCount_Error_Injection_OO.so -- "+str(RunCommand)
				commands.getoutput(RunCommand)
				OutputFile=str(CurrObj)+'_'+str(Key)+'_'+str(FaultConfig[Key][0])+'_'+str(CurrInput[0])+'_'+str(SetSize)+'.log'
				MvCommand='mv InstCountSummary.log '+str(OutputFile)
				print "\n\t MvCommand: "+str(MvCommand)
				sys.exit()

if __name__=="__main__":
	main()
