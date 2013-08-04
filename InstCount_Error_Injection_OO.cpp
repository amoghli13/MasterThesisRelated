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
#include <set>
#include "pin.H"

ofstream OutFile;

// The running count of instructions is kept here
// make it static to help the compiler optimize docount
static UINT64 icount = 0;
int notin_405dae=1;    
int check_405dae=0;
 
//UINT64* monitor_address; 
set<void* > monitor_address;
 ////////////////////////
const string& Target2RtnName(ADDRINT target)
{
  const string & name = RTN_FindNameByAddress(target);

  if (name == "")
      return *new string("[Unknown routine]");
  else
      return *new string(name);
}
/////////
void A_RegisterAddr(void *addr)
{
	monitor_address.insert(addr);
  cout<<"\n\t Adding address: "<<addr;
}
/////////////////////////////

static void A_DoMem(bool isStore, void *addr, ADDRINT pc)
{

	if( monitor_address.find(addr) !=monitor_address.end() )
	{
		
		cout<<"\n\t Address in use: "<<addr<<" PC "<<pc;
	}
}



/////////////

static void I_Trace(TRACE trace, void *v)
{

    //FIXME if (PIN_IsSignalHandler()) {Sequence_ProcessSignalHandler(head)};
    	if( RTN_Valid( TRACE_Rtn(trace) ) )
		cout<<"\n\t This trace is in: "<<( RTN_Name( TRACE_Rtn(trace) ) );
	for(BBL bbl = TRACE_BblHead(trace); BBL_Valid(bbl); bbl = BBL_Next(bbl)) 
	{

		//INS tail = BBL_InsTail(bbl);

		// All memory reads/writes
		for( INS ins = BBL_InsHead(bbl); INS_Valid(ins); ins = INS_Next(ins) ) 
		{

		    if( INS_IsMemoryRead(ins)
		        || INS_HasMemoryRead2(ins)
		        || INS_IsMemoryWrite(ins)
		    ) 
		    {
		      cout<<"\n\t Ins_mem: "<<INS_Address(ins)<<" disassemble: "<<INS_Disassemble(ins)<<endl;
		        INS_InsertCall(ins, IPOINT_BEFORE,
		                       (AFUNPTR)A_DoMem,
		                       IARG_BOOL, INS_IsMemoryWrite(ins),
		                       (INS_IsMemoryWrite(ins) ? IARG_MEMORYWRITE_EA : (INS_IsMemoryRead(ins) ? IARG_MEMORYREAD_EA : IARG_MEMORYREAD2_EA)),
		                       IARG_INST_PTR,
		                       IARG_END);
		    }
		}
	}
}


////////////////////////////////////////////////////////////////
 
// This function is called before every instruction is executed
VOID docount( int inst_bypass,ADDRINT sp,void* ip)
{
	//cout<<"\n\t Inst "<<ip<<" bypass: "<<inst_bypass<<" icount "<<icount<<" stack-pointer: "<<sp;
	if(inst_bypass) 
		icount++;
}
 
