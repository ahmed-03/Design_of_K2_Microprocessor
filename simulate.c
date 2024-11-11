#include<stdio.h>
#include"hardware.h"
void lookup(unsigned char inst,char* str[],int *num,bool* ncase);
char prompt(char *FILENAME[]);
int main(int argc,char *argv[]){
	char *str[100];
	char mode,instruction;
	int RA,RB,RO,pc,s,size,sum,temp,num,Rtemp=0;
	bool j,c,D1,D0,sreg,carry,carryreg,enRA,enRB,enRO,ncase=0;
	mode = prompt(argv);
	char ROM[15];
	char stop;

			
			if(mode=='R'){
			printf("Starting Simulator in continuous mode...\n");
			printf("Execution (Register RO output:):\n");
			}
			else if(mode=='S'){
		        printf("Starting Simulator in step-by-step mode...\n");	
			
			}
			else {
			printf("invalid mode exiting program");
			return 0;
			}
			size=romFunction(ROM,argv[1]);
			pc=0;
			while(1){
				instruction=fetch(ROM,pc);
				decode_instruction(instruction,&j,&c,&D1,&D0,&sreg,&s);
				
				decoder(D0,D1,&enRA,&enRB,&enRO);
				temp=s;
				ALU(RA,RB,((temp>>2)&1)==1,&carry,&sum);
				if(enRA==1)
					RA=mux2_1(sum,s,sreg);
				if(enRB==1)
					RB=mux2_1(sum,s,sreg);
				if(enRO==1){
					RO=RA;
					if(mode=='R')
					printf("RO=%d\n",RO);
				}
				if(mode=='S'){
 				lookup(ROM[pc],str,&num,&ncase);
				printf("Instruction %d:",pc);
				printf("%s",str[0]);
				if(instruction==0x20)
					printf("%d",RO);
				if(ncase==1)
					printf("%d",num);
				printf("[Press Enter to continue]");
				stop =getc(stdin);
				printf("\n");
				}
				if(mode=='R')
				for(int i=0; i<100000000;i++);
				pc=counter(s,j,c,carryreg);
				carryreg=carry;
			}





}
char prompt(char *FILENAME[]){

	char mode;

	printf("Loading binary file: %s \n",FILENAME[1]);
	printf("Select on of the following mode\n");

	printf("R-Run in continuos mode\n");
	printf("S-Run step-by-step\n");
	printf("Select mode:");
	scanf(" %c",&mode);
	printf("Loading binary file: %s \n",FILENAME[1]);

	return mode;
}

void lookup(unsigned char inst,char *str[],int *num,bool* ncase){
	*num=inst&7;
	switch(inst){
	
		case 0x0:
			str[0]="RA=RA+RB";
			*ncase=0;
			break;

		case 0x10:
			str[0]="RB=RA+RB";
			*ncase=0;
			break;
		case 0x4:
			str[0]="RA=RA-RB";
			*ncase=0;
			break;
		case 0x14:
			str[0]="RB=RA-RB";
			*ncase=0;
			break;	
		case 0x20:
			str[0]="RO=RA->RO=";
			*ncase=0;
			break;

		case 0x08:

			str[0]="RA=";
			*ncase=1;
			break;
		case 0x19:
			str[0]="RB=";
			*ncase=1;
	  		  break;
		case 0x70:
			str[0]="JC=";
			*ncase=1;
		break;

		case 0xb0:
		str[0]="J=";
		*ncase=1;
		break;
	
	}





}
