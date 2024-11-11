//#include<hardware.h>
#include<stdio.h>
#include<stdbool.h>
#include"hardware.h"


int mux2_1(int iA,int iB, bool bSel){
	if (bSel==0)
		return iA;
	else
		return iB;
}

void translate(char inst,bool * j,bool * c,bool * D1, bool * D0, bool* sreg, int* scos){
	*j =(inst>>7)& 1;
	*c=(inst>>6) &1;
	*D1=(inst>>5)&1;
	*D0=(inst>>4)&1;
	*sreg=(inst>>3)&1;
	*scos=inst& 7;
}

int counter(int s,bool j, bool c, bool carry){
	bool load = (carry & c) || j;
	static int count =0;

	if (count>14)
		count=0;

	if (load ==1)
		count=s;
	else 
		count+=1;
	printf("\n the count value is %d\n",count);
	return count;
}



void instruction_memory(char inst[],bool * j,bool * c,bool * D1, bool * D0, bool * sreg, int* scos,int pc){
	bool ji,ci,D1i,D0i;
	bool sregi;
	int scosi;

	translate(inst[pc],&ji,&ci,&D1i,&D0i,&sregi,&scosi);

	*j=ji;
	*c=ci;
	*D1=D1i;
	*D0=D0i;
	*sreg=sregi;
	*scos=scosi;
}

int romFunction(unsigned char mem[15]) {
	int i=0;
	FILE *fptr=fopen("fibonacci.bin","rb");
	if(fptr==NULL) {
		printf("Could not open the file\n");
	}
	fread(mem,1,15,fptr);
	do{
		i++;
	}while(mem[i]==0x0 || mem[i]== 0x10 || mem[i]== 0x4  || mem[i]== 0x14 || mem[i]== 0x20 || (mem[i]&0xf8)==0x8  || (mem[i]&0xf8)==0x18  || (mem[i]&0xf8)==0x70  || (mem[i]&0xf8)==0xb0);                          

	fclose(fptr);
	return i;
}

void ALU(int RA, int RB, bool S, bool *carry, int *sum,char inst) {
	if( inst== 0x0 || inst== 0x10 || inst== 0x4 || inst== 0x14 ){
		if (S == 0) {
			if ((RA+RB) > 0b1111) {
				*carry = 1;
				*sum = (RA+RB) & 0b1111;
			} else {
				*carry = 0;
				*sum = (RA+RB) & 0b1111;
			}

			if ((RA+RB) > 0b1111) {
				*carry = 1;
				*sum = (RA+RB) & 0b1111;
			} else {
				*carry = 0;
				*sum = (RA+RB) & 0b1111;
			}
		}
		else {

			*carry = 0;
			*sum = (RA-RB) & 0b1111;
		}

	}
}

void decoder(bool D0, bool D1, bool *enRA, bool *enRB, bool *enRO) {
	int caseValue = (D0 << 1) | D1;
	switch (caseValue) {
		case 0:
			*enRA = 1; *enRB = 0; *enRO = 0;
			break;
		case 1:
			*enRA = 0; *enRB = 1; *enRO = 0;
			break;
		case 2:
			*enRA = 0; *enRB = 0; *enRO = 1;
			break;
		case 3:
			*enRA = 0; *enRB = 0; *enRO = 0;
			break;
		default:
			break;
	}
}

