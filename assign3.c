#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

//struct for storing data
typedef struct data_str {
	unsigned long long int num;
	char textA[70];
	char textB[70];
} data;

int readData(const char filename[], data *ptr);
//void writedata(data *ptr,const char filename[],int amt);
void custom_qsort_num(data *dataSrt, int left, int right);
void custom_qsort_text(data *dataSrt, int left, int right);
void swap(data *dataSrt, int a, int b);
int cmp_num(const void *a, const void *b);
int cmp_txt(const void *a, const void *b);

int main()
{
	int amt,i,j,testRound=10;
	data *dataIn, *dataSrt;
	double bestTime[5]={}, curTime;
	clock_t start, end;
	
	printf("=====Quick sort test program.=====\n");
	printf("Reading file...\n");
	dataIn = (data *) malloc(150000 * sizeof(data));		//malloc struct for input
	dataSrt = (data *) malloc(150000 * sizeof(data));		//malloc struct for sorting
	
	amt=readData("test.csv", dataIn);		//read data from file
	
	if(amt==-1)								//reading error
		printf("File reading error. Exiting...\n\n");
	else
	{
		printf("%d records loaded.\n\n",amt);	//display amount of records
		
		for(i=0;i<4;i++)						//test no.
		{
			if(i==0)
				printf("Test 1: stdlib qsort by number.\n");	
			else if(i==1)
				printf("Test 2: stdlib qsort by text.\n");
			else if(i==2)
				printf("Test 3: custom qsort by number.\n");
			else
				printf("Test 4: custom qsort by text.\n");
			
			for(j=1;j<=testRound;j++)			//amount of testing
			{
				memcpy(dataSrt,dataIn,amt*sizeof(data));	//copy data from input (cache) struct to sorting struct
				printf(" Running iteration %2d...",j);
				start=clock();								//record start time
				
				if(i==0)
					qsort(dataSrt,amt,sizeof(data),cmp_num);	//stdlib qsort num
				else if(i==1)
					qsort(dataSrt,amt,sizeof(data),cmp_txt);	//stdlib qsort txt	
				else if(i==2)
					custom_qsort_num(dataSrt,0,amt-1);			//custom qsort num
				else
					custom_qsort_text(dataSrt,0,amt-1);			//custom qsort txt
				
				end=clock();									//record end time
				curTime=(double)(end-start)/CLOCKS_PER_SEC;		//calculate time in seconds	
				printf("OK Time used %lf seconds.\n",curTime);
				if(curTime<bestTime[i]||j==1)					//if current time is the first one or the best...
					bestTime[i]=curTime;						//record this time
			}
			
			//display index 0,49999,99999
			printf("Verification:\n Index 0:\t%llu\t%s\t%s\n Index 49999:\t%llu\t%s\t%s\n Index 99999:\t%llu\t%s\t%s\n\n",
				dataSrt[0].num,dataSrt[0].textA,dataSrt[0].textB,
				dataSrt[49999].num,dataSrt[49999].textA,dataSrt[49999].textB,
				dataSrt[99999].num,dataSrt[99999].textA,dataSrt[99999].textB);
			
			//debugging file output (for external comparison)
			/*			
			if(i==0)
				writedata(dataSrt,"stdlibsort.txt",amt);
			else if(i==1)
				writedata(dataSrt,"stdlibtextsort.txt",amt);
			else if(i==2)
				writedata(dataSrt,"customsort.txt",amt);	
			else
				writedata(dataSrt,"customtextsort.txt",amt);	
			*/
		}
		
		//display best time
		printf("Best time records:\n");
		printf(" stdlib qsort (num): %lf seconds.\n custom qsort (num): %lf seconds.\n\n stdlib qsort (txt): %lf seconds.\n custom qsort (txt): %lf seconds.\n\n",
			bestTime[0],bestTime[2],bestTime[1],bestTime[3]);		
	}

	free(dataIn);		//deallocate structs
	free(dataSrt);
	
	printf("End program.\n");
	printf("Program written by 60070501064 Sirawit Lappisatepun\n");
	return 0;
}

