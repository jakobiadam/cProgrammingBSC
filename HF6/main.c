#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define m_Fold 5.972e24
#define m_Hold  7.348e22
#define R_Fold_Hold 384400000
#define gamma  6.67384e-11

void Euler(double h, double N, FILE* palya, FILE* energia){

    double x0 = R_Fold_Hold;
    double y0 = 0;
    double vx0 = 0;
    double vy0 = 1022;
    double x1;
    double y1;
    double vx1;
    double vy1;
    fprintf(palya,"%f %f\n", x0, y0);
    double mozgasi = 0.5*m_Hold*(vx0*vx0+vy0*vy0);
    double potencialis = -gamma*m_Hold*m_Fold/pow(x0*x0+y0*y0,0.5);
    fprintf(energia,"%d %f\n", 0, mozgasi+potencialis);

    for(int i = 1; i <= N; i++){
        x1 = x0+vx0*h;
        y1 = y0+vy0*h;
        vx1 = -gamma*m_Fold*x0*h/pow(x0*x0+y0*y0,1.5)+vx0;
        vy1 = -gamma*m_Fold*y0*h/pow(x0*x0+y0*y0,1.5)+vy0;
        fprintf(palya,"%f %f\n", x0, y0);
        mozgasi = 0.5*m_Hold*(vx0*vx0+vy0*vy0);
        potencialis = -gamma*m_Hold*m_Fold/pow(x0*x0+y0*y0,0.5);
        fprintf(energia,"%d %f\n", i, mozgasi+potencialis);
        x0=x1;
        y0=y1;
        vx0=vx1;
        vy0=vy1;
    }
}

int main()
{
    int N = 30000;
    double h = 750;

    FILE* palya = fopen("palya.txt","w");
    FILE* energia = fopen("energia.txt","w");

    Euler(h,N,palya,energia);

    fclose(palya);
    fclose(energia);
}
