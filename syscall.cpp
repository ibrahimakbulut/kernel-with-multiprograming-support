/* SPIM S20 MIPS simulator.
   Execute SPIM syscalls, both in simulator and bare mode.
   Execute MIPS syscalls in bare mode, when running on MIPS systems.
   Copyright (c) 1990-2010, James R. Larus.
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

   Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

   Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation and/or
   other materials provided with the distribution.

   Neither the name of the James R. Larus nor the names of its contributors may be
   used to endorse or promote products derived from this software without specific
   prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
   GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
   OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef _WIN32
#include <unistd.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>

#ifdef _WIN32
#include <io.h>
#endif

#include "spim.h"
#include "string-stream.h"
#include "inst.h"
#include "reg.h"
#include "mem.h"
#include "sym-tbl.h"
#include "syscall.h"

#include "spim-utils.h"
#include "scanner.h"
#include <stdexcept>
#include <time.h>


#include <iostream>

#define BYTES_TO_INST(N) (((N) + BYTES_PER_WORD - 1) / BYTES_PER_WORD * sizeof(instruction*))
		

using namespace std;


class ProcessTableEntry{

	public:
		int ProcessID;
		char ProcessName[30];
		mem_addr ProgramCounter;
		mem_addr NextProgramCounter;
		mem_word *StackPointerAddress;


		int ProcessState;


		int32 Registerss[32];

		int32 L_CCR[4][32], L_CPR[4][32];

		int32 L_HI,L_LO;



	

};
class ProcessTable{

	public:

		int Capacity;
		ProcessTableEntry* Process;
		int number_of_process;

		int Current_process;

		int all_ready;

		ProcessTable(int32 process_numbers);

		void add_process(int id,char name[],mem_addr PC,mem_addr nPC/*,mem_word* stackpointeraddress*/,int state,int32 registers[]);

};







#ifdef _WIN32
/* Windows has an handler that is invoked when an invalid argument is passed to a system
   call. https://msdn.microsoft.com/en-us/library/a9yf33zb(v=vs.110).aspx

   All good, except that the handler tries to invoke Watson and then kill spim with an exception.

   Override the handler to just report an error.
*/

#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>






void myInvalidParameterHandler(const wchar_t* expression,
   const wchar_t* function, 
   const wchar_t* file, 
   unsigned int line, 
   uintptr_t pReserved)
{
  if (function != NULL)
    {
      run_error ("Bad parameter to system call: %s\n", function);
    }
  else
    {
      run_error ("Bad parameter to system call\n");
    }
}

static _invalid_parameter_handler oldHandler;

void windowsParameterHandlingControl(int flag )
{
  static _invalid_parameter_handler oldHandler;
  static _invalid_parameter_handler newHandler = myInvalidParameterHandler;

  if (flag == 0)
    {
      oldHandler = _set_invalid_parameter_handler(newHandler);
      _CrtSetReportMode(_CRT_ASSERT, 0); // Disable the message box for assertions.
    }
  else
    {
      newHandler = _set_invalid_parameter_handler(oldHandler);
      _CrtSetReportMode(_CRT_ASSERT, 1);  // Enable the message box for assertions.
    }
}
#endif





ProcessTable *process_real;

struct StaticBlock {
    StaticBlock(){
        cout << "hello" << endl;
    }
};


static StaticBlock staticBlock;

/*You implement your handler here*/


