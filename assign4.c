#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#include<math.h>

//struct for linked list structure
typedef struct linked_list {
	double num;
	struct linked_list *next;
} data;

//struct for radix pointer structure
typedef struct radix_str {
	struct linked_list *first, *last;
} radix;

int separateString(char cmd[], char sprCmd[][100]);
int syntaxCheck(char sprCmd[][100]);
int parameterCheckAndSeparate(int cmdCount, char sprCmd[][100], int syntaxType, double numberList[]);
int parseNumber(int cmdCount, char sprCmd[][100], double numberList[]);
void displayLinkedlist(data *first);
void addNode(data **first, data **last, double num);
void pushNode(data **first, data **last, double num);
int insertNode(data **first, data **last, double num);
void popNode(data **first, data **last);
void removeNode(data **first, data **last, data **prev, data **cur);
int searchAndDel(data **first, data **last, double num);
int arithmeticSingleOp(data **first, data **last, int opcode, double *answer);
int arithmeticMultiOp(data **first, data **last, int opcode, double *answer);
double peek(data **first, data **last, int idx);
void radix_sort(data **first, data **last);
void divideList(data *first, radix radixTable[], int digit);
void mergeList(data **curFirst, data **curLast, radix radixTable[]);
int confirmation(double num);
void displayHelp();

