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
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include "pin.H"

ofstream OutFile;

// The running count of instructions is kept here
// make it static to help the compiler optimize docount
static UINT64 icount = 0;
int notin_405dae=1;    
int check_405dae=0;

// Print a memory read record
VOID RecordMemRead(VOID * ip, VOID * addr)
{
    //fprintf(trace,"%p: R %p\n", ip, addr);
    cout<<"\n\t Inst: "<<ip<<" read-addr: "<<addr;
}

// Print a memory write record
VOID RecordMemWrite(VOID * ip, VOID * addr)
{
//    fprintf(trace,"%p: W %p\n", ip, addr);
    cout<<"\n\t Inst: "<<ip<<" write-addr: "<<addr;
}


// This function is called before every instruction is executed
VOID docount(VOID* ip) {
if(notin_405dae ) icount++; }//cout<<"\n\t Inst: "<<ip; }


// Pin calls this function every time a new instruction is encountered
VOID Instruction(INS ins, VOID *v)
{
    // Insert a call to docount before every instruction, no arguments are passed
    INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)docount,IARG_INST_PTR, IARG_END);
    string disassemble=INS_Disassemble(ins);
    cout<<"\n\t INS-disassemble: "<<disassemble;
    //if( (disassemble=="call 0x405dae") || (disassemble=="call 0x405e86") )
   // (disassemble=="call 0x") ||
   //4060fa; 405cea;405e6e;405f40;405e86;401264;405dea;406114;405dae;405f40;405d00
   
    if( (disassemble=="call 0x405dae") || (disassemble=="call 0x405e86") || (disassemble=="call 0x4061dc") || 
    (disassemble=="call 0x405cea") ||  (disassemble=="call 0x405d00") ||  (disassemble=="call 0x405e6e") ||  
     (disassemble=="call 0x401264") || (disassemble=="call 0x405f40") || (disassemble=="call 0x405dea") || 
     (disassemble=="call 0x4060fa") || (disassemble=="call 0x405cea") || (disassemble=="call 0x406114") || (disassemble=="call 0x405d00") )    
    {
    	cout<<"\n\t NOTE: call 0x405dae. notin_405dae-before: "<<notin_405dae<<" icount "<<icount<<" disassemble "<<disassemble;
    	notin_405dae=0;
    	check_405dae=1;
    	cout<<"\n\t NOTE: call 0x405dae. notin_405dae-after: "<<notin_405dae<<" icount "<<icount<<" disassemble "<<disassemble;	
    }
    if(check_405dae)
    {
    	if( (disassemble=="ret ") || ( disassemble=="pop rbp" ) )
    	{
	    	cout<<"\n\t NOTE: ret to call 0x405dae. notin_405dae-before: "<<notin_405dae<<" icount "<<icount<<" disassemble "<<disassemble;
	    	notin_405dae=1;
	    	check_405dae=0;
	    	cout<<"\n\t NOTE: ret to call 0x405dae. notin_405dae-after: "<<notin_405dae<<" icount "<<icount<<" disassemble "<<disassemble;    	
    	}
    }
 
 /*    UINT32 memOperands = INS_MemoryOperandCount(ins);

    // Iterate over each memory operand of the instruction.
    for (UINT32 memOp = 0; memOp < memOperands; memOp++)
    {
        if (INS_MemoryOperandIsRead(ins, memOp))
        {
            INS_InsertPredicatedCall(
                ins, IPOINT_BEFORE, (AFUNPTR)RecordMemRead,
                IARG_INST_PTR,
                IARG_MEMORYOP_EA, memOp,
                IARG_END);
        }
        // Note that in some architectures a single memory operand can be 
        // both read and written (for instance incl (%eax) on IA-32)
        // In that case we instrument it once for read and once for write.
        if (INS_MemoryOperandIsWritten(ins, memOp))
        {
            INS_InsertPredicatedCall(
                ins, IPOINT_BEFORE, (AFUNPTR)RecordMemWrite,
                IARG_INST_PTR,
                IARG_MEMORYOP_EA, memOp,
                IARG_END);
        }
    } */
}    

typedef struct routine_info
{
	UINT64 primary_count;
	UINT64 secondary_count;
	UINT64 call_count;
	string sym_name;
	string rtn_name;
	int bypass;
	UINT64 execute_count;
	routine_info* _next; 
} rtn_info;

rtn_info* rtn_list=0;

VOID rtn_count(rtn_info* curr_rtn_info,VOID *v)
{
	curr_rtn_info->call_count+=1;
}