void context_scheduling(){



	//process tabledaki tum processlerin olusturulmadigi yada olusturulsada programlarin ilgili processlerle 
	//iliskilendirilmedigi durumlarda herhangi bir switch islemi yapilmaz tum programlar icin process table da yer alinmasi
	// beklenir cunku. Bu asamada yalnizca process table ekrana bastirilir.

	// Bu kisim aktif edilirse fork ve execv islemleri tamamlanana kadarki process table uzerindeki degisiklikler gorulebilir.

	/*if(process_real->all_ready<4 && process_real->all_ready!=0){


		cout<<"\n\n--------------------ProcessTable--------------------\n";
		cout<<"ProcessID"<<"---ProcessName---ProgramCounter---Stack PointerAddress\n";


		for(int i=0;i<process_real->number_of_process;++i){
			cout<<process_real->Process[i].ProcessID<<"----"<<process_real->Process[i].ProcessName<<"----"<<process_real->Process[i].ProgramCounter<<"----"<<process_real->Process[i].Registerss[REG_SP]<<endl;
		}
		cout<<"\n\n";
	}   */


	if(process_real->all_ready==process_real->number_of_process){


		// tum programlarin hafizaya yuklendigi ve process tabledaki processlerin baslamaya 
		// hazir oldugu durumda interrupt geldiginde yapilacaklar. Bu kisim bir kere calistirilir.

		cout<<"\n\n--------------------ProcessTable--------------------\n";
		cout<<"ProcessID"<<"---ProcessName---ProgramCounter---Stack PointerAddress\n";


		for(int i=0;i<process_real->number_of_process;++i){
			cout<<process_real->Process[i].ProcessID<<"----"<<process_real->Process[i].ProcessName<<"----"<<process_real->Process[i].ProgramCounter<<"----"<<process_real->Process[i].Registerss[REG_SP]<<endl;
		}
		cout<<"Process 1 will be running";
		cout<<"\n\n"; 


		process_real->Process[0].ProgramCounter=PC;
		process_real->Process[0].NextProgramCounter=nPC;

		for(int i=0;i<32;++i){

			process_real->Process[0].Registerss[i]=R[i];
		}

		for(int i=0;i<4;++i){

			for(int j=0;j<32;++j){

				process_real->Process[0].L_CCR[i][j]=CCR[i][j];
			}
		}


		for(int i=0;i<4;++i){

			for(int j=0;j<32;++j){

				process_real->Process[0].L_CPR[i][j]=CPR[i][j];
			}
		}

		process_real->Process[0].L_HI=HI;
		process_real->Process[0].L_LO=LO;



		
		PC=process_real->Process[1].ProgramCounter;
		nPC=process_real->Process[1].NextProgramCounter;
		
		process_real->all_ready=0;

		process_real->Current_process=1;


		}
		else{



		if(process_real->all_ready==0){


		// Tum programlar hafizaya yuklendikten sonra ve en az bir processin calismaya 
		// basladiktan sonraki arada gelen switch cagrilarinda uygulanacak durumlar. 





		for(int i=0;i<32;++i){

			process_real->Process[process_real->Current_process].Registerss[i]=R[i];
		}

		for(int i=0;i<4;++i){

			for(int j=0;j<32;++j){

				process_real->Process[process_real->Current_process].L_CCR[i][j]=CCR[i][j];
			}
		}


		for(int i=0;i<4;++i){

			for(int j=0;j<32;++j){

				process_real->Process[process_real->Current_process].L_CPR[i][j]=CPR[i][j];
			}
		}


		process_real->Process[process_real->Current_process].NextProgramCounter=nPC;
		process_real->Process[process_real->Current_process].ProgramCounter=PC;
		

		process_real->Process[process_real->Current_process].L_HI=HI;
		process_real->Process[process_real->Current_process].L_LO=LO;


		int i;

		if(process_real->Current_process+1==process_real->number_of_process)
			i=0;
		else
			i=process_real->Current_process+1;

		for(; ;){

			if(process_real->Process[i].ProcessState!=10){
				process_real->Current_process=i;
				break;
			}

				++i;

				if(i==process_real->number_of_process)
					i=0;
		}


			for(int i=0;i<32;++i){

				R[i]=process_real->Process[process_real->Current_process].Registerss[i];

			}

			for(int i=0;i<4;++i){

				for(int j=0;j<32;++j){

					CCR[i][j]=process_real->Process[process_real->Current_process].L_CCR[i][j];
				}
			}

			for(int i=0;i<4;++i){

				for(int j=0;j<32;++j){

					CPR[i][j]=process_real->Process[process_real->Current_process].L_CPR[i][j];
				}
			}


			HI=process_real->Process[process_real->Current_process].L_HI;
			LO=process_real->Process[process_real->Current_process].L_LO;



			nPC=process_real->Process[process_real->Current_process].NextProgramCounter;

			PC=process_real->Process[process_real->Current_process].ProgramCounter;

			cout<<"\n\n--------------------ProcessTable--------------------\n";
			cout<<"ProcessID"<<"---ProcessName---ProgramCounter---Stack PointerAddress\n";

			for(int i=0;i<process_real->number_of_process;++i){
				cout<<process_real->Process[i].ProcessID<<"---"<<process_real->Process[i].ProcessName<<"---"<<process_real->Process[i].ProgramCounter<<"---"<<process_real->Process[i].Registerss[REG_SP]<<endl;
			}
			cout<<"Process "<<process_real->Current_process<<" will be running";
			cout<<"\n\n"; 

		}

		}

		return;

}


