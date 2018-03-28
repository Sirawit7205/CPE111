#include<stdio.h>

int getInt(int min, int max);
long long int factRecur(int n);

int main()
{
	int n;
	long long int ans;
	char end;
	
	printf("My Recursion Programs.\n");
	printf("Program calculate n! by recursion (n<=15)\n");
	do {
		printf("Enter n = ");
		n=getInt(0,15);			//get input n
		ans=factRecur(n);		//call recursive	
		printf("Complete calculation of %d!, answer = %lld\n",n,ans);
		
		printf("press [y] to continue, others to exit. : ");
		rewind(stdin);			//rewind input
		while(scanf("%c",&end)!=1)		//check if the input is receive correctly
		{
			rewind(stdin);		//rewind input
			printf("Input error, please enter again =  ");
		}
	} while(end=='y'||end=='Y');	//if Y or y is input
	printf("Program written by 60070501064 Sirawit Lappisatepun.\n");
	return 0;
}

//function for getting an integer
int getInt(int min, int max)
{
	char end;
	int ret;
	
	//check if data is correctly input, value between min and max, and ends with enter
	while(scanf("%d%c",&ret,&end)!=2||end!='\n'||ret<min||ret>max)	
	{
		rewind(stdin);	//rewind input
		printf("Input error, please enter number between 0-15 = ");
	}
	return ret;		//return int
}

//factorial recursive
long long int factRecur(int n)
{
	long long int current;	//a variable for storing factorial from 0! to n-1!
	
	//base case
	if(n==0)
	{
		printf("0! is base case. return answer of 0! = 1\n");
		printf("Calculate 0! complete.\n");
		return 1;	//0! = 1
	}
	else
	{
		printf("%d! is recursive case. Answer = %d * recursion of %d!\n",n,n,n-1);
		printf("\tRecursion to calculate %d!\n",n-1);
		current = factRecur(n-1);	//recursive n-1
		if(n!=1) printf("Calculate %d! complete.\n",n-1);
		printf("\tReturn answer from %d! = %lld to calculate %d! = [%d * %d!] = %d * %lld = %lld\n",n-1,current,n,n,n-1,n,current,n*current);
		return n * current;		//return n * n-1
	}
}