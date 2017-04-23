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

  result1 = Search_in_File("fcfs.txt", "fue:");
  result2 = Search_in_File("sjf.txt", "fue:");
  result3 = Search_in_File("rr.txt", "fue:");
  result4 = Search_in_File("mlfqs.txt", "fue:");

  printf("El tiempo de fcfs es: %d\n  El tiempo de sjc es: %d\n El tiempo de rr es: %d\n El tiempo de mlfqs es: %d\n",result1, result2, result3, result4 );

  return(0);
}