// Pin calls this function every time a new instruction is encountered
VOID Instruction(INS ins, VOID *v)
{
    // Insert a call to docount before every instruction, no arguments are passed
  //   INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)docount,IARG_END);
    string disassemble=INS_Disassemble(ins);
    cout<<"\n\t INS-disassemble: "<<disassemble<<"\t inst_ptr_ "<<INS_Address(ins)<<"\t next_inst "<<INS_NextAddress(ins)<<endl;
    //if( (disassemble=="call 0x405dae") || (disassemble=="call 0x405e86") )
   // (disassemble=="call 0x") ||
   //4060fa; 405cea;405e6e;405f40;405e86;401264;405dea;406114;405dae;405f40;405d00
   
    if( (disassemble=="call 0x405dae") || (disassemble=="call 0x405e86") || (disassemble=="call 0x4061dc") || 
       (disassemble=="call 0x405cea") ||  (disassemble=="call 0x405d00") ||  (disassemble=="call 0x405e6e") ||  
       (disassemble=="call 0x401264") || (disassemble=="call 0x405f40") || (disassemble=="call 0x405dea") || 
       (disassemble=="call 0x4060fa") || (disassemble=="call 0x405cea") || (disassemble=="call 0x406114") ||
       (disassemble=="call 0x405d00") || ( disassemble=="call 0x405a5e") || ( disassemble=="call 0x608160") ||
       (disassemble=="call 0x4041e6") || ( disassemble=="call 0x403e27") )    
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
     INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)docount,IARG_UINT32,notin_405dae,IARG_REG_VALUE, REG_STACK_PTR,IARG_INST_PTR,IARG_END);
   
 
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
 
 
VOID ImageLoad(IMG img, VOID *v)
{
 	cout<< endl<<"loading "<< IMG_Name(img).c_str()<<endl;
 	
 
/*	for( SEC sec=IMG_SecHead(img); SEC_Valid(sec) ; sec=SEC_Next(sec) )
	{
		cout<<"\n\t In Section: "<<SEC_Name(sec);
		
		for( RTN rtn=SEC_RtnHead(sec); RTN_Valid(rtn); rtn= RTN_Next(rtn) )
		{
			RTN_Open(rtn);
			rtn_info* curr_rtn_info=new rtn_info;
			curr_rtn_info->call_count=0;			
 			curr_rtn_info->_next=rtn_list;
			rtn_list=curr_rtn_info;
 
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

		}
	
	}
*/
	for( SYM sym = IMG_RegsymHead(img); SYM_Valid(sym); sym = SYM_Next(sym) )
	{

		RTN rtn1=RTN_FindByName(img, SYM_Name(sym).c_str()); 
		if( RTN_Valid(rtn1) );
	 	{
	 		 RTN_Open(rtn1);
	 		 ADDRINT my_addr=RTN_Address(rtn1);
	 		 char* sym_name=(char *)SYM_Name(sym).c_str();
			cout <<"\n\t Sym name: "<<sym_name<<" addr"<<my_addr<<endl;
			bool has_rand=strcmp(sym_name,"rand");
			if(!has_rand)
			{
				cout<<"\n\t +++Sym_name: "<<sym_name<<" has_rand "<<has_rand<<endl;
			}
	 
	  		if( (my_addr==4218090) || (!has_rand)  ) //|| ( strstr(sym_name.c_str(),"rand" ) ) )
			{
				cout <<"\n\t REGISTERING Sym name: "<<sym_name<<" addr"<<my_addr<<" has_rand "<<has_rand<<endl;			
			      RTN_InsertCall(rtn1, IPOINT_BEFORE,
					     (AFUNPTR)A_RegisterAddr,
					     IARG_G_ARG0_CALLEE,
					     IARG_END);		
		
			}
		
	 		RTN_Close(rtn1);
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
 	for(rtn_info* curr_rtn_info=rtn_list; curr_rtn_info; curr_rtn_info=curr_rtn_info->_next )	
	{ 
		if( curr_rtn_info->call_count  )
		{
 			OutFile << "\t Procedure "<<curr_rtn_info->sym_name<<" calls "<<curr_rtn_info->call_count<<endl;
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
	//monitor_address= new UINT64[100];
    OutFile.open(KnobOutputFile.Value().c_str());

    // Register Instruction to be called to instrument instructions
    INS_AddInstrumentFunction(Instruction, 0);
    PIN_InitSymbols();
    IMG_AddInstrumentFunction(ImageLoad, 0);
    TRACE_AddInstrumentFunction(I_Trace, 0);    
  //  RTN_AddInstrumentFunction(Routine, 0);

    // Register Fini to be called when the application exits
    PIN_AddFiniFunction(Fini, 0);
    
    // Start the program, never returns
    PIN_StartProgram();
    
    return 0;
}
