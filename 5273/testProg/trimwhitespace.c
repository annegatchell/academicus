#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


int trimwhitespace(char *str, char *trimmed, size_t trimmed_size)
{
  
  
  char* temp;
  temp = str;
  char* end = temp + trimmed_size - 1;

  printf("temp %slll\n", temp);
  // Get rid of leading whitespace
  // Trim leading space
  while(isspace(*temp)) 
  	{temp++;}

  printf("temp %slll\n", temp);

  if(*temp == 0)  // All spaces?
    return 0; //Returns 0 is empty string


  // Trim trailing space
  //end = temp + strlen(temp) - 1;
  while(end > temp && isspace(*end)) 
  { end--;}
  printf("temp %slll\n", temp);

  // Write new null terminator
  size_t stringSize = (end - temp) < trimmed_size-1 ? (end - str) : (trimmed_size - 1);
  trimmed[stringSize] = 0;
  printf("temp %slll\n", temp);

  memcpy(trimmed, temp, trimmed_size);


  return 1;
}

int main()
{
  char string[50];
  bzero(&string,sizeof(string));
  char * apple = "     longlonglonglonglong    ";
  memcpy(string, apple, sizeof(string));

  printf("apple, size: %s, %ld \n", apple, sizeof(apple));
  printf("string, size: %s, %ld \n", string, sizeof(string));

  char trim[50];
  int err = trimwhitespace(string, trim,sizeof(trim));

  printf("err %d\n", err);
  printf("string, size: %s, %ld \n", string, sizeof(string));
  printf("trim, size: %s, %ld \n", trim, sizeof(trim));

  /*FILE *fp;
  char file_buffer[512];
  int num_bytes_returned;
  char filename[50];
  char *foo = "foo1";
  memcpy(filename,foo,sizeof(foo));
  
  printf("filename %s\n",filename);
  fp = fopen(filename,"r");
  num_bytes_returned = fread(file_buffer,sizeof(char),sizeof(file_buffer),fp);
  if(num_bytes_returned==-1)
  {printf("Error Reading File, Try again...\n");
  }
  else if(num_bytes_returned > 0)
  {
    printf("file buffer %s\n", file_buffer);

  }

  fclose(fp);

*/

  return 0;
}