int main()
{
	char cmd[100000]={}, sprCmd[1000][100]={};
	int i,cmdCount,syntaxType,ret,listCount=0;
	double numberList[1000]={}, answer;
	data *first=NULL, *last=NULL;

	do {
		displayLinkedlist(first);				//display current linked list

		printf("command> ");
		gets(cmd);								//get input command

		for(i=0;i<strlen(cmd);i++)				//for all characters
			cmd[i]=tolower(cmd[i]);				//convert all to lowercase
		cmdCount=separateString(cmd,sprCmd);	//split string with spaces
		syntaxType=syntaxCheck(sprCmd);			//check type of syntax

		if(syntaxType==-1)						//invalid syntax
			printf("answer> Invalid syntax.\n");
		else if(parameterCheckAndSeparate(cmdCount,sprCmd,syntaxType,numberList)==-1)	//separate parameters then try to parse to number
			printf("answer> Invalid parameter.\n");			//some elements failed to parse
		else									//valid syntax and parameters
		{
			//for all commands that requires at least 1 data [pop/sort/sqrt/rec/neg/delete/peek]
			if(syntaxType==2||syntaxType==3||syntaxType==4||syntaxType==5||syntaxType==6||syntaxType==12||syntaxType==15)
			{
				if(listCount==0)			//no data
					printf("answer> No data.\n");
				else if(syntaxType==2)		//pop
				{
					popNode(&first,&last);			//pop the first data
					//removeNode(&first,&last,&first,&first);		//pop method without additional function
					listCount--;					//1 data removed		
				}
				else if(syntaxType==3)		//sort
				{
					radix_sort(&first,&last);		//radix sort
				}
				else if(syntaxType==4)		//sqrt
				{
					if(arithmeticSingleOp(&first,&last,1,&answer)==-1)		//single op mode 1 = sqrt
						printf("answer> Cannot perform this operation: negative sqrt.\n");	//error when the first number is negative
					else
						printf("answer> %g\n",answer);		//show answer
				}
				else if(syntaxType==5)		//rec
				{
					if(arithmeticSingleOp(&first,&last,2,&answer)==-1)		//single op mode 2 = rec
						printf("answer> Cannot perform this operation: divide by zero.\n");	//error when divided by 0	
					else
						printf("answer> %g\n",answer);		//show answer
				}
				else if(syntaxType==6)		//neg
				{
					arithmeticSingleOp(&first,&last,3,&answer);	//single op mode 3 = neg	
					printf("answer> %g\n",answer);			//show answer
				}

				else if(syntaxType==12)		//delete
				{
					ret=searchAndDel(&first,&last,numberList[1]);	//search for number and delete
					if(ret==-1)				//the number doesn't exist
						printf("answer> %g not found.\n",numberList[1]);	//show not found
					else
						listCount-=ret;		//decrease count of those removed
				}

				else if(syntaxType==15)		//peek
				{
					if(listCount<=(int)numberList[1])	//peek exceed data
						printf("answer> maximum index available is %d\n",listCount-1);	//show error
					else
					{
						answer=peek(&first,&last,(int)numberList[1]);	//peek data at that index
						printf("answer> %g\n",answer);	//show answer
					}
				}
			}
			//for all commands that requires at least 2 data [+/-/*/(/)/pow]
			else if(syntaxType>=7&&syntaxType<=11)
			{
				if(listCount==0)			//no data
					printf("answer> No data.\n");
				else if(listCount==1)		//less than 2 data
                    printf("answer> cannot perform this operation: needs 2 data.\n");
				else if(syntaxType==7)		//+
				{
					arithmeticMultiOp(&first,&last,1,&answer);	//multi op mode 1 = +
					printf("answer> %g\n",answer);	//show answer	
					listCount--;			//remove 2 add 1
				}
				else if(syntaxType==8)		//-
				{
					arithmeticMultiOp(&first,&last,2,&answer);	//multi op mode 2 = -
					printf("answer> %g\n",answer);	//show answer
					listCount--;			//remove 2 add 1
				}
				else if(syntaxType==9)		//*
				{
					arithmeticMultiOp(&first,&last,3,&answer);	//multi op mode 3 = *
					printf("answer> %g\n",answer);	//show answer
					listCount--;			//remove 2 add 1
				}
				else if(syntaxType==10)		// /
				{
					if(arithmeticMultiOp(&first,&last,4,&answer)==-1)	//multi op mode 4 = /
						printf("answer> Cannot perform this operation: divide by zero.\n");	// error for divided by 0
					else
					{
						printf("answer> %g\n",answer);	//show answer
						listCount--;		//remove 2 add 1
					}
				}
				else if(syntaxType==11)		//pow
				{
					arithmeticMultiOp(&first,&last,5,&answer);	//multi op mode 4 = pow
					printf("answer> %g\n",answer);	//show answer	
					listCount--;			//remove 2 add 1
				}
			}
			//for all commands that doesn't require any data [end/help/push/insert/add]
			else
			{
				if(syntaxType==0)			//end
					printf("answer> End program.\n");	//show end message
				else if(syntaxType==1)		//help
					displayHelp();						//show help
				else if(syntaxType==13)		//push
				{
					pushNode(&first,&last,numberList[1]);	//push a number to the front of linked list
					listCount++;			//add 1 data
				}
				else if(syntaxType==14)		//insert
				{
					if(insertNode(&first,&last,numberList[1])==-1) //insert a number to sorted linked list
						printf("answer> Please sort the data first.\n");	//error if the linked list in not sorted	
					else
						listCount++;		//add 1 data
				}
				else if(syntaxType==16)		//add
				{
					for(i=1;i<=numberList[0];i++)	//for each data input...
						addNode(&first,&last,numberList[i]);	//add to the end of linked list
					listCount+=numberList[0];		//add n data
				}
			}
		}
	} while(syntaxType!=0);		//if the command is not 'end'

	printf("Program written by 60070501064 Sirawit Lappisatepun\n");
	return 0;
}

//function for separating space-delimited strings
int separateString(char cmd[], char sprCmd[][100])
{
	int count=0;
	char* ptr;

	ptr=strtok(cmd," ");				//separate with space
	while(ptr!=NULL)					//continue until there's nothing left
	{
		strcpy(sprCmd[count],ptr);		//copy substring
		count++;						//count substring
		ptr=strtok(NULL," ");			//continue
	}
	return count;						//return substring count
}

//function for checking valid syntax type
int syntaxCheck(char sprCmd[][100])
{
	int i, ret=-1;	//default to invalid syntax
	char validCmd[17][10]={"end","help","pop","sort","sqrt","rec","neg","+","-","*","/","pow",
							"delete","push","insert","peek","add"};

	for(i=0;i<17;i++)	//for each command available...
	{
		if(strcmp(sprCmd[0],validCmd[i])==0)	//if it matches current command...
			ret=i;		//set return to be the ID of this command
	}

	return ret;		//return syntax type
}

