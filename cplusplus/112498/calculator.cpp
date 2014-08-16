#include <stdio.h>

int main()

{

	int a,c;
	char b;

	printf("Input first integer: ");
	scanf("%i", &a);
	getchar();

	printf("What's the operation to be performed: ");
	scanf("%c", &b);
	getchar();

	printf("Input second integer: ");
	scanf("%i", &c);
	getchar();

	if (c == 0 && b == '/')

	{
		printf("The solution to the information given is : %i / %i = The denominator is zero and cannot be calculated\n\n", a,c);
	}

	else if (b == '-')

	{
		printf("The solution to the information given is : %i - %i = %i\n\n", a,c, a - c);
	}

	else if (b == '*')

	{
		printf("The solution to the information given is : %i * %i = %i\n\n", a,c, a * c);
	}

	else if (b == '/')

	{
		printf("The solution to the information given is : %i / %i = %.2f\n\n", a,c, (double)a / c);
	}

	else

	{
		printf("The solution to the information given is : %i + %i = %i\n\n", a,c, a + c);
	}

	getchar();
	return 0;

}
