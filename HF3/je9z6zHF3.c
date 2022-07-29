#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct matrix {
    double *p;
    int rows;
    int cols;
} matrix_t;

matrix_t alloc_vector(int rows, int cols);
void read_matrix(FILE* f, matrix_t m);
void write_matrix(matrix_t m);
matrix_t matrix_szorzas(matrix_t m1, matrix_t m2);

double *realloc_matrix(double *matrix,int matrixrealloc);
matrix_t beolvb(FILE *f);

int main(int argc, char *argv[])
{
    if (argc==1 || !(((strcmp(argv[1],"a")==0) && argc==8) || ((strcmp(argv[1],"b")==0) && argc==4))) {
        printf("Nem megfelelo argumentumok.\n");
        printf("argv[0]: maga a program\n");
        printf("argv[1]: feladat szama: a vagy b\n");
        printf("argv[2]: matrix1 File neve\n");
        printf("argv[3]: matrix2 File neve\n");
        printf("argv[4]: rows1\n");
        printf("argv[5]: cols1\n");
        printf("argv[6]: rows2\n");
        printf("argv[7]: cols2\n");
        exit(-1);
    }

    FILE* f1=fopen(argv[2],"r");
    FILE* f2=fopen(argv[3],"r");

    if (argv[1][0]=='a') {
        int rows1=atoi(argv[4]);
        int cols1=atoi(argv[5]);
        int rows2=atoi(argv[6]);
        int cols2=atoi(argv[7]);
        matrix_t matrix1=alloc_vector(rows1,cols1);
        matrix_t matrix2=alloc_vector(rows2,cols2);
        read_matrix(f1, matrix1);
        read_matrix(f2, matrix2);
        printf("Matrix1:\n");
        write_matrix(matrix1);
        printf("\n");
        printf("Matrix2:\n");
        write_matrix(matrix2);
        printf("\n");
        if (matrix1.cols!=matrix2.rows) {
            printf("Nem osszeszorozhato matrixok.\n");
            return 0;
        }
        matrix_t matrix3=matrix_szorzas(matrix1, matrix2);
        printf("Matrixszorzat eredmenye:\n");
        write_matrix(matrix3);
        free(matrix1.p);
        free(matrix2.p);
        free(matrix3.p);
        }

    if (argv[1][0]=='b') {
        matrix_t matrix1=beolvb(f1);
        matrix_t matrix2=beolvb(f2);
        printf("Matrix1:\n");
        write_matrix(matrix1);
        printf("\n");
        printf("Matrix2:\n");
        write_matrix(matrix2);
        printf("\n");
        if (matrix1.cols!=matrix2.rows) {
            printf("Nem osszeszorozhato matrixok.\n");
            return 0;
        }
        matrix_t matrix3=matrix_szorzas(matrix1, matrix2);
        printf("Matrixszorzat eredmenye:\n");
        write_matrix(matrix3);
        free(matrix1.p);
        free(matrix2.p);
        free(matrix3.p);
        }

    return 0;
}

matrix_t alloc_vector(int rows, int cols) {
    matrix_t mat;
    mat.rows=rows;
    mat.cols=cols;
    mat.p = (double*)malloc(cols*rows*sizeof(double));
    if (mat.p==0) {
        printf("Memory allocation error.\n");
        exit(-1);
    }
    return mat;
}

void read_matrix(FILE* f, matrix_t m) {
    for (int i=0;i<m.rows;i++) {
        for (int j=0;j<m.cols;j++) {
            fscanf(f, "%lf", &m.p[i*m.cols+j]);
        }
    }
}

void write_matrix(matrix_t m) {
    for (int i=0;i<m.rows;i++) {
        for (int j=0;j<m.cols;j++) {
            printf("%10.4f  ", m.p[i*m.cols+j]);
        } printf("\n\n");
    }
}

