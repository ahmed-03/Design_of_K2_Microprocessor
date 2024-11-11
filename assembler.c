#include <stdio.h>
#include <string.h>

int main(int nums,char *FILENAME[]) {
	FILE *mystring, *mybinary;
	char temp[100];
	strcpy(temp,FILENAME[1]);
	temp[strlen(temp)-1]='n';
	temp[strlen(temp)-2]='i';
	temp[strlen(temp)-3]='b';
	mystring = fopen(FILENAME[1], "r");
	mybinary = fopen(temp, "wb");

	char fptr[100];
	unsigned char bins[100];
	int i = 0;

	if (mystring == NULL) {
		printf("Could not open the file\n");
		return 1;
	}
	if (mybinary == NULL) {
		printf("Could not create the output file\n");
		fclose(mystring);
		return 1;
	}
	printf("Starting Assembler...\nReading file: %s\n",FILENAME[1]);
	while (fgets(fptr, 100, mystring)) {

		if (strcmp(fptr, "RA=RA+RB\n") == 0)
			bins[i] = 0b00000000;
		else if (strcmp(fptr, "RB=RA+RB\n") == 0)
			bins[i] = 0b00010000;
		else if (strcmp(fptr, "RA=RA-RB\n") == 0)
			bins[i] = 0b00000100;
		else if (strcmp(fptr, "RB=RA-RB\n") == 0)
			bins[i] = 0b00010100;
		else if (strcmp(fptr, "R0=RA\n") == 0)
			bins[i] = 0b00100000;
		else if (fptr[3] >= '0' && fptr[3] <= '9' && fptr[2] == '=' && fptr[4] == '\n') {
			if (strncmp(fptr, "RA=", 3) == 0)
				bins[i] = 0b00001000 + (fptr[3] - '0');
			else if (strncmp(fptr, "RB=", 3) == 0)
				bins[i] = 0b00011000 + (fptr[3] - '0');
			else if (strncmp(fptr, "JC=", 3) == 0)
				bins[i] = 0b01110000 + (fptr[3] - '0');
		} else if (fptr[2] >= '0' && fptr[2] <= '9' && fptr[1] == '=' && fptr[3] == '\n') {
			bins[i] = 0b10110000 + (fptr[2] - '0');
		} else {
			printf("Invalid operation\n");
			continue;
		}
		i++;
		printf("Line %d:",i);
		int y=0;
		while(fptr[y]!='\n'){
		printf("%c",fptr[y]);
		y++;
		}
		printf("-> Machine Code:");
		printf("%.8b",bins[i-1]);
		printf("\n");
	}
	fclose(mystring);

	fwrite(bins, sizeof(unsigned char), i, mybinary);
	fclose(mybinary);
	
	printf("Successfully generated output file: %s\n",temp);

	return 0;
}
