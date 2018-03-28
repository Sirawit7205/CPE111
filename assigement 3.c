#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<time.h>

#define loop 5

typedef struct data
{
    long long int No;
    char set1[64];
    char set2[64];
} scv;

scv* open(char name[],scv *data,int *size);
int cmpNo(const void *a, const void *b);
int cmpStr2(const void *a, const void *b);



int main()
{
    int size;
    scv *data;
    scv *ddr;

    int i,j;

    time_t start,end;

    // qsort
    double T_read,best_no,best_str;
    double best_mqNo,best_mqStr;
    printf("start");

    for(i = 0; i < loop; i++)
    {
        ddr = open("test.csv",data,&size);
        start = clock();
        qsort(ddr,size, sizeof(scv), cmpNo);
        end = clock();

        T_read = (double)(end - start)/CLOCKS_PER_SEC;
        printf("loop time %d => %g\n",i+1,T_read);

        if(i <= 0)
            best_no = T_read;
        if(T_read < best_no)
            best_no = T_read;
    }
    printf("best time = %g\n\n\n",best_no);


    for(i = 0; i < loop; i++)
    {
        ddr = open("test.csv",data,&size);
        start = clock();
        qsort(ddr,size, sizeof(scv), cmpStr2);
        end = clock();

        T_read = (double)(end - start)/CLOCKS_PER_SEC;
        printf("loop time %d => %g\n",i+1,T_read);

        if(i <= 0)
            best_str = T_read;
        if(T_read < best_str)
            best_str = T_read;
    }
    printf("best time = %g\n\n\n",best_str);


    for(i = 0; i < loop; i++)
    {
        printf("%llu,%s,%s\n",(ddr + i)->No,(ddr + i)->set1,(ddr + i)->set2);
    }

    for(i = 0; i < loop; i++)
    {
        ddr = open("test.csv",data,&size);
        start = clock();
        mqsortNo(ddr,0,size-1);
        end = clock();

        T_read = (double)(end - start)/CLOCKS_PER_SEC;
        printf("loop time %d => %g\n",i+1,T_read);

        if(i <= 0)
            best_mqNo = T_read;
        if(T_read < best_mqNo)
            best_mqNo = T_read;

    }
    printf("my best time = %g\n\n\n",best_mqNo);


    for(i = 0; i < loop; i++)
    {
        printf("%llu,%s,%s\n",(ddr + i)->No,(ddr + i)->set1,(ddr + i)->set2);
    }



    printf("\n\n\n");
    for(i = 0; i < loop; i++)
    {
        ddr = open("test.csv",data,&size);
            for(i = 0; i < loop; i++)
    {
        printf("%llu,%s,%s\n",(ddr + i)->No,(ddr + i)->set1,(ddr + i)->set2);
    }
        start = clock();
        mqsortStr(ddr,0,size-1);
        end = clock();

        T_read = (double)(end - start)/CLOCKS_PER_SEC;
        printf("loop time %d => %g\n",i+1,T_read);

        if(i <= 0)
            best_mqStr = T_read;
        if(T_read < best_mqStr)
            best_mqStr = T_read;

    }
    printf("my best_mqStr time = %g\n\n\n",best_mqStr);

    for(i = 0; i < loop; i++)
    {
        printf("%llu,%s,%s\n",(ddr + i)->No,(ddr + i)->set1,(ddr + i)->set2);
    }



    return 0;
}

int cmpNo(const void *a, const void *b)
{
    const scv *aa = (scv*)a;
    const scv *bb = (scv*)b;

    if(aa->No < bb->No)
        return -1;
    else if(aa->No > bb->No)
        return 1;
    else return 0;
}

int cmpStr1(const void *a, const void *b)
{
    char scva[64],scvb[64];
    strcpy(scva,((scv*)a)->set1);
    strcpy(scvb,((scv*)b)->set1);
    return strcmp(scva,scvb);
}

int cmpStr2(const void *a, const void *b)
{
    const scv *aa = (scv *)a;
	const scv *bb = (scv *)b;
	return (strcmp(bb->set2,aa->set2));
}

scv* open(char name[],scv *data,int *size)
{
    FILE *fpt;
    scv buf;
    int i;
    *size = 0;

    if((fpt = fopen(name,"r")) != NULL)
    {
        while(fscanf(fpt,"%llu,%[^,],%[^\n]",&buf.No,buf.set1,buf.set2) == 3)
            (*size)++;
        fclose(fpt);
    }
    data = malloc((*size) * sizeof(scv));

    if(data == NULL)
    {
        printf("Error! memory not allocated.");
        exit(0);
    }

    if((fpt = fopen(name,"r")) != NULL)
    {
        for(i = 0; i < (*size); i++)
            fscanf(fpt,"%llu,%[^,],%[^\n]",&(*(data + i)).No,(*(data + i)).set1,(*(data + i)).set2);

        fclose(fpt);
    }
    return data;
}

void mqsortNo(scv *data,int first,int last)
{
    int i=first,j=last;
    scv buf;
    if(first < last)
    {
        do
        {
            while((*(data + i)).No <= (*(data + j)).No && (i < j))
                j--;
            if((*(data + i)).No > (*(data + j)).No)
            {
                buf.No = (*(data + i)).No;
                (*(data + i)).No = (*(data + j)).No;
                (*(data + j)).No =  buf.No;
                i++;
            }
            while((*(data + i)).No <= (*(data + j)).No && (i < j))
                i++;
            if((*(data + i)).No > (*(data + j)).No)
            {
                buf.No = (*(data + i)).No;
                (*(data + i)).No = (*(data + j)).No;
                (*(data + j)).No =  buf.No;
                j--;
            }
        }
        while(i<j);
        if (first < j-1)
            mqsortNo(data, first, j-1);
        if (i+1 < last)
            mqsortNo(data, i+1, last);
    }
}

void mqsortStr(scv *data,int first,int last)
{
    int i=first,j=last;
    scv buf;
    if(first < last)
    {
        do
        {
            while(strcmp(((*(data + i))).set2,(*(data + j)).set2) <= 0 && (i < j))
                j--;
            if(strcmp((*(data + i)).set2,(*(data + j)).set2) > 0 )
            {
                strcpy(buf.set2,(*(data + i)).set2);
                strcpy((*(data + i)).set2,(*(data + j)).set2);
                strcpy((*(data + j)).set2,buf.set2);
                i++;
            }
            while(strcmp(((*(data + i))).set2,(*(data + j)).set2) <= 0 && (i < j))
                i++;
             if(strcmp((*(data + i)).set2,(*(data + j)).set2) > 0 )
            {
                strcpy(buf.set2,(*(data + i)).set2);
                strcpy((*(data + i)).set2,(*(data + j)).set2);
                strcpy((*(data + j)).set2,buf.set2);
                j--;
            }
        }
        while(i<j);
        if (first < j-1)
            mqsortStr(data, first, j-1);
        if (i+1 < last)
            mqsortStr(data, i+1, last);
    }
}