//function for verifying commands
int parameterCheckAndSeparate(int cmdCount, char sprCmd[][100], int syntaxType, double numberList[])
{
	int type;

	//if the command is end/help/pop/sort/sqrt/rec/neg/+/-/*/(/)/pow and doesn't have any parameters
	if( (syntaxType>=0&&syntaxType<=11) && cmdCount==1)
		type=0;		//parameter check ok
	//if the command is delete/push/insert/peek and have 1 parameter
	else if( (syntaxType>=12&&syntaxType<=15) && (parseNumber(cmdCount,sprCmd,numberList)==0&&cmdCount==2) )	//try to parse to double
		type=0;		//parameter check ok
	//if the command is add and have at least 1 parameter
	else if( syntaxType==16 && (parseNumber(cmdCount,sprCmd,numberList)==0&&cmdCount>1) )	//try to parse to double
		type=0;		//parameter check ok
	else
		type=-1;	//parameter check error	

	return type;	//return check	
}

//function for parsing string into number
int parseNumber(int cmdCount, char sprCmd[][100], double numberList[])
{
	int i,j=1,result=0;
	char *endptr;

	for(i=1;i<cmdCount;i++)		//for all substrings
	{
		numberList[j] = strtod(sprCmd[i],&endptr);	//try to parse to double
		if((*endptr)!=0)							//if there's something left after parsing
			result=-1;								//mark as failed
		j++;
	}

	numberList[0]=j-1;	//set numberList[0] to be the amount of number

	return result;	//return parsing result
}

//function for displaying the linked list
void displayLinkedlist(data *first)
{
	data *cur;

	if(first==NULL)		//nothing in the linked list
		printf("\nlist> NULL\n");
	else
	{
		cur=first;		//set current position to the first idx

		printf("\nlist> ");
		do{
			printf("%g ",cur->num);		//display number at the current idx
			cur=cur -> next;			//move to next position
		} while(cur != NULL);			//repeat until it reaches NULL at the end
		printf("\n");
	}
}

//function for adding number to the last idx
void addNode(data **first, data **last, double num)
{
	data *newptr;

	newptr=(data *) malloc(sizeof(data));	//allocates memory for new block
	newptr -> num = num;					//set data
	newptr -> next = NULL;					//set end pointer to NULL 

	if((*first)==NULL)						//if nothing is in the linked list
		(*first) = (*last) = newptr;		//first and last points to this block
	else
	{
		(*last) -> next = newptr;			//previous last points to this block
		(*last) = newptr;					//last points to this block
	}
}

//function for adding number to the first idx
void pushNode(data **first, data **last, double num)
{
	data *newdata;

	newdata = (data *)malloc(sizeof(data));	//allocates memory for new block
	newdata -> num = num;					//set data

	if((*first)==NULL)						//if nothing is in the linked list
	{
		newdata -> next = NULL;				//set next pointer to NULL
		(*first) = (*last) = newdata;		//first and last points to this block
	}
	else
	{
		newdata -> next = (*first);			//this block points to previous first		
		(*first) = newdata;					//first points to this block
	}
}

//function for inserting number in sorted position
int insertNode(data **first, data **last, double num)
{
	data *cur = (*first), *newptr;	//set current position to first

	//check if the linked list is sorted
	while(cur!=NULL && cur -> next !=NULL)		//loop until NULL is found
	{
		if(cur -> num > (cur -> next) -> num)	//a number is in wrong position
			return -1;							//return error
		cur = cur -> next;						//moves to next block
	}

	//if check passed
	newptr = (data *) malloc(sizeof(data));		//allocates memory for new block
	newptr -> num = num;						//set data

    if((*first)==NULL)							//if nothing is in the linked list
    {
        newptr -> next = NULL;					//set next pointer to NULL
        (*first) = (*last) = newptr;			//first and last points to this block
    }
    else if((*first) -> num > num)				//insert to first position
    {
        newptr -> next = (*first);				//this block points to previous first
        (*first) = newptr;						//first points to this block
    }
    else
    {
        cur = (*first);							//set current position to first
        while(cur!=NULL && cur -> next !=NULL && (cur -> next) -> num < num)	//traversal to find correct position
            cur = cur -> next;					//moves to next block

        if(cur==(*last))						//insert to last position
        {
            newptr -> next = NULL;				//set next pointer to NULL
            (*last) -> next = newptr;			//previous last points to this block	
            (*last) = newptr;					//set last pointer to this block
        }
        else									//insert anywhere else	
        {
            newptr -> next = cur -> next;		//set next pointer to current's next pointer
            cur -> next = newptr;				//set current's next to this block
        }
    }

	return 0;	//return ok	
}