matrix_t matrix_szorzas(matrix_t m1, matrix_t m2) {
    matrix_t m3=alloc_vector(m1.rows,m2.cols);
    if (m1.cols!=m2.rows) {
        printf("Nem osszeszorozhato matrixok.\n");
        return m3;
    }
    for (int i=0;i<m3.rows;i++) {
        for (int j=0;j<m3.cols;j++) {
            m3.p[i*m3.cols+j]=0;
            for (int k=0;k<m1.cols;k++) {
                m3.p[i*m3.cols+j]+=(m1.p[i*m1.cols+k] * m2.p[k*m2.cols+j]);
            }
        }
    }
    return m3;
}


double *realloc_matrix(double *matrix,int matrixrealloc) {
    matrix = (double*)realloc(matrix,matrixrealloc*matrixrealloc*sizeof(double));
    if (matrix==0) {
        printf("Memory allocation error.\n");
        exit(-1);
    }
    return matrix;

}

// '-' : szamok kozott szimplan elvalaszt, elojelet csak nem szam utan es kozvetlen szam elott jelez
//.1=1, 0.1=0.1, -0.1=-0.1, -.1=-0.1  --> ponttal nem kezdodik szam string, de -.2=-0.2 eredmenyt ad!
// minden mas karakter egyszeruen elvalasztja a szamokat
matrix_t beolvb(FILE *f) {
    matrix_t m;
    int matrixrealloc=3;
    m.p=(double*)malloc(matrixrealloc*matrixrealloc*sizeof(double));
    m.rows=0;
    m.cols=0;
    int colsx=0;
    int pontsz=0;
    char beolv;
    char *szam=(char*)malloc(21*sizeof(char));
    double szamf;
    int i=0;
    while (1) {
        beolv=fgetc(f);
        int j=0;
        while (beolv=='-' || beolv=='.' || beolv=='0' || beolv=='1' || beolv=='2' || beolv=='3' || beolv=='4' || beolv=='5' || beolv=='6' || beolv=='7' || beolv=='8' || beolv=='9') {
            if (beolv=='.' && pontsz==1) { //pont kezeles
                pontsz=0;
                j+=1;
                szam[j-1]='\0';
                break;
            } else if (beolv=='.' && j==0) {
                break;
            }
            if (beolv=='.') {
                pontsz=1;
            }
            if (beolv=='-' && j!=0 && szam[j-1]!='-') { // minusz kezeles
                j++;
                szam[j-1]='\0';
                break;
            } else if (!(beolv=='-' && j!=0)) {
                j+=1; //szamjegy stringbe olvasasa
                szam[j-1]=beolv;
            }

            if (feof(f)) { //file vege
                if (szam[j-1]=='-') {
                    break;
                }
                j+=1;
                szam[j-1]='\0';
                break; }
            beolv=fgetc(f); //kovetkezo karakter beolvasas es teszteles
            if (!(beolv=='-' || beolv=='.' || beolv=='0' || beolv=='1' || beolv=='2' || beolv=='3' || beolv=='4' || beolv=='5' || beolv=='6' || beolv=='7' || beolv=='8' || beolv=='9')) {
                if (szam[j-1]=='-') {
                    break;
                }
                j+=1;
                szam[j-1]='\0';
		pontsz=0;
                break;
            }
        }
        if (j>1 && !(j==3 && szam[j-3]=='-' && szam[j-2]=='.')) { //szam matrixba irasa ; -. eset kizarasa
            szamf=atof(szam);
            i+=1;
            if (i>=matrixrealloc) { //matrix ujrameretezes
                matrixrealloc+=1;
                m.p=realloc_matrix(m.p,matrixrealloc);
            }
            m.p[i-1]=szamf;
            colsx+=1;
        }
        if (beolv=='\n' || feof(f)) { //ujsor kezelese vagy file vege (~ujsor)
            if (colsx!=0) {
                m.rows+=1;
            }
            if (m.cols==0 && colsx!=0) {
                m.cols=colsx;
            }
            if (colsx!=0 && colsx!=m.cols) {
                printf("A FILE nem matrix alaku!");
                free(szam);
                return m;
            }
            if (feof(f)) {
                free(szam);
                return m;
            }
            colsx=0;
        }
    }
}