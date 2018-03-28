#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<math.h>

typedef struct linkedlist {
	char command[50];
	struct linkedlist *next;
} lnkdlist;

int separateString(char cmd[], char sprCmd[][50]);
void addStringDelim(char cmd[]);
void convertSignOperator(char sprCmd[][50], int cmdCount);
int syntaxCheck(char sprCmd[][50], int cmdCount);
int calculate(char sprCmd[][50], int cmdCount, double *lastAns, double var[]);
void convertPostfix(char cmd[][50], int cmdCount, double lastAns, lnkdlist **qFirst, lnkdlist **qLast);
int calculatePostfix(double *lastAns, double var[], lnkdlist **qFirst, lnkdlist **qLast);
void insertConstant(lnkdlist **ptr, double lastAns);
int priorityCheck(char cmd[]);
int getSyntaxType(char cmd[]);
int isFunction(char cmd[]);
int isVariable(char cmd[]);
int isConstant(char cmd[]);
lnkdlist *createNode(char cmd[]);
void insertQueue(lnkdlist **first, lnkdlist **last, lnkdlist *ptr);
void insertStack(lnkdlist **first, lnkdlist **last, lnkdlist *ptr);
lnkdlist *popData(lnkdlist **first, lnkdlist **last);
void displayHelp();

int main()
{
	int i,cmdCount,res;
	char cmd[5000]={}, sprCmd[100][50]={};
	double lastAns=0, var[11]={};

	do {
		printf("\nExpression> ");
		gets(cmd);

		for(i=0;i<strlen(cmd);i++)				//for all characters
			cmd[i]=tolower(cmd[i]);				//convert all to lowercase
		addStringDelim(cmd);					//add separation spaces
		cmdCount=separateString(cmd,sprCmd);	//split string with spaces
		convertSignOperator(sprCmd,cmdCount);	//convert sign - to !
		res=syntaxCheck(sprCmd,cmdCount);		//check if command syntax is valid

		if(res==-1)								//syntax error
			printf("Syntax check> ERROR\n");
		else									//syntax OK
        {
            printf("Syntax check> OK\n");

            if(strcmp(sprCmd[1],"help")==0)			//HELP command
                displayHelp();						//display help
            else if(strcmp(sprCmd[1],"end")==0)		//END command
                printf("End program.\n");
            else									//other commands
            {
                res=calculate(sprCmd,cmdCount,&lastAns,var);	//calculate postfix
                if(res==-1)										//math error
                    printf("Answer> ERROR\n");
                else											//math OK
                    printf("Answer> %g\n",lastAns);				//display answer
            }
        }
	} while(strcmp(sprCmd[1],"end")!=0);			//loop until END is entered	

	printf("Program written by 60070501064 Sirawit Lappisatepun\n");
	return 0;
}

//function to add space delimiters to the string
void addStringDelim(char cmd[])
{
	int i;
	char buff[5000]={},last[5000]={};

	for(i=0;i<strlen(cmd);i++)						//check each character...
	{
		if(strchr("+-*/^()=",cmd[i])!=NULL)			//if current char is an operator...
			sprintf(buff,"%s %c ",last,cmd[i]);		//add spaces
		else										//else...
			sprintf(buff,"%s%c",last,cmd[i]);		//do nothing
		strcpy(last,buff);
	}
	strcpy(cmd,buff);								//copy to result string
}

//function to separate string with spaces
int separateString(char cmd[], char sprCmd[][50])
{
	int count=0;
	char *ptr;

	ptr = strtok(cmd," ");				//get token
	while(ptr!=NULL)					//if the token is not NULL
	{
		strcpy(sprCmd[count+1],ptr);	//copy to result array	
		ptr = strtok(NULL," ");			//get new token
		count++;						//count new data		
	}

	return count;						//return amount of data
}

