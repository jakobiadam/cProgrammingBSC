#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct matrix {
    double *p;
    int rows;
    int cols;
} matrix_t;

matrix_t alloc_vector(int rows, int cols);
void read_matrix(FILE* f, matrix_t m);
void write_matrix(matrix_t m);
void unit_matrix(int n, matrix_t* mat);
int abs_max_rows(int i, matrix_t* mat);
void multiply_row(int i, matrix_t* mat, double d);
void gje_norm_matrix(matrix_t* mat, matrix_t* unit);
int gje_partial_pivot(int j, matrix_t* mat);
void gje_swap_rows(matrix_t* mat, int i1, int i2);
void gje_swap_cols(matrix_t* mat, int j1, int j2);
void gje_substract_rows(matrix_t* mat, double a, int i1, int i2);
void gje_elim_down(matrix_t* mat, matrix_t* unit);
void gje_elim_up(matrix_t* mat, matrix_t* unit);
void gje_start(matrix_t* mat, matrix_t* unit);
void gje_invert(matrix_t* mat);
void gje_solve(matrix_t* mat, matrix_t* b);

int main(int argc, char *argv[])
{
    if (argc==4) {
        int rows=atoi(argv[2]);
        int cols=atoi(argv[3]);

        matrix_t mat=alloc_vector(rows,cols);
        FILE* f=fopen(argv[1],"r");
        read_matrix(f, mat);

        gje_invert(&mat);
    } else if (argc==7) {
        int rows=atoi(argv[2]);
        int cols=atoi(argv[3]);
        int rowsb=atoi(argv[5]);
        int colsb=atoi(argv[6]);

        matrix_t mat=alloc_vector(rows,cols);
        FILE* f=fopen(argv[1],"r");
        read_matrix(f, mat);
        matrix_t matb=alloc_vector(rowsb,colsb);
        FILE* fb=fopen(argv[4],"r");
        read_matrix(fb, matb);

        gje_solve(&mat, &matb);
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

void unit_matrix(int n, matrix_t* mat) {
    if ((*mat).rows!=(*mat).cols) {
        printf("Nem negyzetes matrix, nem lehet egyseg matrix!");
        return;
    }
    for (int i=0;i<n;i++) {
        for (int j=0;j<n;j++) {
            if (i==j) {
                (*mat).p[i*n+j]=1;
            } else {
                (*mat).p[i*n+j]=0;
            }
        }
    }
}

int abs_max_rows(int i, matrix_t* mat) {
    double abs_max=0;
    int index_col;
    for (int j=0;j<(*mat).cols;j++) {
        if (fabs((*mat).p[i*(*mat).cols+j])>abs_max && fabs((*mat).p[i*(*mat).cols+j]-abs_max)>0.000000001) {
            abs_max=fabs((*mat).p[i*(*mat).cols+j]);
            index_col=j;
        }
    }
    if (fabs(abs_max)<0.000000001) {
        printf("Nem megoldhato egyenletrendszer (van egy csupa 0 sor)!");
        exit(-1);
    }
    return index_col;
}

void multiply_row(int i, matrix_t* mat, double d) {
    for (int j=0;j<(*mat).cols;j++) {
        (*mat).p[i*(*mat).cols+j]*=d;
    }
}

void gje_norm_matrix(matrix_t* mat, matrix_t* unit) {
    for (int i=0;i<(*mat).rows;i++) {
        int max_elem=abs_max_rows(i, mat);
        double max_elem_ertek=(*mat).p[i*(*mat).cols+max_elem];
        multiply_row(i, mat, 1.0/max_elem_ertek);
        multiply_row(i, unit, 1.0/max_elem_ertek);
    }
}

int gje_partial_pivot(int j, matrix_t* mat) {
    double abs_max=0;
    int index_row;
    for (int i=j;i<(*mat).rows;i++) {
        if (fabs((*mat).p[i*(*mat).cols+j])>abs_max && fabs((*mat).p[i*(*mat).cols+j]-abs_max)>0.000000001) {
            abs_max=(*mat).p[i*(*mat).cols+j];
            index_row=i;
        }
    }
    if (fabs(abs_max)<0.000000001) {
        printf("Nem megoldhato egyenletrendszer!");
        exit(-1);
    }
    return index_row;
}

int gje_teljes_pivot(int j, matrix_t* mat) {
    double abs_max=0;
    int index_row;
    int index_col;
    for (int i=j;i<(*mat).rows;i++) {
        for (int j_futo=j;j_futo<(*mat).cols;j_futo++) {
            if (fabs((*mat).p[i*(*mat).cols+j])>abs_max && fabs((*mat).p[i*(*mat).cols+j]-abs_max)>0.000000001) {
                abs_max=(*mat).p[i*(*mat).cols+j_futo];
                index_row=i;
                index_col=j_futo;
            }
        }
    }
    if (fabs(abs_max)<0.000000001) {
        printf("Nem megoldhato egyenletrendszer!");
        exit(-1);
    }
    gje_swap_cols(mat, j, index_col);


    return index_row;
}

void gje_swap_rows(matrix_t* mat, int i1, int i2) {
    matrix_t csere=alloc_vector(1, (*mat).cols);
    for (int j=0;j<(*mat).cols;j++) {
        csere.p[j]=(*mat).p[i1*(*mat).cols+j];
        (*mat).p[i1*(*mat).cols+j]=(*mat).p[i2*(*mat).cols+j];
    }
    for (int j=0;j<(*mat).cols;j++) {
        (*mat).p[i2*(*mat).cols+j]=csere.p[j];
    }
    free(csere.p);
}

void gje_swap_cols(matrix_t* mat, int j1, int j2) {
    matrix_t csere=alloc_vector((*mat).rows, 1);
    for (int i=0;i<(*mat).rows;i++) {
        csere.p[i]=(*mat).p[i*(*mat).cols+j1];
        (*mat).p[i*(*mat).cols+j1]=(*mat).p[i*(*mat).cols+j2];
    }
    for (int i=0;i<(*mat).cols;i++) {
        (*mat).p[i*(*mat).cols+j2]=csere.p[i];
    }
    free(csere.p);
}

//i1-a*i2
void gje_substract_rows(matrix_t* mat, double a, int i1, int i2) {
    for (int j=0;j<(*mat).cols;j++) {
        (*mat).p[i1*(*mat).cols+j]-=a*(*mat).p[i2*(*mat).cols+j];
    }
}

void gje_elim_down(matrix_t* mat, matrix_t* unit) {
    double d;
    for (int i=0;i<(*mat).rows;i++) {
        if (fabs((*mat).p[i*(*mat).cols+i])<0.000000001) {
            int pivot=gje_partial_pivot(i, mat);
            gje_swap_rows(mat, i, pivot);
            gje_swap_rows(unit, i, pivot);
        }
        d=(*mat).p[i*(*mat).cols+i];
        multiply_row(i,mat,1.0/d);
        multiply_row(i,unit,1.0/d);
        for (int i2=i+1;i2<(*mat).rows;i2++) {
            d=(*mat).p[i2*(*mat).cols+i];
            gje_substract_rows(mat,d,i2,i);
            gje_substract_rows(unit,d,i2,i);
        }
    }
}

void gje_elim_up(matrix_t* mat, matrix_t* unit) {
    double d;
    for (int i=(*mat).rows-1;i>0;i--) {
        for (int i2=i-1;i2>=0;i2--) {
            d=(*mat).p[i2*(*mat).cols+i];
            gje_substract_rows(mat,d,i2,i);
            gje_substract_rows(unit,d,i2,i);
        }
    }
}

void gje_start(matrix_t* mat, matrix_t* unit) {
    gje_norm_matrix(mat, unit);
    gje_elim_down(mat, unit);
    gje_elim_up(mat, unit);
}

void gje_invert(matrix_t* mat) {
    matrix_t unit=alloc_vector((*mat).rows, (*mat).cols);
    unit_matrix((*mat).rows,&unit);
    gje_start(mat, &unit);
    write_matrix(*mat);
    printf("\nA matrix inverze:\n\n");
    write_matrix(unit);
}

void gje_solve(matrix_t* mat, matrix_t* b) {
    gje_start(mat, b);
    write_matrix(*mat);
    printf("\nAz egyenletrendszer(ek) megoldasai:\n\n");
    write_matrix(*b);
}
