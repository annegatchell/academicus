#include <string.h>
#include <stdio.h>


int main()
{

	char* charstar= "orange";
	char chararray[] = "apple";

	printf("Charstar array style %c\n", charstar[0]);
	printf("Charstar dereference %c\n", *charstar);
	//printf("Charstar address %d\n", &charstar);
	printf("Chararray array style %c\n", chararray[0]);
	printf("Chararray dereference %d\n\n\n", *chararray);
	//printf("Chararray address %d\n", &chararray);

	charstar = chararray;

	printf("Charstar array style %c\n", charstar[0]);
	printf("Charstar array style %c\n", charstar[1]);
	printf("Charstar dereference %c\n", *charstar);
	charstar++;
	printf("Charstar dereference %c\n", *charstar);
	//printf("Charstar address %d\n", &charstar);
	printf("Chararray array style %c\n", chararray[0]);
	printf("Chararray dereference %d\n", *chararray);
	//printf("Chararray %d\n", chararray);
	//printf("Chararray address %d\n", &chararray);

	char a[10];
	char b[] = "bc";
	char c[5];
	char d[] = "hhh";

	//strcat(c,d);
	strcat(a,c);
	//strcat(a,b);

	printf("b %s\n", b);
	printf("d %s\n", d);
	int i;
	for (i = 0;i<sizeof(a); i++)
	{
		printf("%c\n", a[i]);
	}
	printf("size of a %ld\n", sizeof(a));
	printf("%s\n", b);



	return 0;
}