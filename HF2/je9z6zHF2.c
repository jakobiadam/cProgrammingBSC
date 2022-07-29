#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void hf2a(double* v, int n);
void hf2b(double* v, int n);
void hf2c(char* argv3,void *v,size_t n,size_t size, int (*comparisonfunc)(const void *, const void*));
int comparisonfunc(const void* x, const void* y);
void sort_own(double *base, int n, int (*compar)(const void *, const void*));
int partition(double *base,int left, int right);
void qsort_own(double *base, int left, int right);
int kereses(double *base,int n, double szam, int (*compar)(const void *, const void*));

int main(int argc, char* argv[])
{
    if (argc<4 || argc>5) {
        printf("Nem megfelelo szamu argumentum.\n");
        printf("argv[0]: maga a program\n");
        printf("argvf[1]: beolvasott vektorok szama\n");
        printf("argvf[2]: beolvasando File neve\n");
        printf("argvf[3]: qsort eseten a beepitett, qsort_own eseten a sajat rendezo fuggveny fog lefutni\n");
        printf("argvf[4]: keresett szam (opcionalis)\n");
        exit(-1);
    }

    int n=atoi(argv[1]);

    char* filename = argv[2];
    FILE* f=fopen(filename,"r");

    double* v=(double*)malloc(n*sizeof(double));
    for (int i=0;i<n;i++)
    {
        fscanf(f,"%lf",&v[i]);
    }
    printf("Vektor elemei a File beolvasasabol:\n");
    for (int i=0;i<n;i++)
    {
        printf("%f\t",v[i]);
    }
    printf("\n\n");

    hf2a(v,n);

    printf("\n\n");

    hf2b(v,n);

    hf2c(argv[3],v,n,sizeof(double),comparisonfunc);

    printf("\n\n");
    printf("Novekvo sorrend kiiratas:\n");
    for (int i=0;i<n;i++) {
        printf("%f  ",v[i]);
    }

    printf("\n\n");

    if (argc>=5) {
        double szam=atof(argv[4]);
        kereses(v,n,szam,comparisonfunc); }

    free(v);
    fclose(f);

    return 0;
}


void hf2a(double* v, int n)
{
    int max_index=0;
    for (int i=1;i<n;i++) {
        if (v[i]>v[max_index])
            max_index=i; }
    printf("A legnagyobb elem indexe %d, erteke %f.\n",max_index,v[max_index]);
}


void hf2b(double* v, int n) {
    printf("Vektor elemei kettesevel visszafele:\n");
    for (int i=0;i<n;i++) {
        if (i%2==0)
            printf("%f\t",v[n-1-i]);
    }

}


void hf2c(char* argv3,void *v,size_t n,size_t size, int (*comparisonfunc)(const void *, const void*)) {
    if (strcmp(argv3,"qsort")==0)
        qsort(v,n,sizeof(double),comparisonfunc);
    else if (strcmp(argv3,"qsort_own")==0) {
        int left=0;
        int right=n-1;
        qsort_own(v, left, right);
    }
    else if (strcmp(argv3,"sort_own")==0)
        sort_own(v, n, comparisonfunc);
    else {
        printf("argv[3] nem jo, helyes erteke qsort vagy qsort_own vagy sort_own");
        exit(-1);
    }
}


int comparisonfunc (const void * a, const void * b)
{
  if (*(double*)a > *(double*)b)
    return 1;
  else if (*(double*)a < *(double*)b)
    return -1;
  else
    return 0;
}


void sort_own(double *base, int n, int (*compar)(const void *, const void*)) {
    double a;
    for (int i=0;i<n-1;i++) {
        for (int j=i+1;j<n;j++) {
            if (comparisonfunc(&base[i],&base[j])==1) {
                a=base[i];
                base[i]=base[j];
                base[j]=a;
                i-=1;
                break;
            }

        }
    }
}


int partition(double *base,int left, int right) {
    double pivot=base[right];
    double swap;
    int i=left;
    for (int j=left;j<right;j++) {
        if (base[j]<pivot) {
            swap=base[j];
            base[j]=base[i];
            base[i]=swap;
            i++;
        }
    }
    base[right]=base[i];
    base[i]=pivot;
    return i;
}


void qsort_own(double *base, int left, int right) {
    if (left<right) {
        int p=partition(base, left, right);
        qsort_own(base, left, p-1);
        qsort_own(base, p+1, right);
    }
}


int kereses(double *base,int n, double szam, int (*compar)(const void *, const void*)) {
    int a=0;
    int b=n-1;
    int c=-1;
    while (b-a!=1) {
        if ((b-a+1)%2==0)
            c=a+(b-a+1)/2 -1;
        else
            c=a+(b-a)/2;

        if (szam==base[c]) {
            printf("Megvan a szam! Index: %d.\n",c);
            return 1;
        }
        else if (szam>base[c])
            a=c;
        else
            b=c;
    }
    if (szam==base[a]) {
        printf("Megvan a szam! Index: %d.\n",a);
        return 1; }
    if (szam==base[b]) {
        printf("Megvan a szam! Index: %d.\n",b);
        return 1; }
    printf("Nincs ilyen szam a vektorban!");
    return 0;
}
