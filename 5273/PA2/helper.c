#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <semaphore.h>
#include <time.h>
#include <string.h>

#include "helper.h"


// void intArrayToString(int array[], char *charArray){
//   int i;
//   charArray = (char*) calloc(sizeof(array)/sizeof(array[0]), sizeof(char));
//   printf("%ld\n", sizeof(array)/sizeof(array[0]));
//   printf("%ld\n", sizeof(charArray));
//   for(i=0; i < sizeof(charArray);i++){
//     charArray[i] = (char)(((int)'0')+array[i]);
//     printf("%s\n", charArray[i]);
//   }
//   return;
// }

void printIntArray(FILE *f, int array[], int size){
  int i;
  //printf("%ld\n", sizeof(array)/sizeof(array[0]));
  fprintf(f,"<");
  for(i=0; i < size-1;i++){
    fprintf(f, "%d,", array[i]);
  }
  fprintf(f,"%d>", array[size-1]);
}

/*Function to print to the log file of the client or server. 
c_or_s is !=0 for client, 0 for server*/
int toLog(int c_or_s, char* filename, char * action, SwpSeqno seq_num, int acked[], int size_of_acked, SwpState *state){
  /*CLIENT: <Send | Resend | Receive> <Seq #> <LAR> <LFS> <Seq #'s of ACKed packets> <Time> */
  /*SERVER: <Send | Resend | Receive> <Seq #> <LFR> <LAF> <Sequence #'s of Received Data Packets> <Time> */
  FILE *flog;
  flog = fopen(filename, "a");
  struct timeval time;
  char* list;
  //intArrayToString(acked, list);
  //printf("%s\n", list);
  gettimeofday(&time, NULL);
  if(c_or_s){
    fprintf(flog, "<%s> <%d> <%d> <%d>", 
    action, seq_num, state->LAR, state->LFS);
  }
  else{
     fprintf(flog, "<%s> <%d> <%d> <%d>", 
    action, seq_num, state->LAR, state->LFS);
  }
  printIntArray(flog, acked, size_of_acked);
  fprintf(flog, "<%ld>\n", (time.tv_sec * 1000000 + time.tv_usec));

  fclose(flog);
  return 1;
}



int trimwhitespace(char *str, char *trimmed, size_t str_size)
{
  char* temp;
  temp = str;
  char* end = temp + str_size - 1;

  while(isspace(*temp)) 
  	{temp++;}

  if(*temp == 0)  // All spaces?
    return 0; //Returns 0 is empty string

  // Trim trailing space
  while(end > temp && isspace(*end)) 
  { end--;}

  // Write new null terminator
  size_t stringSize = (end - temp) < str_size-1 ? (end - str) : (str_size - 1);
  trimmed[stringSize] = 0;
  
  memcpy(trimmed, temp, str_size);
  return 1;
}

int getSizeOfBuffer(char* str){
	int counter = 0;
	do{
		if(str[counter]!= '\0'){
			counter++;
		}
	}while(str[counter] != '\0');

	return counter;
}