VOID Routine( RTN rtn,rtn_info* curr_rtn_info, VOID *v )
{

	int rtn_primary_count=0;
	RTN_Open(rtn);
	// 263552
	//264160
	for(INS ins=RTN_InsHead(rtn); INS_Valid(ins); ins= INS_Next(ins) )
		rtn_primary_count++;
 
	int rtn_secondary_count=0;
	for(INS ins=RTN_InsHead(rtn); INS_Valid(ins); ins= INS_Next(ins) )
	{
	
		if(INS_HasFallThrough(ins) )
		{
			rtn_secondary_count++;
			string disassemble=INS_Disassemble(ins);			
			cout<<"\n\t\t\t RTN-Ins: "<<disassemble;
		}
	
	}
	curr_rtn_info->primary_count=rtn_primary_count;
	curr_rtn_info->secondary_count=rtn_secondary_count;
	cout<<"\n\t\t RTN: "<<RTN_Name(rtn)<<" primary count: "<<rtn_primary_count<<" secondary count "<<rtn_secondary_count;
	cout<<endl;
	RTN_Close(rtn);

}


 
VOID ImageLoad(IMG img, VOID *v)
{
 	cout<< endl<<"loading "<< IMG_Name(img).c_str()<<endl;
 	
 
	for( SEC sec=IMG_SecHead(img); SEC_Valid(sec) ; sec=SEC_Next(sec) )
	{
		cout<<"\n\t In Section: "<<SEC_Name(sec);
		
		for( RTN rtn=SEC_RtnHead(sec); RTN_Valid(rtn); rtn= RTN_Next(rtn) )
		{
			RTN_Open(rtn);
			rtn_info* curr_rtn_info=new rtn_info;
			curr_rtn_info->call_count=0;			
			curr_rtn_info->bypass=0;
			curr_rtn_info->_next=rtn_list;
			rtn_list=curr_rtn_info;
			curr_rtn_info->execute_count=0;
			if( SYM_Valid( RTN_Sym(rtn) ) )
			{
				string sym_name=PIN_UndecorateSymbolName( SYM_Name( RTN_Sym(rtn) ) , UNDECORATION_NAME_ONLY ) ;
				curr_rtn_info->sym_name=sym_name;
				curr_rtn_info->rtn_name=RTN_Name(rtn);
				ADDRINT rtn_address= RTN_Address(rtn);
				cout<<"\n\t\t RTN: "<<RTN_Name(rtn)<<" SYM "<<sym_name<<" rtn_address "<<rtn_address;

				RTN_InsertCall(rtn, IPOINT_BEFORE, (AFUNPTR) rtn_count,IARG_PTR,curr_rtn_info,IARG_END);
			}
			RTN_Close(rtn);
			Routine(rtn,curr_rtn_info,0);

		}
	
	}



}					



KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool",
    "o", "InstCountSummary.log", "specify output file name");

// This function is called when the application exits
VOID Fini(INT32 code, VOID *v)
{
	//Write to a file since cout and cerr maybe closed by the application
	OutFile.setf(ios::showbase);
	OutFile<<"\n\t Opening now at Fini! \n";
	UINT64 recount_primary=0,recount_secondary=0;
	for(rtn_info* curr_rtn_info=rtn_list; curr_rtn_info; curr_rtn_info=curr_rtn_info->_next )	
	{ 
		if( curr_rtn_info->call_count  )
		{
			int primary_local_count=(curr_rtn_info->call_count)*(curr_rtn_info->primary_count);
			int secondary_local_count=(curr_rtn_info->call_count)*(curr_rtn_info->secondary_count);		
	 		recount_primary+=primary_local_count;	recount_secondary+=secondary_local_count;
			OutFile << "\t Procedure "<<curr_rtn_info->sym_name<<" calls "<<curr_rtn_info->call_count<<" recount_primary "<<recount_primary<<" recount_secondary "<<recount_secondary<<endl;
		}
    	}
    	OutFile<<"\n\t iCount: "<<icount<<endl<<endl;
    	OutFile<<"\n\t Closing now at Fini ! \n";
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
    INS_AddInstrumentFunction(Instruction, 0);
    PIN_InitSymbols();
    IMG_AddInstrumentFunction(ImageLoad, 0);
  //  RTN_AddInstrumentFunction(Routine, 0);

    // Register Fini to be called when the application exits
    PIN_AddFiniFunction(Fini, 0);
    
    // Start the program, never returns
    PIN_StartProgram();
    
    return 0;
}
