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

int main() {

	int result1, result2, result3, result4;

	result1 = Search_in_File("log.txt", "fue:");
	result2 = Search_in_File("log2.txt", "fue:");
	result2 = Search_in_File("log32.txt", "fue:");
	return(0);
}
