#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>

int separateString(char cmd[], char sprCmd[][100]);
int verifyCommand(int cmdCount, char sprCmd[][100]);
int parseNumber(int cmdCount, char sprCmd[][100]);

int main()
{
	char cmd[100000]={}, sprCmd[1000][100]={};
	int cmdCount,res,i;
	do {
		printf("command> ");
		gets(cmd);								//get input command
		
		for(i=0;i<strlen(cmd);i++)				//for all characters
			cmd[i]=tolower(cmd[i]);				//convert all to lowercase
		cmdCount=separateString(cmd,sprCmd);	//split string with spaces	
		res=verifyCommand(cmdCount,sprCmd);		//verify if the command is valid
		
		if(res==0||res==1)						//0=OK or 1=end
			printf("OK\n");
		else if(res==2)							//2=syntax error
			printf("Syntax Error\n");
		else									//3=parameter error
			printf("Parameter Error\n");
		
	} while(res!=1);							//1=end
	printf("End program.\n");
	printf("Program written by 60070501064 Sirawit Lappisatepun");
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

//function for verifying commands
int verifyCommand(int cmdCount, char sprCmd[][100])
{
	int type;
	
	//list, pop, sort
	if(strcmp(sprCmd[0],"list")==0||strcmp(sprCmd[0],"pop")==0||strcmp(sprCmd[0],"sort")==0)
	{
		if(cmdCount==1) //no parameters
			type=0;
		else
			type=3;
	}
	//end
	else if(strcmp(sprCmd[0],"end")==0)
	{
		if(cmdCount==1)	//no parameters
			type=1;
		else
			type=3;
	}
	//delete, push, insert, search, peek
	else if(strcmp(sprCmd[0],"delete")==0||strcmp(sprCmd[0],"push")==0||strcmp(sprCmd[0],"insert")==0||strcmp(sprCmd[0],"search")==0||strcmp(sprCmd[0],"peek")==0)
	{
		if(parseNumber(cmdCount,sprCmd)==0&&cmdCount==2)	//one parameter, all numbers
			type=0;
		else
			type=3;
	}
	//add
	else if(strcmp(sprCmd[0],"add")==0)
	{
		if(parseNumber(cmdCount,sprCmd)==0&&cmdCount>1)		//at least one parameter, all numbers
			type=0;
		else
			type=3;
	}
	//invalid command
	else
		type=2;
	
	return type;	//return result, 0=OK, 1=END, 2=SYN ERR, 3=PARAM ERR
}

//function for parsing string into number
int parseNumber(int cmdCount, char sprCmd[][100])
{
	int i,result=0;
	char *endptr;
	double temp;
	
	for(i=1;i<cmdCount;i++)		//for all substrings
	{
		temp = strtod(sprCmd[i],&endptr);	//try to parse to double
		if((*endptr)!=0)					//if there's something left after parsing
			result=-1;						//mark as failed
	}
	
	return result;	//return parsing result
}