#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


int trimwhitespace(char *str, char *trimmed/*, int trimmed_size*/)
{
  
  char* temp;
  temp = str;
  char* end;

  // Get rid of leading whitespace
  // Trim leading space
  while(isspace(*temp)) 
  	{temp++;}

  printf("temp %s\n", temp);

  if(*temp == 0)  // All spaces?
    return 0; //Returns 0 is empty string


  // Trim trailing space
  end = temp + sizeof(temp) - 1;
  while(end > temp && isspace(*end)) 
  { end--;}
  printf("temp %s\n", temp);

  // Write new null terminator
  *(end+1) = 0;
  printf("temp %s\n", temp);
  memcpy(trimmed, temp, sizeof(temp));

  return 1;
}

int main()
{
  char string[50];
  bzero(&string,sizeof(string));
  char * apple = "   apple   ";
  memcpy(string, apple, sizeof(apple));

  printf("string, size: %s, %ld \n", string, sizeof(string));

  char trim[50];
  int err = trimwhitespace(string, trim);

  printf("err %d\n", err);
  printf("string, size: %s, %ld \n", string, sizeof(string));
  printf("trim, size: %s, %ld \n", trim, sizeof(string));

  FILE *fp;
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



  return 0;
}