//function to convert sign operator - to !
void convertSignOperator(char sprCmd[][50], int cmdCount)
{
	int i;

	if(strcmp(sprCmd[1],"-")==0)		//if the first char is - ...
		strcpy(sprCmd[1],"!");			//convert to !

	for(i=2;i<=cmdCount;i++)			//for all other characters...
	{
		if(strcmp(sprCmd[i+1],"-")==0 && strstr("+-*/^(",sprCmd[i]))	//if the sign operator follows an operator...
			strcpy(sprCmd[i+1],"!");	//convert to !
	}
}

//function for checking command syntax
int syntaxCheck(char sprCmd[][50], int cmdCount)
{
	int res=0,i,parenCount=0,curType=0,nextType=0;

	for(i=1;i<=cmdCount&&res==0;i++)		//for each command or until an error is found
	{
		//0=start 1=num/const 2=op 3=sign 4=fn 5=( 6=) 7=var 8== 9=help 10=end
		curType=nextType;					//save current command syntax type
		nextType=getSyntaxType(sprCmd[i]);	//get next command syntax type

		if(nextType==-1)					//if type is error (-1)
			res=-1;							//set result to error (-1)

		if(nextType==5)						//if type is ( ...
			parenCount++;					//increase parenthesis count
		if(nextType==6&&parenCount==0)		//if type is ) but no leading ( ...
			res=-1;							//set result to error (-1)
		else if(nextType==6)				//if type is )
            parenCount--;					//decrease parenthesis count	
		
		//check command orders based on state diagram, if state is invalid, set result to error (-1)
		if((curType==0)&&!(nextType==1||nextType==3||nextType==4||nextType==5||nextType==7||nextType==9||nextType==10))
			res=-1;
		else if((curType==1)&&!(nextType==2||nextType==6||nextType==10))
			res=-1;
		else if((curType==2)&&!(nextType==1||nextType==3||nextType==4||nextType==5||nextType==7))
			res=-1;
		else if((curType==3)&&!(nextType==1||nextType==4||nextType==5||nextType==7))
			res=-1;
		else if((curType==4)&&!(nextType==5))
			res=-1;
		else if((curType==5)&&!(nextType==1||nextType==3||nextType==4||nextType==5||nextType==7))
			res=-1;
		else if((curType==6)&&!(nextType==2||nextType==6||nextType==10))
			res=-1;
		else if((curType==7)&&!(nextType==2||nextType==3||nextType==6||nextType==8||nextType==10))
			res=-1;
		else if((curType==8)&&!(nextType==1||nextType==3||nextType==4||nextType==5||nextType==7))
			res=-1;
	}

	if(!(nextType==1||nextType==6||nextType==7||nextType==9||nextType==10))		//if the final state is wrong...
		res=-1;				//set result to error (-1)
	if(parenCount!=0)		//if there's unclosed parenthesis...
		res=-1;				//set result to error (-1)

	return res;
}

//function to control postfix calculation
int calculate(char cmd[][50], int cmdCount, double *lastAns, double var[])
{
	int res;
	lnkdlist *qFirst=NULL, *qLast=NULL;		//pointers for postfix queue

	convertPostfix(cmd,cmdCount,*lastAns,&qFirst,&qLast);		//convert infix to postfix
	res=calculatePostfix(lastAns,var,&qFirst,&qLast);			//calculate postfix

	//DEBUG, see if there's any leftover data in queue
	/*temp=qFirst;
	while(temp!=NULL)
	{
		printf("%s ",temp->command);
		temp = temp -> next;
	}
	printf("\n");*/

	return res;													//return answer	
}

