
#include <stdio.h>
#include <stdlib.h>
// #include <iostream>
#include <time.h>
#include <cmath>
#include <limits>
#include <vector>
#include <stack>
#include <queue>
#include <omp.h>

#define C 11

// using namespace std;

struct Point {
    double x;
    double y;
};
// typedef struct Point Point;

struct Path
{
    std::vector<int> order;
    bool visited[C];
    double cost;
} ;

// struct ComparePaths
// {
//     bool operator()(const Path &l, const Path &r) {return l.cost/l.order.size() > r.cost/r.order.size();}
// };

struct ComparePaths // LessThan
{
    bool operator()(const Path &l, const Path &r) {
        if(l.order.size() == r.order.size())
            return l.cost > r.cost;
        else
            return l.order.size() < r.order.size();
    }
};

void initialize_coordinates(Point coordinates[C]);
void calculate_distances(Point coordinates[C], double distances[C][C]);
void print_matrix(double A[C][C]);
void print_path(Path path);


int main(int argc, char const *argv[])
{
    Path best_path;
    best_path.cost = std::numeric_limits<float>::infinity();
    std::queue<Path> global_unexplored_paths;
    double begin, end;
    Point coordinates[C];
    double distances[C][C];

    initialize_coordinates(coordinates);
    calculate_distances(coordinates,distances);

    #pragma omp parallel shared(best_path, global_unexplored_paths,coordinates,distances)
    {
        #pragma omp master
        {
            begin = omp_get_wtime();
        }

        Path current_path, new_path;

        

    //     #pragma omp master
    //     {
    //         Path origin;
    //         origin.cost = 0.0;
    //         origin.order.push_back(0);
    //         for(int i = 0; i < C; i++)
    //         {
    //             origin.visited[i] = false;
    //         }
    //         #pragma omp critical(unexplored_paths)
    //         {
    //             global_unexplored_paths.push(origin);
    //         }
           
    //         ////////////
            
    //         while( global_unexplored_paths.size() < omp_get_num_threads() )
    //         {
    //             #pragma omp critical(unexplored_update)
    //             {
    //                 current_path = global_unexplored_paths.front();
    //                 global_unexplored_paths.pop();
    //             }
    //             for(int node = 1 ; node < C ; node++)
    //             {
    //                 new_path = current_path;
    //                 if(!new_path.visited[node])
    //                 {
    //                     new_path.cost += distances[new_path.order.back()][node];
    //                     new_path.visited[node] = true;
    //                     new_path.order.push_back(node);
    //                     #pragma omp critical(unexplored_update)
    //                     {
    //                         if(new_path.cost < best_path.cost)
    //                         {
    //                             if(new_path.order.size() < C)
    //                             {
    //                                 global_unexplored_paths.push(new_path);
    //                             }
    //                             else
    //                             {
    //                                 best_path = new_path;
    //                             }
    //                         }
    //                     }
    //                     // print_path(new_path);
    //                 }
    //             }
    //         }
    //     }

    //     #pragma omp barrier

    //     #pragma omp master
    //     {

    //         while(!global_unexplored_paths.empty())
    //         {
    //             int i = 0;
    //         }

    //         ////////////

    //         end = omp_get_wtime();
    //         print_path(best_path);
    //         double check_cost = 0.0;
    //         for(int i = 0 ; i < C-1 ; i++)
    //         {
    //             check_cost += distances[best_path.order[i]][best_path.order[i+1]];
    //         }
    //         printf("%lf\n",check_cost );
    //         printf("\nTIME : %lf seconds\n", end - begin);
    //     }

    //     std::stack<Path> unexplored_paths;
    //     // std::priority_queue<Path,std::vector<Path>,ComparePaths> unexplored_paths;


    //     while(!global_unexplored_paths.empty())
    //     {
    //         #pragma omp critical(unexplored_update)
    //         {
    //            unexplored_paths.push(global_unexplored_paths.front());
    //            global_unexplored_paths.pop();
    //         }
    //         while(!unexplored_paths.empty())
    //         {
    //             current_path = unexplored_paths.top();
    //             unexplored_paths.pop();
    //             for(int node = 1 ; node < C ; node++)
    //             {
    //                 new_path = current_path;
    //                 if(!new_path.visited[node])
    //                 {
    //                     new_path.cost += distances[new_path.order.back()][node];
    //                     new_path.visited[node] = true;
    //                     new_path.order.push_back(node);
    //                     #pragma omp critical(unexplored_update)
    //                     {
    //                         if(new_path.cost < best_path.cost)
    //                         {
    //                             if(new_path.order.size() < C)
    //                             {
    //                                 if( global_unexplored_paths.empty() )
    //                                     global_unexplored_paths.push(new_path);
    //                                 else
    //                                     unexplored_paths.push(new_path);
    //                             }
    //                             else
    //                             {
    //                                 best_path = new_path;
    //                             }
    //                         }
    //                     }
    //                     // print_path(new_path);
    //                 }
    //             }
    //         }
    //     }

        #pragma omp master
        {
            end = omp_get_wtime();
            print_path(best_path);
            // double check_cost = 0.0;
            // for(int i = 0 ; i < C-1 ; i++)
            // {
            //     check_cost += distances[best_path.order[i]][best_path.order[i+1]];
            // }
            // printf("%lf\n",check_cost );
            printf("\nTIME : %lf seconds\n", end - begin);
        }
    }
    
    // print_matrix(distances);
    
    return 0;

}


void initialize_coordinates(Point coordinates[C])
{
    #pragma omp parallel for
    for (int c = 0; c < C; c++)
    {
        coordinates[c].x = 1.1*(c*c % 17);
        coordinates[c].y = 0.5*(c*c*c % 23);
    }
}

// void calculate_distances(Point coordinates[C], double distances[C][C])
// {
//     for( int i = 0; i < C; i++)
//     {
//         double dist = 0.0;
//         double iX = coordinates[i].x;
//         double iY = coordinates[i].y;
//         distances[i][i] = 0.0;
//         for(int j = 0; j < i; j++)
//         {
//             double deltaX = pow(fabs(iX-coordinates[j].x),1.5);
//             double deltaY = pow(fabs(iY-coordinates[j].y),1.5);
//             dist = pow(deltaX+deltaY,2.0/3.0);
//             distances[i][j] = dist;
//             distances[j][i] = dist;
//         }
//     }
// }

void calculate_distances(Point coordinates[C], double distances[C][C])
{
    #pragma omp parallel for
    for( int z = 0; z < (C*(C+1))/2; z++)
    {
        double dist, deltaX, deltaY;
        int i = z / C;
        int j = z % C;
        if(i == j)
        {
            distances[i][i] = 0;
        }
        else
        {
            if(i > j)
            {
                i = C-i;
                j = C-1-j;
            }
            deltaX = pow(fabs(coordinates[i].x-coordinates[j].x),1.5);
            deltaY = pow(fabs(coordinates[i].y-coordinates[j].y),1.5);
            dist = pow(deltaX+deltaY,2.0/3.0);
            distances[i][j] = dist;
            distances[j][i] = dist;
        }
        
    }
}

void print_matrix(double A[C][C])
{
    for (int i = 0; i < C; i++)
    {
        for (int j = 0; j < C; j++)
        {
            printf("%lf ", A[i][j]);
        }
        printf("\n");
    }
}

void print_path(Path path)
{
    for(int i = 0 ; i < path.order.size() ; i++)
    {
        printf("%d, ", path.order[i]);
    }
    printf("\b\b : %lf\n",path.cost);
}