//function for removing first data
void popNode(data **first, data **last)
{
    data *cur=(*first);		//set current position to first

    if((*first)==(*last))	//if only 1 data is in the linked list
        (*first) = (*last) = NULL;	//set first and last to NULL
    else
        (*first) = (*first) -> next;	//set first pointer to next block

    free(cur);				//free current block
}

//function for removing a data
void removeNode(data **first, data **last, data **prev, data **cur)
{
    data *rem=(*cur);		//set remove position to current

	if((*cur)==(*first)&&(*first)==(*last))	//if only 1 data is in the linked list
		(*first) = (*last) = NULL;	//set first and last to NULL
	else if((*cur)==(*first))		//remove first data
    {
        (*first) = (*cur) -> next;	//set first pointer to next block
    }
	else if((*cur)==(*last))		//remove last data 
	{
		(*last) = (*prev);			//set last data to the one before current last
		(*prev) -> next = NULL;		//set next pointer of previous to NULL
	}
	else							//remove anywhere else
		(*prev) -> next = (*cur) -> next;	//set next pointer of previous to next of current

	free(rem);		//free current block		
}

//function for searching a data to remove
int searchAndDel(data **first, data **last, double num)
{
	int remCount=0, found=0;
	data *cur, *prev, *temp;

	cur=(*first);		//set current and previous to first
	prev=(*first);

	while(cur!=NULL)	//search until NULL is reached	
	{
		if(cur->num == num)		//if current data matches user input
		{
			found++;	//increment found amount	
			if(confirmation(num)==1)	//if user confirmed deletion
			{
				remCount++;		//increment delete amount
				temp = cur -> next;		//set temp pointer to next position of current
				removeNode(first,last,&prev,&cur);	//remove current block
				cur = temp;		//set current to temp
			}
			else		//user didn't confirmed
            {
                prev = cur;			//previous is the current position
                cur = cur -> next;	//set current to next position
            }
		}
		else			//not match
        {
            prev = cur;			//previous is the current position
            cur = cur -> next;	//set current to next position
        }
	}

	if(found==0)			//not found
		return -1;			//return not found
	else					//found
		return remCount;	//return amount of deletion
}

//function for arithmetic with one data
int arithmeticSingleOp(data **first, data **last, int opcode, double *answer)
{
	double num,ans;
	int result=0;

	num=(*first) -> num;	//calculate with the first data

	if(opcode==1)			//sqrt
	{
		if(num<0)			//error: sqrt < 0
			result=-1;
		else
			ans=sqrt(num);	//calculate sqrt(x)
	}
	else if(opcode==2)		//rec
	{
		if(num==0)			// error: 1/0
			result=-1;
		else
			ans=1/num;		//calculate 1/x
	}
	else if(opcode==3)		//neg
	{
		ans=num*-1;			//calculate negation
	}

	if(result==0)			//if no error
	{
		popNode(first,last);			//remove first block
		pushNode(first,last,ans);		//add result as the first block	
		(*answer)=ans;					//return answer
	}

	return result;		//return error/ok
}

//function for arithmetic with two data
int arithmeticMultiOp(data **first, data **last, int opcode, double *answer)
{
	double numA,numB,ans;
	int result=0;

	numA=((*first) -> next) -> num;		//calculate with the second data...
	numB=(*first) -> num;				//...and the first data

	if(opcode==1)			//+
	{
		ans=numA+numB;		//calculate A+B
	}
	else if(opcode==2)		//-
	{
		ans=numA-numB;		//calculate A-B
	}
	else if(opcode==3)		//*
	{
		ans=numA*numB;		//calculate A*B
	}
	else if(opcode==4)		// /
	{
		if(numB==0)			//error A/0
			result=-1;		//return error
		else
			ans=numA/numB;	//calculate A/B
	}
	else if(opcode==5)		//pow
	{
		ans=pow(numA,numB);	//calculate A^B
	}

	if(result==0)			//if no error
	{
		popNode(first,last);		//remove first block
		popNode(first,last);		//remove second block	
		pushNode(first,last,ans);	//add result as the first block	
		(*answer)=ans;				//return answer	
	}

	return result;		//return error/ok
}