//function for converting infix to postfix
void convertPostfix(char cmd[][50], int cmdCount, double lastAns, lnkdlist **qFirst, lnkdlist **qLast)
{
	int i,type,chkA,chkB;
	lnkdlist *sFirst=NULL, *sLast=NULL, *ptr, *temp;		//pointers for operator stack

	strcpy(cmd[0],"(");					//add parenthesis to front...
	strcpy(cmd[cmdCount+1],")");		//and to last position of the command
	cmdCount+=2;						//add 2 more commands

	for(i=0;i<cmdCount;i++)								//for each command...
	{
		ptr=createNode(cmd[i]);							//create a new linked list node for the command
		type=getSyntaxType(cmd[i]);						//get syntax type

		if(type==1)										//num/const
		{
			insertConstant(&ptr,lastAns);				//change constant to number (if applicable)
			insertQueue(qFirst,qLast,ptr);				//insert to postfix queue
		}
		else if(type==7)								//variable
			insertQueue(qFirst,qLast,ptr);				//insert to postfix queue
		else if((type>=2&&type<=4)||type==8)			//operators and =
		{
			chkA=priorityCheck(ptr->command);			//check priority of new operator

			do {
				chkB=priorityCheck(sFirst->command);	//check priority of the stack front	
				if(chkB>=chkA)							//if the priority is higher or equal...	
				{
					temp=popData(&sFirst,&sLast);		//pop from stack...
					insertQueue(qFirst,qLast,temp);		//and insert to postfix queue
				}
			} while(chkB>=chkA&&sFirst!=NULL);			//do until the priority is lower

			insertStack(&sFirst,&sLast,ptr);			//insert new operator to stack
		}
		else if(type==5)								// (
			insertStack(&sFirst,&sLast,ptr);			//insert to stack
		else if(type==6)								// )
		{
			while(strcmp(sFirst->command,"(")!=0)		//do until the top of the stack is (
			{
				temp=popData(&sFirst,&sLast);			//pop from stack...
				insertQueue(qFirst,qLast,temp);			//and insert to postfix queue
			}
			temp=popData(&sFirst,&sLast);				//discard (
		}
	}
}

//function for calculating postfix
int calculatePostfix(double *lastAns, double var[], lnkdlist **qFirst, lnkdlist **qLast)
{
	int type, fType, varA, varB;
	double numA, numB, ans;
	char *temp;
	lnkdlist *sFirst=NULL, *sLast=NULL;					//pointers for number stack
	lnkdlist *ptr, *ptrA, *ptrB, *new;

	ptr=popData(qFirst,qLast);							//pop first data from postfix queue
	while(ptr!=NULL)									//do until nothing is left in postfix queue
	{
		type=getSyntaxType(ptr->command);				//get syntax type
		if(type==1||type==7)							//if num/var
			insertStack(&sFirst,&sLast,ptr);			//insert to stack
		else if(type==2||type==8)						//if binary operator/=
		{
			ptrB=popData(&sFirst,&sLast);				//pop 2 first data
			ptrA=popData(&sFirst,&sLast);
			varB=isVariable(ptrB->command);				//check if they are variables
			varA=isVariable(ptrA->command);

			if(varB!=-1) numB=var[varB];				//if A is a variable, insert data from cache
			else numB=strtod(ptrB->command,&temp);		//else, convert string to number
			if(varA!=-1) numA=var[varA];				//if B is a variable, insert data from cache
			else numA=strtod(ptrA->command,&temp);		//else, convert string to number

			if(strcmp(ptr->command,"+")==0)				//+
				ans=numA+numB;
			else if(strcmp(ptr->command,"-")==0)		//-	
				ans=numA-numB;
			else if(strcmp(ptr->command,"*")==0)		//*
				ans=numA*numB;
			else if(strcmp(ptr->command,"/")==0)		// /
			{
				if(numB!=0) ans=numA/numB;
				else return -1;							//error: divide by zero
			}
			else if(strcmp(ptr->command,"^")==0)		//^
				ans=pow(numA,numB);
			else if(strcmp(ptr->command,"=")==0)		//=
				ans=var[varA]=numB;						//also pass the answer to variable cache

			sprintf(temp,"%g",ans);						//convert answer back to string
			new=createNode(temp);						//create new node for answer
			insertStack(&sFirst,&sLast,new);			//insert to stack
			free(ptrB);									//free unused data	
			free(ptrA);
			free(ptr);
		}
		else if(type==3||type==4)							//sign/unary operator (functions)
		{
			ptrA=popData(&sFirst,&sLast);					//pop first data
			varA=isVariable(ptrA->command);					//check if it is a variable

			if(varA!=-1) numA=var[varA];					//if A is a variable, insert data from cache
			else numA=strtod(ptrA->command,&temp);			//else, convert string to number

			fType=isFunction(ptr->command);					//check if it is a function
			if(fType>=0&&fType<=2) numA=(numA*M_PI)/180;	//if sin/cos/tan, convert degree to radian	

			if(strcmp(ptr->command,"!")==0)					//sign operator
				ans=numA*-1;
			else if(fType==0)		//sin			
				ans=sin(numA);
			else if(fType==1)		//cos
				ans=cos(numA);
			else if(fType==2)		//tan
				ans=tan(numA);
			else if(fType==3)		//asin
				ans=asin(numA);
			else if(fType==4)		//acos
				ans=acos(numA);
			else if(fType==5)		//atan
				ans=atan(numA);
			else if(fType==6)		//sqrt
			{
				if(numA>=0) ans=sqrt(numA);		
				else return -1;					//error: negative under sqrt
			}
			else if(fType==7)					//log10
			{
				if(numA>0) ans=log10(numA);
				else return -1;					//error: negative under log10
			}
			else if(fType==8)		//exp
				ans=exp(numA);
			else if(fType==9)		//abs
				ans=abs(numA);

            if(fType>=3&&fType<=5) ans=(ans*180)/M_PI;		//if asin/acos/atan, convert radian to degree
			sprintf(temp,"%g",ans);				//convert answer back to string
			new=createNode(temp);				//create new node for answer
			insertStack(&sFirst,&sLast,new);	//insert to stack	
			free(ptrA);							//free unused data	
			free(ptr);
		}
		ptr = popData(qFirst,qLast);			//pop the top data from postfix queue
	}

	ptr=popData(&sFirst,&sLast);				//pop the answer from stack
	(*lastAns)=strtod(ptr->command,&temp);		//convert answer to number
	return 0;									//return OK
}

