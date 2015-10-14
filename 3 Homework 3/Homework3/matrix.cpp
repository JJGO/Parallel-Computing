#include <mpi.h>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ITERATIONS 10

#define LEFT_COLUMN A[1][0]
#define RIGHT_HALO A[1][width]
#define UPPER_ROW A[1][0]
#define UPPER_HALO A[0][0]
#define LOWER_ROW A[height][0]
#define LOWER_HALO A[height+1][0]

double** new_matrix(int height, int width);
void delete_matrix(double** matrix);

void initialize_matrix(double** A, int size, int height, int width, int* coords);
void transform_matrix(double** A,double** B, int height, int width, int* coords);
void print_matrix(double** A, int height, int width);

char* gettime(char *buffer);

int main(int argc, char  *argv[])
{
    //////////////// VARIABLES ////////////////
    char *s;

    double **A, **B;
    int n, m, p, sqrt_p, height, width;
    int rank, coords[2];

    // int output = 0;

    MPI_Comm matrix_comm;
    static int dims [2];
    int periods [2] = { 1, 0 };
    int reorder = 1;
    int p_up, p_down, p_right, p_left;

    double local_sum=0.0, local_min;
    double starttime, endtime;

    double global_sum=0.0, global_min;

    char buffer[80];

    ///////////////// INIT  /////////////////

    MPI_Init (&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&p);
    sqrt_p = (int)sqrt(p);
    dims[0] = sqrt_p;
    dims[1] = sqrt_p;

    MPI_Cart_create(MPI_COMM_WORLD,2,dims,periods,reorder,&matrix_comm);
    
    MPI_Comm_rank(matrix_comm,&rank);
    MPI_Cart_coords(matrix_comm, rank, 2, coords);

    MPI_Datatype column;
    // The dimensions are reversed becuase MPI_Cart creates the matrix by columns and we want it by rows
    MPI_Cart_shift(matrix_comm, 1, -1, &p_right, &p_left);
    MPI_Cart_shift(matrix_comm, 0, -1, &p_down, &p_up);

    //////////////// PROGRAM ////////////////

    n = strtol(argv[1], &s, 10);

    m = ceil(float(n) / sqrt_p);
    height = coords[0] != sqrt_p-1 ? m : n - m*(sqrt_p-1);
    width  = coords[1] != sqrt_p-1 ? m : n - m*(sqrt_p-1);

    MPI_Type_vector (height, 1, width+1, MPI_DOUBLE, &column);
    MPI_Type_commit (&column);

    A = new_matrix(height+2,width+1);
    B = new_matrix(height+2,width+1);
    
    initialize_matrix(A,m,height,width,coords);

    MPI_Barrier(matrix_comm);
    if(rank == 0){
        starttime = MPI_Wtime();
    }
    
    for (int i = 0; i < ITERATIONS; ++i)
    {
        //// HALO SWAPPING ////
        // Up
        MPI_Sendrecv(&( UPPER_ROW ), width, MPI_DOUBLE, p_up, 0, &( LOWER_HALO ), width, MPI_DOUBLE, p_down, 0, matrix_comm, MPI_STATUS_IGNORE);
        // Down
        MPI_Sendrecv(&( LOWER_ROW ), width, MPI_DOUBLE, p_down, 0, &( UPPER_HALO ), width, MPI_DOUBLE, p_up, 0, matrix_comm, MPI_STATUS_IGNORE);
        // Left
        MPI_Sendrecv(&( LEFT_COLUMN ), 1, column, p_left, 0, &( RIGHT_HALO ), 1, column, p_right, 0, matrix_comm, MPI_STATUS_IGNORE);
    
        if(coords[1] != sqrt_p-1){
            transform_matrix(A,B,height,width,coords);
        }else{
            transform_matrix(A,B,height,width-1,coords);
        }

    }

    //////////////// VERIFICATION ////////////////
    
    local_min = A[1+0][0];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            local_sum += fabs( A[1+i][j]) ;
            local_min = A[1+i][j] < local_min ? A[1+i][j] : local_min;
        }
    }
    
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, matrix_comm);
    MPI_Reduce(&local_min, &global_min, 1, MPI_DOUBLE, MPI_MIN, 0, matrix_comm);

    ////////////// FINALIZE ////////////////
    if(rank == 0){
        endtime = MPI_Wtime();
        FILE *f = fopen("Results.txt", "a");
        if(f != NULL)
        {
            fprintf(f, "%s\n", gettime(buffer));
            fprintf(f,"%dx%d matrix\n",n,n);
            fprintf(f,"%d processors\n",p);
            fprintf(f,"Sum : %f\n", global_sum );
            fprintf(f,"Min : %f\n", global_min );
            fprintf(f,"Time : %f seconds\n", endtime-starttime);
            fprintf(f,"\n=====================\n");
        }
    }

    delete_matrix(A);
    delete_matrix(B);

    MPI_Finalize();

    return 0;

}

double** new_matrix(int height, int width)
{
    double** matrix;
    matrix = new double*[height];
    matrix[0] = new double[height * width];
    for (int i = 1; i < height; i++)
        matrix[i] = matrix[i-1] + width;
    return matrix;
}

void delete_matrix(double** matrix)
{
    delete[] matrix[0];
    delete[] matrix;
}


void initialize_matrix(double** A, int size, int height, int width, int* coords)
{
    int Origin_x = coords[0]*size;
    int Origin_y = coords[1]*size;
    for (int i_G = Origin_x, i = 0 ; i_G < Origin_x + height ; i_G++, i++)
    {
        for (int j_G = Origin_y, j = 0 ; j_G < Origin_y + width ; j_G++, j++)
        {
            if(coords[0] == coords[1] && i == j){
                A[1+i][j] = i_G*sin(sqrt(i_G));
            }else{
                A[1+i][j] = pow(i_G+j_G,1.1);
            }
        }
    }

    return;
}

void transform_matrix(double** A,double** B, int height, int width, int* coords)
{
    double x;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if( ! (coords[0] == coords[1] && i == j) )
            {
                x = 0.0;
                for(int k = 1 ; k <= 10 ; k++){
                    x += pow( fabs( 0.5 + A[1+i+1][j] ) , 1.0/double(k));
                    x -= pow( fabs( A[1+i-1][j] ) , 1.0/double(k+1)) * pow( fabs( A[1+i][j+1] ) , 1.0/double(k+2));
                }
                x = x < 10.0 ? x : 10.0;
                x = x > -10.0 ? x : -10.0;
                B[1+i][j] = x;
            }else{
                B[1+i][j] = A[1+i][j];
            }
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            A[1+i][j] = B[1+i][j];
        }
    }
    return;
}

void print_matrix(double** A, int height, int width)
{
    for (int i = 0; i < height+2; i++)
    {
        for (int j = 0; j < width+1; j++)
        {
            printf("%f ", A[i][j]);
        }
        printf("\n");
    }
}

char* gettime(char* buffer)
{
    time_t rawtime;
    struct tm * timeinfo;
    
    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,80,"%Y-%m-%d %I:%M:%S",timeinfo);
    std::string str(buffer);

    return buffer;
}

