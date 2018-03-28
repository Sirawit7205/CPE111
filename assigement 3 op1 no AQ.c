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

scv* open(char name[],int *size);
int cmpNo(const void *a, const void *b);
int cmpStr2(const void *a, const void *b);
void mqsortNo(scv *data,int first,int last);
void mqsortStr(scv *data,int first,int last);
void show(scv *ddr);
void swap(scv *data,int i,int j);

int main()
{
    int size,i,j;
    scv *ddr,*mddr;
    time_t start,end;
    double best_mqNo,best_mqStr;
    double TIME,best_no,best_str;
    printf("Load file test.scv\n");
    ddr = open("test.csv",&size);
    printf("Total %d records read\n\n",size);
    mddr = (scv *)malloc(sizeof(scv) * size);
    memcpy(mddr,ddr,sizeof(scv) * size);
    for(i = 0; i < loop; i++)
    {
        memcpy(ddr,mddr,sizeof(scv) * size);
        start = clock();
        qsort(ddr,size, sizeof(scv), cmpNo);
        end = clock();
        TIME = (double)(end - start)/CLOCKS_PER_SEC;
        printf("Test qsort round #%d : TIME running >> %g sec\n",i+1,TIME);
        if(TIME < best_no || i <= 0)best_no = TIME;
    }
    printf("\n");
    for(i = 0; i < loop; i++)
    {
        memcpy(ddr,mddr,sizeof(scv) * size);
        start = clock();
        mqsortNo(ddr,0,size-1);
        end = clock();
        TIME = (double)(end - start)/CLOCKS_PER_SEC;
        printf("Test Quick sort round #%d : TIME running >> %g sec\n",i+1,TIME);
        if(TIME < best_mqNo || i <= 0)best_mqNo = TIME;
    }
    printf("\n");
    for(i = 0; i < loop; i++)
    {
        memcpy(ddr,mddr,sizeof(scv) * size);
        start = clock();
        qsort(ddr,size, sizeof(scv), cmpStr2);
        end = clock();
        TIME = (double)(end - start)/CLOCKS_PER_SEC;
        printf("Test qsort string round #%d : TIME running >> %g sec\n",i+1,TIME);
        if(TIME < best_str || i <= 0)best_str = TIME;
    }
    printf("\n");
    for(i = 0; i < loop; i++)
    {
        memcpy(ddr,mddr,sizeof(scv) * size);
        start = clock();
        mqsortStr(ddr,0,size-1);
        end = clock();
        TIME = (double)(end - start)/CLOCKS_PER_SEC;
        printf("Test Quick sort string round #%d : TIME running >> %g sec\n",i+1,TIME);
        if(TIME < best_mqStr || i <= 0)best_mqStr = TIME;
    }
    printf("\n");
    printf("Minimum time of C -> qsort >> %g sec\n",best_no);
    printf("Minimum time of Quick sort (partition) >> %g sec\n\n",best_mqNo);
    printf("Minimum time of C -> qsort string >> %g sec\n",best_str);
    printf("Minimum time of Quick sort string (partition) >> %g sec\n\n",best_mqStr);
    printf("Before sort number:\n");
    show(mddr);
    printf("After sort number:\n");
    qsort(ddr,size, sizeof(scv), cmpNo);
    show(ddr);
    printf("\n");
    printf("Before sort string:\n");
    show(mddr);
    printf("After sort number:\n");
    qsort(ddr,size, sizeof(scv), cmpStr2);
    show(ddr);
    return 0;
}

void show(scv *ddr)
{
    int i;
    i = 0;printf("\tdata[%5d] : %llu , %s , %s\n",i,(ddr + i)->No,(ddr + i)->set1,(ddr + i)->set2);
    i = 49999;printf("\tdata[%5d] : %llu , %s , %s\n",i,(ddr + i)->No,(ddr + i)->set1,(ddr + i)->set2);
    i = 99999;printf("\tdata[%5d] : %llu , %s , %s\n",i,(ddr + i)->No,(ddr + i)->set1,(ddr + i)->set2);
}

int cmpNo(const void *a, const void *b)
{
    const scv *aa = (scv*)a;
    const scv *bb = (scv*)b;
    if(aa->No < bb->No)return -1;
    else if(aa->No > bb->No)return 1;
    else return 0;
}

int cmpStr2(const void *a, const void *b)
{
    const scv *aa = (scv*)a;
    const scv *bb = (scv*)b;
    return strcmp(aa->set2,bb->set2);
}

scv* open(char name[],int *size)
{
    FILE *fpt;
    scv buf,*data;
    int i;
    *size = 0;
    if((fpt = fopen(name,"r")) != NULL)
    {
        while(fscanf(fpt,"%llu,%[^,],%[^\n]",&buf.No,buf.set1,buf.set2) == 3)(*size)++;
        fclose(fpt);
    }
    data = malloc((*size) * sizeof(scv));
    if(data == NULL)printf("Error! memory not allocated.");
    if((fpt = fopen(name,"r")) != NULL)
    {
        for(i = 0; i < (*size); i++)fscanf(fpt,"%llu,%[^,],%[^\n]",&(*(data + i)).No,(*(data + i)).set1,(*(data + i)).set2);
        fclose(fpt);
    }
    return data;
}

void swap(scv *data,int i,int j)
{
    scv buf;
    buf = *(data + i);
    *(data + i) = *(data + j);
    *(data + j) =  buf;
}

void mqsortNo(scv *data,int first,int last)
{
    int i=first,j=last;
    scv buf;
    if(first < last)
    {
        do
        {
            while((*(data + i)).No <= (*(data + j)).No && (i < j))j--;
            if((*(data + i)).No > (*(data + j)).No)swap(data,i++,j);
            while((*(data + i)).No <= (*(data + j)).No && (i < j))i++;
            if((*(data + i)).No > (*(data + j)).No)swap(data,i,j--);
        }
        while(i<j);
        if (first < j-1)mqsortNo(data, first, j-1);
        if (i+1 < last)mqsortNo(data, i+1, last);
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
            while(strcmp(((*(data + i))).set2,(*(data + j)).set2) <= 0 && (i < j))j--;
            if(strcmp((*(data + i)).set2,(*(data + j)).set2) > 0 )swap(data,i++,j);
            while(strcmp(((*(data + i))).set2,(*(data + j)).set2) <= 0 && (i < j))i++;
            if(strcmp((*(data + i)).set2,(*(data + j)).set2) > 0 )swap(data,i,j--);
        }
        while(i<j);
        if (first < j-1)mqsortStr(data, first, j-1);
        if (i+1 < last)mqsortStr(data, i+1, last);
    }
}
