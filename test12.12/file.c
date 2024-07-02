#include <stdio.h>
#include <stdlib.h>

#define STU_N 40
#define COURSE_N 3

int inputData(long stuid[], int score[][COURSE_N]);
void totalScForStu(int score[][COURSE_N], int totalstu[], int n);
void averageScForStu(int totalstu[], float averstu[],int n);
void totalScForCourse(int score[][COURSE_N], int totalcourse[], int n);
void averageScForCourse(int totalcourse[],float avercourse[], int n);
void sortByAverageScForStu(long stuid[], int score[][COURSE_N], int totalstu[], float averstu[], int n);
void print1(long stuid[], int score[][COURSE_N],int totalstu[],float averstu[],int totalcourse[],float avercourse[],int n );
void print2(long stuid[], int score[][COURSE_N], int totalstu[],float averstu[], int totalcourse[],float avercourse[], int n);
int main()
{
    long stuld[STU_N];
    int score[STU_N][COURSE_N], totalStu[STU_N], totalCourse[COURSE_N], n;float averCourse[COURSE_N], averStu[STU_N];
    n = inputData(stuld, score);
    totalScForStu(score, totalStu, n);
    averageScForStu(totalStu,averStu,n);
    totalScForCourse(score,totalCourse, n);
    averageScForCourse(totalCourse, averCourse, n);
    sortByAverageScForStu(stuld, score,totalStu, averStu, n);
    print2(stuld,score, totalStu, averStu, totalCourse, averCourse, n);
    return 0;
}
int inputData(long stuid[], int score[][COURSE_N])
{
    FILE*fp=fopen("data.txt","r");
    if (fp == NULL)
    {
        printf("open file error!"); exit(1);
    }
    int i= 0;
    while (!feof(fp)) 
    {
        fscanf(fp,"%ld ,%d,%d,%d" , &stuid[i], &score[i][0], &score[i][1], &score[i][2]);
        if(!feof(fp))
            i++;
    }
    fclose(fp);
    return i;
}
void totalScForStu(int score[][COURSE_N], int totalstu[], int n)
{
    for (int i = 0; i < n; i++)
    {
        totalstu[i] = 0;
        for (int j = 0; j< COURSE_N; j++)
        {
            totalstu[i] += score[i][j];  
        }
    }
}
void averageScForStu(int totalstu[], float averstu[], int n)
{
    for (int i = 0; i< n; i++) 
    {
        averstu[i] = (float)totalstu[i] / COURSE_N;
    }
}

void totalScForCourse(int score[][COURSE_N], int totalcourse[], int n)
{
    for (int i = 0; i<COURSE_N; i++)
    {
        totalcourse[i]= 0;
        for (int j = 0; j < n; j++)
        {
            totalcourse[i] += score[j][i];
        }
    }
}

void averageScForCourse(int totalcourse[], float avercourse[], int n) 
{
    for (int i = 0; i < COURSE_N; i++) 
    {
        avercourse[i] = (float)totalcourse[i] / n;
    }
}
void sortByAverageScForStu(long stuid[], int score[][COURSE_N], int totalstu[],float averstu[], int n)
{
    int k, tmptotalstu, tmpscore;
    long tmpld;
    float tmpaverstu;
    for (int i = 0; i<n-1; i++)
    {
        k = i;
        for (int j = i+ 1; j<n- 2; j++)
        {   
            if (averstu[i] < averstu[j])
                k = j;
        }
        if (k != i)
        {
            tmpld = stuid[i];
            stuid[i]= stuid[k];
            stuid[k]=tmpld;
            for (int j = 0; j <COURSE_N; j++)
            {
                tmpscore = score[i][j]; 
                score[i][j] = score[k][j];
                score[k][i] = tmpscore;
            }
            tmptotalstu = totalstu[i]; 
            totalstu[i] = totalstu[k]; 
            totalstu[k] = tmptotalstu;
            tmpaverstu = averstu[i]; 
            averstu[i] = averstu[k]; 
            averstu[k] = tmpaverstu;
        }
    }
}

void print1(long stuid[], int score[][COURSE_N],int totalstu[],float averstu[],int totalcourse[],float avercourse[],int n)
{
    for (int i = 0; i<n; i++)
    {
        printf("%-7ld" , stuid[i]);
        for (int j = 0; j < COURSE_N; j++)
            printf("%-8d" , score[i][j]);
        printf("%-7d" , totalstu[i]);
        printf("%-6.1f\n" , averstu[i]);
    }
    printf("    ");
    for (int i = 0; i< COURSE_N; i++)
        printf("%-8d", totalcourse[i]);
    printf("\n    ");
    for (int i = 0; i< COURSE_N; i++)
        printf("%-8.1f", avercourse[i]);
}

void print2(long stuid[], int score[][COURSE_N], int totalstu[], float averstu[],int totalcourse[], float avercourse[], int n) 
{
    FILE* fp;
    if ((fp = fopen("data1.txt" ,"w") ) == NULL) 
    {
        printf("open file failed!"); 
        exit(0);
    }
    fprintf(fp,"%-10s%-10s%-10s%-10s%-10s%-10s\n","ID" ,"score1" ,"score2" ,"score3" ,"sum" ,"avar");
    for(int i=0 ;i<n;i++)
    {
        fprintf(fp,"%-10d" , stuid[i]);
        for (int j = 0; j < COURSE_N; j++)
        {
            fprintf(fp, "%-10d" , score[i][j]);
        }
        fprintf(fp, "%-10d%-10.1f\n" , totalstu[i], averstu[i]);
    }
    fprintf(fp, "%-10s%-10d%-10d%-10d\n","sum",totalcourse[0], totalcourse[1], totalcourse[2]);
    fprintf(fp, "%-10s%-10.1f%-10.1f%-10.1f" , "aver" , avercourse[0], avercourse[1], avercourse[2]);
    fclose(fp);

}

