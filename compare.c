#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void Usage(char *filename) {
	printf("Usage: %s <file> <string>\n", filename);
}


int Search_in_File(char *fname, char *str) {
	FILE *fp;
	int line_num = 1;
	int find_result = 0;
	char temp[512];
	
	//gcc users
	if((fp = fopen(fname, "r")) == NULL) {
		return(-1);
	}
	char* string; 
	int number;
	while(fgets(temp, 512, fp) != NULL) {
		if((string = strstr(temp, str)) != NULL) {

			number = atoi(string+5);
			printf("%d\n", number );
			return number;
		}
		line_num++;
	}
	
	//Close the file if still open.
	if(fp) {
		fclose(fp);
	}
   	return(0);
}


//Our main function.
int main(int argc, char *argv[]) {
	int result, errno;

	if(argc < 3 || argc > 3) {
		Usage(argv[0]);
		exit(1);
	}

	system("clear");

	result = Search_in_File("log.txt", "fue:");
	if(result == -1) {
		perror("Error");
		printf("Error number = %d\n", errno);
		exit(1);
	}
	return(0);
}