void SPIM_timerHandler()
{

try{

context_scheduling();

}
   catch ( exception &e )
   {
      cerr <<  endl << "Caught: " << e.what( ) << endl;

   };

  
	//}

}
/* Decides which syscall to execute or simulate.  Returns zero upon
   exit syscall and non-zero to continue execution. */
int
do_syscall ()
{
#ifdef _WIN32
    windowsParameterHandlingControl(0);
#endif

  /* Syscalls for the source-language version of SPIM.  These are easier to
     use than the real syscall and are portable to non-MIPS operating
     systems. */

  switch (R[REG_V0])
    {
    case PRINT_INT_SYSCALL:
      write_output (console_out, "%d", R[REG_A0]);
      break;

    case PRINT_FLOAT_SYSCALL:
      {
	float val = FPR_S (REG_FA0);

	write_output (console_out, "%.8f", val);
	break;
      }

    case PRINT_DOUBLE_SYSCALL:
      write_output (console_out, "%.18g", FPR[REG_FA0 / 2]);
      break;

    case PRINT_STRING_SYSCALL:
      write_output (console_out, "%s", mem_reference (R[REG_A0]));
      break;

    case READ_INT_SYSCALL:
      {
	static char str [256];

	read_input (str, 256);
	R[REG_RES] = atol (str);
	break;
      }

    case READ_FLOAT_SYSCALL:
      {
	static char str [256];

	read_input (str, 256);
	FPR_S (REG_FRES) = (float) atof (str);
	break;
      }

    case READ_DOUBLE_SYSCALL:
      {
	static char str [256];

	read_input (str, 256);
	FPR [REG_FRES] = atof (str);
	break;
      }

    case READ_STRING_SYSCALL:
      {
	read_input ( (char *) mem_reference (R[REG_A0]), R[REG_A1]);
	data_modified = true;
	break;
      }

    case SBRK_SYSCALL:
      {
	mem_addr x = data_top;
	expand_data (R[REG_A0]);
	R[REG_RES] = x;
	data_modified = true;
	break;
      }

    case PRINT_CHARACTER_SYSCALL:
      write_output (console_out, "%c", R[REG_A0]);
      break;

    case READ_CHARACTER_SYSCALL:
      {
	static char str [2];

	read_input (str, 2);
	if (*str == '\0') *str = '\n';      /* makes xspim = spim */
	R[REG_RES] = (long) str[0];
	break;
      }

    case EXIT_SYSCALL:
      spim_return_value = 0;
      return (0);

    case EXIT2_SYSCALL:
      spim_return_value = R[REG_A0];	/* value passed to spim's exit() call */
      return (0);

    case OPEN_SYSCALL:
      {
#ifdef _WIN32
        R[REG_RES] = _open((char*)mem_reference (R[REG_A0]), R[REG_A1], R[REG_A2]);
#else
	R[REG_RES] = open((char*)mem_reference (R[REG_A0]), R[REG_A1], R[REG_A2]);
#endif
	break;
      }

    case READ_SYSCALL:
      {
	/* Test if address is valid */
	(void)mem_reference (R[REG_A1] + R[REG_A2] - 1);
#ifdef _WIN32
	R[REG_RES] = _read(R[REG_A0], mem_reference (R[REG_A1]), R[REG_A2]);
#else
	R[REG_RES] = read(R[REG_A0], mem_reference (R[REG_A1]), R[REG_A2]);
#endif
	data_modified = true;
	break;
      }

    case WRITE_SYSCALL:
      {
	/* Test if address is valid */
	(void)mem_reference (R[REG_A1] + R[REG_A2] - 1);
#ifdef _WIN32
	R[REG_RES] = _write(R[REG_A0], mem_reference (R[REG_A1]), R[REG_A2]);
#else
	R[REG_RES] = write(R[REG_A0], mem_reference (R[REG_A1]), R[REG_A2]);
#endif
	break;
      }

    case CLOSE_SYSCALL:
      {
#ifdef _WIN32
	R[REG_RES] = _close(R[REG_A0]);
#else
	R[REG_RES] = close(R[REG_A0]);
#endif
	break;
      }

     //initiliziation of Process Table. Process tableda init process haric kac process olacagi parametre olarak gonderilir.
    case INIT:
    {

    int process_all=R[REG_A0];
    process_real=new ProcessTable(process_all);

    break;
    }



    //Bu sistem cagrisi init processin kopyasini olusturur. Bu kopya process daha sonra memory e yuklenecek 
    // bir programi calistirmak uzere gorevlendirilecektir.
    case FORK_MY:
	{
	



	//isim gecici sekilde init olarak veriliyor daha sonra bu isim calistirilacak programin adini alacaktir.
	char init[30]="init";
	process_real->add_process(process_real->number_of_process,init,process_real->Process[0].ProgramCounter,process_real->Process[0].NextProgramCounter/*,process_real->Process[0].StackPointerAddress*/,0,R);
	

	break;

	}

    case EXECVE:
	{

	char *sub_program_name= (char *) malloc(sizeof(char)*30);
	char *sub_program_name2= (char *) malloc(sizeof(char)*30);

	strcpy(sub_program_name,(char *) mem_reference(R[REG_A1]));
	strcpy(sub_program_name2,sub_program_name);

	initialize_symbol_table ();
	read_assembly_file(strcat(sub_program_name,".s"));
	pop_scanner();



	strcpy(process_real->Process[R[REG_A0]].ProcessName,sub_program_name2);
	process_real->Process[R[REG_A0]].ProcessState=1;
	process_real->all_ready+=1;


	process_real->Process[R[REG_A0]].ProgramCounter=find_symbol_address ("main");


	break;
	}


	// bu sistem cagrisi programlar tarafindan bittiklerini haber vermek icin kullanilir.
	case PROCESS_EXIT:
	{

	//guncel processin bittigini belirtmek icin ProcessState degiskenine 10 degeri atanilir.
	process_real->Process[process_real->Current_process].ProcessState=10;

	break;	
	}


	//Process Tabledaki tum processler sonlandi ise posix exit sistem cagrisi ile cikilir.
	case WAIT_PID:
	{

	int finish_all_flag=1;

	for(int i=1;i<process_real->number_of_process;++i){

		if (process_real->Process[i].ProcessState!=10){
			finish_all_flag=0;
			break;
		}
	}


	if(finish_all_flag==1)
		exit(1);

	break;
	}


	//rastgele sayi uretmek icin cagrilan sistem cagrisi. Uretilen sayi a0 registerina yazilir.
	case RANDOM_GENERATOR:
	{


	srand(time(NULL));

    int random_number=  rand() % 3;

    R[REG_A0]=random_number;

    	break;

	}


    default:
      run_error ("Unknown system call: %d\n", R[REG_V0]);
      break;
    }

#ifdef _WIN32
    windowsParameterHandlingControl(1);
#endif
  return (1);
}