//function for displaying nth data/last data
double peek(data **first, data **last, int idx)
{
	int i=0;
	data *cur=(*first);		//set current position to first

	if(idx==-1)				//ask for last idx
		return (*last) -> num;	//return data of last block
	else
	{
		while(i!=idx)		//loop until it reaches nth block
		{
			cur = cur -> next;	//moves to next position
			i++;				//increase count
		}

		return cur -> num;	//return data of nth block
	}
}

//function for radix sort
void radix_sort(data **first, data **last)
{
	data *curFirst=NULL, *curLast=NULL;
	radix radixTable[10];
	int digit;

	for(digit=0;digit<8;digit++)					//for each digit from LSD to MSD, limit to 8 digits
	{
		divideList(*first,radixTable,digit);		//divide into radix table
		mergeList(&curFirst,&curLast,radixTable);	//merge back into single linked list
		(*first)=curFirst;							//set new first address
		(*last)=curLast;							//set new last address	
	}
}

//function for dividing linked list into table
void divideList(data *first, radix radixTable[], int digit)
{
	data *cur = first;
	int i,num;

	for(i=0;i<10;i++)		//for each digit's table...
		radixTable[i].first = radixTable[i].last = NULL;	//set all pointers to NULL

	while(cur!=NULL)		//loop until NULL is reached
	{
		num=(int)((cur->num)/pow(10,digit))%10;		//get the number in the current digit
		if(radixTable[num].first==NULL)				//if nothing is in this table's idx yet
		{
			radixTable[num].first=cur;				//set first to current
			radixTable[num].last=cur;				//set last to current
		}
		else
		{
			radixTable[num].last -> next = cur;		//set previous last next pointer to current
			radixTable[num].last=cur;				//set last to current	
		}

        cur=cur -> next;							//moves to next position
        radixTable[num].last -> next = NULL;		//set last next pointer to NULL
	}
}

//function for merging radix table back into one linked list
void mergeList(data **curFirst, data **curLast, radix radixTable[])
{
	int i;

	(*curFirst)=(*curLast)=NULL;	//set new first and last pointer to NULL

	for(i=0;i<10;i++)				//for each radix table's idx
	{
		if(radixTable[i].first!=NULL)	//if this idx has data
        {
            if((*curFirst)==NULL)		//if new linked list is blank
            {
                (*curFirst)=radixTable[i].first;	//set first to address of first of this digit	
                (*curLast)=radixTable[i].last;		//set last to address of last of this digit
            }
            else
            {
                (*curLast) -> next = radixTable[i].first;	//set next pointer to first address of this digit
                (*curLast) = radixTable[i].last;			//set last to address of last of this digit
            }
        }
	}
}

//function to confirm data deletion with user
int confirmation(double num)
{
	char inp, ret;

	printf("answer> %g found, press y to delete: ",num);	//display current data

	while(scanf("%c%c",&inp,&ret)!=2||ret!='\n')			//input error
		printf("answer> Invalid input, try again: ");		//show error message

	if(inp=='Y'||inp=='y')		//confirm deletion
		return 1;				//return confirm
	else						//don't delete
		return 0;				//return no
}

//function for displaying help message
void displayHelp()
{
	printf("Available commands:\n\n");
	printf("add <num1> <num2> ... \t add number(s) to linked list.\n");
	printf("peek <n> \t\t display linked list index <n>, enter -1 for last index.\n");
	printf("delete <num> \t\t search and delete all instances of <num> in linked list.\n");
	printf("push <num> \t\t insert <num> to the front of linked list.\n");
	printf("pop \t\t\t remove first data of linked list.\n");
	printf("sqrt , rec , neg \t perform sqrt(x), 1/x, and -x to the first data. Result will be inserted to the front.\n");
	printf("+ , - , * , / , pow \t perform +, -, *, /, pow to the first two data. Result will be inserted to the front.\n");
	printf("insert <num> \t\t insert <num> to the sorted linked list.\n");
	printf("sort \t\t\t sort data in linked list.\n");
	printf("help \t\t\t display help message.\n");
	printf("end \t\t\t exit program.\n\n");
}