//auxiliary function for inserting constants over placeholder
void insertConstant(lnkdlist **ptr, double lastAns)
{
	int type=isConstant((*ptr)->command);					//get constant type
	char temp[50]={};

	if(type==0)
		strcpy((*ptr)->command,"3.14159265358979323846");	//pi
	else if(type==1)
		strcpy((*ptr)->command,"2.71828182845904523536");	//e
	else if(type==2)
		strcpy((*ptr)->command,"299792458");				//c
	else if(type==3)
		strcpy((*ptr)->command,"9.80665");					//g
	else if(type==4)
	{
		sprintf(temp,"%g",lastAns);							//convert last answer to string
		strcpy((*ptr)->command,temp);						//ans
	}
}

//auxiliary function for checking operator priority
int priorityCheck(char cmd[])
{
	if(strcmp(cmd,"=")==0)								//=
		return 1;
	else if(strcmp(cmd,"+")==0||strcmp(cmd,"-")==0)		//+-
		return 2;
	else if(strcmp(cmd,"*")==0||strcmp(cmd,"/")==0)		//*/
		return 3;
	else if(strcmp(cmd,"^")==0)							//^	
		return 4;
	else if(strcmp(cmd,"!")==0)							//!	
		return 5;
	else if(isFunction(cmd)!=-1)						//functions
		return 6;
	else												//others
		return 0;
}

//auxiliary function that returns syntax type of the string
int getSyntaxType(char cmd[])
{
	double temp;
	char *endptr;

	//tries to convert to number
	temp=strtod(cmd,&endptr);

	//0=start 1=num/const 2=op 3=sign 4=fn 5=( 6=) 7=var 8== 9=help 10=end -1=error
	if((*endptr)==0)
		return 1;
	else if(isConstant(cmd)!=-1)
		return 1;
	else if(strchr("+-*/^",cmd[0])!=NULL)
		return 2;
	else if(strcmp(cmd,"!")==0)
		return 3;
	else if(isFunction(cmd)!=-1)
		return 4;
	else if(strcmp(cmd,"(")==0)
		return 5;
	else if(strcmp(cmd,")")==0)
		return 6;
	else if(isVariable(cmd)!=-1)
		return 7;
	else if(strcmp(cmd,"=")==0)
		return 8;
	else if(strcmp(cmd,"help")==0)
		return 9;
	else if(strcmp(cmd,"end")==0)
		return 10;
	else
		return -1;
}

