#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void hf1a(int n);
void hf1b(int n);
void hf1c(int n);
void hf1d(int n);


int main(int argc, char* argv[])
{
    if (argc!=3) {
        printf("Nem adtad meg a szükseges 2 argumentumot!");
        return -1;
    }

    if(strcmp(argv[1],"a")==0)
        hf1a(atoi(argv[2]));
    else if (strcmp(argv[1],"b")==0)
        hf1b(atoi(argv[2]));
    else if (strcmp(argv[1],"c")==0)
        hf1c(atoi(argv[2]));
    else if (strcmp(argv[1],"d")==0)
        hf1d(atoi(argv[2]));
    else
        printf("Wrong argument!\n");

    return 0;
}


void hf1a(int n)
{
    for (int k=1;k<n;k++)
    {
        for (int j=1;j<n;j++)
        {
            for (int i=1;i<n;i++)
            {
                if (i*i+j*j==k*k)
                    printf("%d,%d,%d\n",i,j,k);
                else if (i*i+k*k==j*j)
                    printf("%d,%d,%d\n",i,j,k);
                else if (j*j+k*k==i*i)
                    printf("%d,%d,%d\n",i,j,k);
            }
        }
    }
}


void hf1b(int n) //i,j,k es j,i,k ismetlodesek (i*i+j*j=k*k) ; i<k
{
    int s=0, k=2;
    while (1)
    {
        int j=1;
        while (j<k)
        {
            int i=1;
            while(i<k)
            {
                if (i*i+j*j==k*k)
                {
                    printf("%d,%d,%d\n",i,j,k);
                    s+=1;
                    if (s==n)
                        return;
                }
                i++;
            }
            j++;
        }
        k++;
    }
}


void hf1c(int n)  // a*a+b*b=c*c eseten c szerint no es a<b<c. Elteres hf1b-tol: while (i<j), (i<k) helyett
{
    int s=0, k=2;
    while (1)
    {
        int j=1;
        while (j<k)
        {
            int i=1;
            while(i<j)
            {
                if (i*i+j*j==k*k)
                {
                    printf("%d,%d,%d\n",i,j,k);
                    s+=1;
                    if (s==n)
                        return;
                }
                i++;
            }
            j++;
        }
        k++;
    }

}


void hf1d(int n)
{
    int* tomb=(int*)malloc(3*n*sizeof(int));
    int s=0, k=2;
    while (1) {
        int j=1;
        while (j<k) {
            int i=1;
            while (i<j) {
                if (i*i+j*j==k*k) {
                    if (s==0) {
                        printf("%d,%d,%d \n",i,j,k);
                        tomb[s]=i;
                        tomb[s+1]=j;
                        tomb[s+2]=k;
                        s+=3;
                        if (s==3*n)
                            return;
                    }
                    else {
                        double di=i;
                        double dj=j;
                        int l=0;
                        while (1)
                        {
                            if (di/tomb[l]==dj/tomb[l+1])
                                break;
                            if (l==s-3)
                            {
                                printf("%d,%d,%d \n",i,j,k);
                                tomb[s]=i;
                                tomb[s+1]=j;
                                tomb[s+2]=k;
                                s+=3;
                                if (s==3*n)
                                    return;
                                break;
                            }
                            l+=3;
                        }
                    }
                }
                i++;
            }
            j++;
        }
        k++;
    }
}