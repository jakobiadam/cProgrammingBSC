#include <stdio.h>
#include <stdlib.h>

int hf1a(int n);
int hf1a2(int n);
int hf1b(int n);
int hf1c(int n);
int hf1c2(int n);
int hf1d(int n);
int hf1d2(int n);
int lnko(int a, int b);

int main(int argc, char* argv[])
{
    if(strcmp(argv[1],"a")==0)
        hf1a(atof(argv[2]));
    else if (strcmp(argv[1],"b")==0)
        hf1b(atof(argv[2]));
    else if (strcmp(argv[1],"c")==0)
        hf1c(atof(argv[2]));
    else if (strcmp(argv[1],"d")==0)
        hf1d(atof(argv[2]));
    else
        printf("Wrong argument!\n");

    return 0;
}



int lnko(int a, int b) //Euklideszi algoritmus legnagyobb kozos osztora
{
    int t;
    while (b!=0) {
        t=b;
        b=a%b;
        a=t;
    }
    return a;
}



int hf1a(int n) //j,k<n
{
    for (int k=1;k<n;k++)
    {
        for (int j=1;j<n;j++)
        {
            for (int i=1;i<n;i++)
            {
                if (i*i+j*j==k*k)
                {
                    printf("%d,%d,%d\n",i,j,k);
                }
            }
        }
    }
}



int hf1a2(int n) //nincs ismetlodes, csak tobbszoros. j<k,i<j
{
    for (int k=1;k<n;k++)
    {
        for (int j=1;j<k;j++)
        {
            for (int i=1;i<j;i++)
            {
                if (i*i+j*j==k*k)
                {
                    printf("%d,%d,%d\n",i,j,k);
                }
            }
        }
    }
}



int hf1b(int n) //i,j,k es j,i,k ismetlodesek (i*i+j*j=k*k) ; i<k
{
    int s=0;
    int k=2;
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
                    {
                        return 0;
                    }
                }
                i+=1;
            }
            j+=1;
        }
        k+=1;
    }

}



int hf1c(int n) // a*a+b*b=c*c eseten b szerint no es a<b<c, sokkal gyorsabb hf1c2 verzional
{
    double test;
    int k, s=0; //s szamlalja a kiirt jo megoldasokat
    int i=2;
    while (1) {
        int j=1;
        while (j<i) {
            test=sqrt(i*i+j*j);
            k=test; //test double tipusu, amikor az erteket beirom int tipusba, a program levagja a tizedesvesszo utani reszt
            if (k==test) { //k==test, ha i*i+j*j egesz szam, vagyis ha elcsiptunk egy jo szamharmast
                printf("%d,%d,%d \n",j,i,k);
                s+=1;
                if (s==n) {
                    return 0;
                }
            }
            j+=1;
        }
        i+=1;
    }
}



int hf1c2(int n)  // a*a+b*b=c*c eseten c szerint no es a<b<c. Elteres hf1b-tel: while (i<j), (i<k) helyett
{
    int s=0;
    int k=2;
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
                    {
                        return 0;
                    }
                }
                i+=1;
            }
            j+=1;
        }
        k+=1;
    }

}



int hf1d(int n) //hf1c kiegeszitve egy relativ prim tesztelovel (ha minden a relprim b, akkor nincsenek szamsoros megoldasok)
{
    double test;
    int k, s=0;
    int i=2;
    while (1)
    {
        int j=1;
        while (j<i)
        {
            test=sqrt(i*i+j*j);
            k=test;
            if (k==test)
                {
                if (lnko(i,j)==1) //csak akkor irja ki a szamharmast, ha i es j relativ primek is
                {
                    printf("%d,%d,%d \n",j,i,k);
                    s+=1;
                    if (s==n)
                    {
                        return 0;
                    }
                }
            }
            j+=1;
        }
        i+=1;
    }
}



int hf1d2(int n) //bonyolultabb, tobb memoriat igenyel, kb ugyanolyan gyors mint hf1d
{
    int tomb[n][3];
    double test;
    int k, s=0;
    int i=2;
    while (1) {
        int j=1;
        while (j<i) {
            test=sqrt(i*i+j*j);
            k=test;
            if (k==test) {
                if (s==0)
                {
                    printf("%d,%d,%d \n",j,i,k);
                    tomb[s][0]=j;
                    tomb[s][1]=i;
                    tomb[s][2]=k;
                    s+=1;
                    if (s==n)
                    {
                        return 0;
                    }
                }
                else
                {
                    double dj=j;
                    double di=i;
                    int l=0;
                    while (1)
                    {
                        if (dj/tomb[l][0]==di/tomb[l][1])
                        {
                            break;
                        }
                        if (l==s-1)
                        {
                            printf("%d,%d,%d \n",j,i,k);
                            tomb[s][0]=j;
                            tomb[s][1]=i;
                            tomb[s][2]=k;
                            s+=1;
                            if (s==n)
                            {
                                return 0;
                            }
                            break;
                        }
                        l+=1;
                    }
                }
            }
            j+=1;
        }
        i+=1;
    }
}