void
handle_exception ()
{
  if (!quiet && CP0_ExCode != ExcCode_Int)
    error ("Exception occurred at PC=0x%08x\n", CP0_EPC);

  exception_occurred = 0;
  PC = EXCEPTION_ADDR;

  switch (CP0_ExCode)
    {
    case ExcCode_Int:
      break;

    case ExcCode_AdEL:
      if (!quiet)
	error ("  Unaligned address in inst/data fetch: 0x%08x\n", CP0_BadVAddr);
      break;

    case ExcCode_AdES:
      if (!quiet)
	error ("  Unaligned address in store: 0x%08x\n", CP0_BadVAddr);
      break;

    case ExcCode_IBE:
      if (!quiet)
	error ("  Bad address in text read: 0x%08x\n", CP0_BadVAddr);
      break;

    case ExcCode_DBE:
      if (!quiet)
	error ("  Bad address in data/stack read: 0x%08x\n", CP0_BadVAddr);
      break;

    case ExcCode_Sys:
      if (!quiet)
	error ("  Error in syscall\n");
      break;

    case ExcCode_Bp:
      exception_occurred = 0;
      return;

    case ExcCode_RI:
      if (!quiet)
	error ("  Reserved instruction execution\n");
      break;

    case ExcCode_CpU:
      if (!quiet)
	error ("  Coprocessor unuable\n");
      break;

    case ExcCode_Ov:
      if (!quiet)
	error ("  Arithmetic overflow\n");
      break;

    case ExcCode_Tr:
      if (!quiet)
	error ("  Trap\n");
      break;

    case ExcCode_FPE:
      if (!quiet)
	error ("  Floating point\n");
      break;

    default:
      if (!quiet)
	error ("Unknown exception: %d\n", CP0_ExCode);
      break;
    }
}