//auxiliary function to check if the string is function or not
int isFunction(char cmd[])
{
	int i,res=-1;		//default to not function (-1)
	char funcName[10][5]={"sin","cos","tan","asin","acos","atan","sqrt","log","exp","abs"};

	for(i=0;i<10;i++)						//for each available function...
		if(strcmp(cmd,funcName[i])==0)		//if the string matches...
			res=i;							//set result to function no.

	return res;								//return result
}

//auxiliary function to check if the string is variable or not
int isVariable(char cmd[])
{
	int i,res=-1;		//default to not variable (-1)
	char varName[10][5]={"x1","x2","x3","x4","x5","x6","x7","x8","x9","x10"};

	for(i=0;i<10;i++)						//for each available variable...
		if(strcmp(cmd,varName[i])==0)		//if the string matches...
			res=i;							//set result to variable no.

	return res;								//return result
}	

//auxiliary function to check if the string is constant or not
int isConstant(char cmd[])
{
	int i,res=-1;		//default to not constant (-1)
	char constName[5][5]={"pi","e","c","g","ans"};

	for(i=0;i<5;i++)						//for each available constant...
	if(strcmp(cmd,constName[i])==0)			//if the string matches...
		res=i;								//set result to constant no.		

	return res;								//return result
}

//function for creating linked list node
lnkdlist *createNode(char cmd[])
{
	lnkdlist *ptr;

	ptr = (lnkdlist *) malloc(sizeof(lnkdlist));		//allocate memory
	strcpy(ptr->command,cmd);							//copy command to linked list 				
	ptr -> next = NULL;									//set next pointer to NULL

	return ptr;											//return new node
}

//function for inserting linked list node to a queue
void insertQueue(lnkdlist **first, lnkdlist **last, lnkdlist *ptr)
{
    ptr -> next = NULL;				//set next pointer to NULL

	if((*first)==NULL)				//nothing in the queue
		(*first)=(*last)=ptr;		//set first and last to current
	else							//something in the queue
	{
		(*last) -> next = ptr;		//last points to current
		(*last) = ptr;				//last is current
	}
}

//function for inserting linked list node to a stack
void insertStack(lnkdlist **first, lnkdlist **last, lnkdlist *ptr)
{
    ptr -> next = NULL;				//set next pointer to NULL

	if((*first)==NULL)				//nothing in the stack
		(*first)=(*last)=ptr;		//set first and last to current
	else							//something in the stack
	{
		ptr -> next = (*first);		//current points to first
		(*first) = ptr;				//first is current
	}
}

//function for removing linked list node from stack/queue
lnkdlist *popData(lnkdlist **first, lnkdlist **last)
{
	lnkdlist *temp;

	if((*first)==NULL)				//nothing in the list	
		return NULL;				//return NULL
    else if((*first)==(*last))		//only one node in the list
    {
        temp=(*first);				//get the first node
        (*first)=(*last)=NULL;		//set first and last to NULL
        return temp;				//return the first node
    }
	else								//more than one node in the list
	{
		temp=(*first);					//get the first node		
		(*first) = (*first) -> next;	//set first to the next node	
		return temp;					//return the first node
	}
}

//function for displaying the help text
void displayHelp()
{
	printf("\n===Help===\n");
	printf("-> Enter problems in infix form.\n");
	printf("-> All angles in degrees.\n");
	printf("-> Supported operations: + - * / ^\n");
	printf("-> Supported functions: sin cos tan asin acos atan sqrt log exp abs\n");
	printf("-> Supported constants: pi e c g ans(zero init)\n");
	printf("-> Supported variables (assign with =, zero init): x1-x10\n");
}
