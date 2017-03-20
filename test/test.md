# Test Program
This test program designed for testing opcode operations.
This program use 12 opcode except 'ge' and 'move' command.

* Input : any ascii character.
* Output : Print out length of program.
	* In case of length is bigger than 9. It prints out 'over 10'
	* Otherwise it prints out 0~9.

### PseudoCode
```C
#include <stdio.h>
#include <strlen.h>

int main(void){
	int length=0;
	char input[40];
	gets(input);

	length=strlen(input);

	if(length>9)
		printf("over 10\n");
	else
		printf("%d\n",length);

	return 0;
}
```