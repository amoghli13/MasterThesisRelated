/*BEGIN_LEGAL 
Intel Open Source License 

Copyright (c) 2002-2012 Intel Corporation. All rights reserved.
 
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.  Redistributions
in binary form must reproduce the above copyright notice, this list of
conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.  Neither the name of
the Intel Corporation nor the names of its contributors may be used to
endorse or promote products derived from this software without
specific prior written permission.
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE INTEL OR
ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
END_LEGAL */
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "pin.H"

ofstream OutFile;

// The running count of instructions is kept here
// make it static to help the compiler optimize docount
static UINT64 icount = 0;

// This function is called before every instruction is executed
VOID docount() { icount++; }
    
// Pin calls this function every time a new instruction is encountered
VOID Instruction(INS ins, VOID *v)
{
    // Insert a call to docount before every instruction, no arguments are passed
    INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)docount, IARG_END);
}

VOID Routine( RTN rtn, VOID *v )
{

	int rtn_primary_count=0;
	RTN_Open(rtn);
	for(INS ins=RTN_InsHead(rtn); INS_Valid(ins); ins= INS_Next(ins) )
		rtn_primary_count++;
	
//	if( IMG_IsMainExecutable( SEC_Img( RTN_Sec(rtn)  ) ) )
//	{
//		cout<<"\n\t Considering routine: "<<RTN_Name(rtn)<<" and adding "<<rtn_inst_count<<" number of instructions! ";
//	}
//	else
//	{
//		cout<<"\n\t Considering routine: "<<RTN_Name(rtn)<<" and NOT adding "<<rtn_inst_count<<" number of instructions! ";	
//	}
//	RTN_Close(rtn);
	int rtn_secondary_count=0;
	for(INS ins=RTN_InsHead(rtn); INS_Valid(ins); ins= INS_Next(ins) )
	{
	
		if(INS_HasFallThrough(ins) )
			rtn_secondary_count++;
	
	}
	
	cout<<"\n\t\t RTN: "<<RTN_Name(rtn)<<" primary count: "<<rtn_primary_count<<" secondary count "<<rtn_secondary_count;
	cout<<endl;
	RTN_Close(rtn);

}


VOID Routine1( RTN rtn, VOID *v )
{

	int rtn_primary_count=0;
	RTN_Open(rtn);
	for(INS ins=RTN_InsHead(rtn); INS_Valid(ins); ins= INS_Next(ins) )
		rtn_primary_count++;
	
//	if( IMG_IsMainExecutable( SEC_Img( RTN_Sec(rtn)  ) ) )
//	{
//		cout<<"\n\t Considering routine: "<<RTN_Name(rtn)<<" and adding "<<rtn_inst_count<<" number of instructions! ";
//	}
//	else
//	{
//		cout<<"\n\t Considering routine: "<<RTN_Name(rtn)<<" and NOT adding "<<rtn_inst_count<<" number of instructions! ";	
//	}
//	RTN_Close(rtn);
	int rtn_secondary_count=0;
	for(INS ins=RTN_InsHead(rtn); INS_Valid(ins); ins= INS_Next(ins) )
	{
	
		if(INS_HasFallThrough(ins) )
			rtn_secondary_count++;
	
	}
	
	cout<<"\n\t\t --- RTN: "<<RTN_Name(rtn)<<" primary count: "<<rtn_primary_count<<" secondary count "<<rtn_secondary_count;
	cout<<endl;
	RTN_Close(rtn);

}



VOID ImageLoad(IMG img, VOID *v)
{
 	cout<< "loading "<< IMG_Name(img).c_str()<<endl;
// 	int count_sym=1;
// 	if(IMG_IsMainExecutable(img) )
//	{
//		for (SYM sym = IMG_RegsymHead(img); SYM_Valid(sym); sym = SYM_Next(sym),count_sym++)
//		{
//			string undFuncName = PIN_UndecorateSymbolName(SYM_Name(sym), UNDECORATION_NAME_ONLY);
//			cout<<"\n\t Symbol count "<<count_sym<<"\t Name: "<<undFuncName;
//		}
//	}

	for( SEC sec=IMG_SecHead(img); SEC_Valid(sec) ; sec=SEC_Next(sec) )
	{
		cout<<"\n\t In Section: "<<SEC_Name(sec);
		
		for( RTN rtn=SEC_RtnHead(sec); RTN_Valid(rtn); rtn= RTN_Next(rtn) )
		{
			RTN_Open(rtn);
			if( SYM_Valid( RTN_Sym(rtn) ) )
			{
				string sym_name=PIN_UndecorateSymbolName( SYM_Name( RTN_Sym(rtn) ) , UNDECORATION_NAME_ONLY ) ;
				cout<<"\n\t\t RTN: "<<RTN_Name(rtn)<<" SYM "<<sym_name;
			}
			RTN_Close(rtn);
			Routine(rtn,0);

		}
	
	}



}					



KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool",
    "o", "inscount.out", "specify output file name");

// This function is called when the application exits
VOID Fini(INT32 code, VOID *v)
{
    // Write to a file since cout and cerr maybe closed by the application
    OutFile.setf(ios::showbase);
    OutFile << "Count " << icount << endl;
     OutFile.close();
}



/* ===================================================================== */
/* Print Help Message                                                    */
/* ===================================================================== */

INT32 Usage()
{
    cerr << "This tool counts the number of dynamic instructions executed" << endl;
    cerr << endl << KNOB_BASE::StringKnobSummary() << endl;
    return -1;
}

/* ===================================================================== */
/* Main                                                                  */
/* ===================================================================== */
/*   argc, argv are the entire command line: pin -t <toolname> -- ...    */
/* ===================================================================== */

int main(int argc, char * argv[])
{
    // Initialize pin
    if (PIN_Init(argc, argv)) return Usage();

    OutFile.open(KnobOutputFile.Value().c_str());

    // Register Instruction to be called to instrument instructions
    //INS_AddInstrumentFunction(Instruction, 0);
    PIN_InitSymbols();
    IMG_AddInstrumentFunction(ImageLoad, 0);
   // RTN_AddInstrumentFunction(Routine, 0);

    // Register Fini to be called when the application exits
    PIN_AddFiniFunction(Fini, 0);
    
    // Start the program, never returns
    PIN_StartProgram();
    
    return 0;
}