//function for reading input from file
int readData(const char filename[], data *ptr)
{
	FILE *fp;
	int amt=0;
	
	fp = fopen(filename,"r");	//open file for reading
	if(fp!=NULL)				//file open OK
	{
		while(fscanf(fp,"%llu,%[^,],%s",&ptr[amt].num,&ptr[amt].textA,&ptr[amt].textB)==3)	//file reading OK
			amt++;		//count and continue to read
		fclose(fp);		//close file
		return amt;		//return records count
	}
	else				//open error
		return -1;		//return -1 as error
}

//function for debug file output
/*
void writedata(data *ptr,const char filename[],int amt)
{
	FILE *fp;
	int i;
	fp=fopen(filename,"w");		//open file for writing
	for(i=0;i<amt;i++)
		fprintf(fp,"%llu,%s,%s\n",ptr[i].num,ptr[i].textA,ptr[i].textB);	//output all records to file
	fclose(fp);					//close file
}
*/

//custom qsort function for sorting by number
void custom_qsort_num(data *dataSrt, int left, int right)
{
	int i,j;
	
	if(left < right)	//still in bounds
	{
		i=left;			//set i as left bound
		j=right;		//set j as right bound
		
		while(i!=j)		//if middle element is not reached yet
		{
			while(dataSrt[i].num<dataSrt[j].num) i++;	//while left<right -> increase left bound...
			swap(dataSrt,i,j);							//until the false condition is met, then swap
			while(dataSrt[i].num<dataSrt[j].num) j--;	//while left<right -> decrease right bound...
			swap(dataSrt,i,j);							//until the false condition is met, then swap
		}
		if(i!=left) custom_qsort_num(dataSrt,left,i-1);		//if the left bound is not the leftmost index, recursive the left side of middle element
		if(j!=right) custom_qsort_num(dataSrt,j+1,right);	//if the right bound is not the rightmost index, recursive the right side of middle element
	}
}

//custom qsort function for sorting by text
void custom_qsort_text(data *dataSrt, int left, int right)
{
	int i,j;
	
	if(left < right)	//still in bounds
	{
		i=left;			//set i as left bound
		j=right;		//set j as right bound
		
		while(i!=j)		//if middle element is not reached yet
		{
			while(strcmp(dataSrt[i].textB,dataSrt[j].textB)<0) i++;	//while left<right -> increase left bound...
			swap(dataSrt,i,j);							//until the false condition is met, then swap
			while(strcmp(dataSrt[i].textB,dataSrt[j].textB)<0) j--;	//while left<right -> decrease right bound...
			swap(dataSrt,i,j);							//until the false condition is met, then swap
		}
		if(i!=left) custom_qsort_text(dataSrt,left,i-1);	//if the left bound is not the leftmost index, recursive the left side of middle element	
		if(j!=right) custom_qsort_text(dataSrt,j+1,right);	//if the right bound is not the rightmost index, recursive the right side of middle element
	}
}

//aux function for swapping two struct elements
void swap(data *dataSrt, int a, int b)
{
	data temp;
	
	temp=dataSrt[a];		//a --> temp
	dataSrt[a]=dataSrt[b];	//b --> a
	dataSrt[b]=temp;		//temp --> b
}

//qsort compare function for sorting unsigned long long int from least to most
int cmp_num(const void *a, const void *b)
{
	const data *aa = (data *)a;
	const data *bb = (data *)b;
	if(aa->num < bb->num) return -1;
	else if(aa->num > bb->num) return 1;
	else return 0;
}

//qsort compare function for sorting string from least to most
int cmp_txt(const void *a, const void *b)
{
	const data *aa = (data *)a;
	const data *bb = (data *)b;
	return (strcmp(aa->textB,bb->textB));
}