//Process Table Constructor. Process Table ilk olusturuldugunda 0 id ile init process yerlestirilir.
ProcessTable::ProcessTable(int32 process_numbers){

	Capacity=process_numbers;

	Current_process=0;
	Process= (ProcessTableEntry*) malloc(process_numbers*sizeof(ProcessTableEntry));


	Process[0].ProcessID=0;
	strcpy(Process[0].ProcessName,"init");
	Process[0].ProgramCounter=PC;
	Process[0].ProcessState=0;
	Process[0].NextProgramCounter=nPC;


	all_ready=1;

		
	memcpy(Process[0].L_CCR,CCR,sizeof(int32)*4*32);
	memcpy(Process[0].L_CPR,CPR,sizeof(int32)*4*32);

	Process[0].L_HI=HI;
	Process[0].L_LO=LO;
		

	for(int i=0;i<32;++i){

		Process[0].Registerss[i]=R[i];

	} 

	number_of_process=1;
}
void ProcessTable::add_process(int id,char name[],mem_addr PC,mem_addr nPC,int state,int32 registers[]){


	Process[number_of_process].ProcessID=id;
	strcpy(Process[number_of_process].ProcessName,name);
	Process[number_of_process].ProgramCounter=PC;
	Process[number_of_process].ProcessState=state;
	Process[number_of_process].NextProgramCounter=nPC;



	


		
	memcpy(Process[number_of_process].L_CCR,Process[0].L_CCR,sizeof(int32)*4*32);
	memcpy(Process[number_of_process].L_CPR,Process[0].L_CPR,sizeof(int32)*4*32);

	Process[number_of_process].L_HI=Process[0].L_HI;
	Process[number_of_process].L_LO=Process[0].L_LO;
		

	for(int i=0;i<32;++i){

		Process[number_of_process].Registerss[i]=registers[i];

	}

	number_of_process+=1; 
	return;

	}




