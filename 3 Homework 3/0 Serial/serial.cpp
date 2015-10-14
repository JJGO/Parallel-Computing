
#include <stdio.h>
#include <stdlib.h>
// #include <iostream>
#include <math.h>
#include <time.h>

#define IT 10

// using namespace std;

double** new_matrix(int height, int width);
void delete_matrix(double** matrix);
void initialize_matrix(double** A, int size);
void transform_matrix(double** A,double** B, int N);

void print_matrix(double** A, int N);

int main(int argc, char const *argv[])
{
    int N;
    double sumA=0.0, minA;
    char *p;
    double **A, **B;

    clock_t begin, end;
    double time_spent;


    if( argc > 1)
    {
        N = strtol(argv[1], &p, 10);

        begin = clock();
        
        A = new_matrix(N,N);
        B = new_matrix(N,N);
        
        initialize_matrix(A,N);

        for (int i = 0; i < IT; ++i)
        {
            transform_matrix(A,B,N);
        }

        minA = A[0][0];

        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                sumA += fabs(A[i][j]);
                minA = A[i][j] < minA ? A[i][j] : minA;
            }
        }
        // print_matrix(A,N);

        delete_matrix(A);
        delete_matrix(B);
        printf("\n SUM : %lf\n", sumA );
        printf("\n MIN : %lf\n", minA );
        end = clock();
        time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("\nTIME : %lf seconds\n", time_spent);
    }

    return 0;

}

double** new_matrix(int height, int width)
{
    double** matrix;
    matrix = new double*[width];
    matrix[0] = new double[width * height];
    for (int i = 1; i < width; i++)
        matrix[i] = matrix[i-1] + height;
    return matrix;
}

void delete_matrix(double** matrix)
{
    delete[] matrix[0];
    delete[] matrix;
}


void initialize_matrix(double** A, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if(i == j){
                A[i][j] = i*sin(sqrt(i));
            }else{
                A[i][j] = pow(i+j,1.1);
            }
        }
    }

    return;
}

void transform_matrix(double** A,double** B, int N)
{
    double x;
    int ip, ipp;
    for (int i = 0; i < N; i++)
    {
        ip = i != N-1 ? i+1 : 0;
        ipp = i != 0 ? i-1 : N-1;
        
        for (int j = 0; j < N-1; j++)
        {
            if( i != j)
            {
                x = 0.0;
                for(int k = 1 ; k <= 10 ; k++){
                    x += pow( fabs( 0.5 + A[ip][j] ) , 1.0/double(k));
                    x -= pow( fabs( A[ipp][j] ) , 1.0/double(k+1)) * pow( fabs( A[i][j+1] ) , 1.0/double(k+2));
                }
                x = x < 10.0 ? x : 10.0;
                x = x > -10.0 ? x : -10.0;
                B[i][j] = x;
            }else{
                B[i][j] = A[i][j];
            }
        }
    }
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N-1; j++)
        {
            A[i][j] = B[i][j];
        }
    }

    return;
}


void print_matrix(double** A, int N)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%lf ", A[i][j]);
        }
        printf("\n");
    }
}

