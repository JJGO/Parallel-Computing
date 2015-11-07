
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

#define C 15

struct Point {
    double x;
    double y;
};

struct Path
{
    std::vector<int> order;
    bool visited[C];
    double cost;
} ;

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
    long busy_workers;

    #pragma omp parallel shared(best_path, global_unexplored_paths,coordinates,distances,busy_workers)
    {
        std::priority_queue<Path,std::vector<Path>,ComparePaths> unexplored_paths;
        Path current_path, new_path;

        long idle_mask = 1 << omp_get_thread_num();
        long busy_mask = busy_workers ^ idle_mask;

        #pragma omp master
        {
            printf("This program is running in %d threads\n", omp_get_num_threads());
            begin = omp_get_wtime();
            busy_workers = ( 1 << omp_get_num_threads() ) -1;
        }
        // #pragma omp critical(print)
        // {
        //     printf("I am proc%d and my masks: %x %x %x \n",busy_mask );
        // }
        
        
        initialize_coordinates(coordinates);
        calculate_distances(coordinates,distances);

        #pragma omp master
        {
            Path origin;
            origin.cost = 0.0;
            origin.order.push_back(0);
            for(int i = 0; i < C; i++)
                origin.visited[i] = false;
            
            global_unexplored_paths.push(origin);
           
            ////////////
            
        // master_BFS_search();
            while( global_unexplored_paths.size() < omp_get_num_threads() )
            {
                current_path = global_unexplored_paths.front();
                global_unexplored_paths.pop();
                for(int node = 1 ; node < C ; node++)
                {
                    new_path = current_path;
                    if(!new_path.visited[node])
                    {
                        new_path.cost += distances[new_path.order.back()][node];
                        new_path.visited[node] = true;
                        new_path.order.push_back(node);
                        if(new_path.cost < best_path.cost)
                        {
                            if(new_path.order.size() < C)
                            {
                                global_unexplored_paths.push(new_path);
                            }
                            else
                            {
                                best_path = new_path;
                            }
                        }
                        // print_path(new_path);
                    }
                }
            }
        }

        #pragma omp barrier

    // worker_DFS_Search();
        while(busy_workers > 0)
        {
            if(!global_unexplored_paths.empty())
            {
                #pragma omp critical(global_unexplored_paths)
                {
                    if(!global_unexplored_paths.empty())
                    {
                        unexplored_paths.push(global_unexplored_paths.front());
                        global_unexplored_paths.pop();
                        busy_workers |= busy_mask;
                    }
                }
            }
            
            while(!unexplored_paths.empty())
            {
                current_path = unexplored_paths.top();
                unexplored_paths.pop();
                for(int node = 1 ; node < C ; node++)
                {
                    new_path = current_path;
                    if(!new_path.visited[node])
                    {
                        new_path.cost += distances[new_path.order.back()][node];
                        new_path.visited[node] = true;
                        new_path.order.push_back(node);

                        // #pragma omp critical(print)
                        // {
                        //     print_path(new_path);
                        //     printf("%x\n",busy_workers);
                        // }
                        
                        if(new_path.cost < best_path.cost)
                        {
                            if(new_path.order.size() < C)
                            {
                                if( global_unexplored_paths.empty() )
                                {
                                    #pragma omp critical(global_unexplored_paths)
                                    {
                                        global_unexplored_paths.push(new_path);
                                    }
                                }
                                else
                                    unexplored_paths.push(new_path);
                            }
                            else
                            {
                                #pragma omp critical(best_path)
                                {
                                    if(new_path.cost < best_path.cost)
                                    {
                                       best_path = new_path;
                                       // print_path(new_path);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            #pragma omp atomic
            busy_workers &= idle_mask;
        }
    // master output
        #pragma omp master
        {
            end = omp_get_wtime();
            print_path(best_path);
            double check_cost = 0.0;
            for(int i = 0 ; i < C-1 ; i++)
            {
                check_cost += distances[best_path.order[i]][best_path.order[i+1]];
            }
            printf("%lf\n",check_cost );
            printf("\nTIME : %lf seconds\n", end - begin);
        }
    }
    
    // print_matrix(distances);
    double sum = 0.0;
    for (int i = 0; i < C; i++)
    {
        for (int j = 0; j < C; j++)
        {
            sum += distances[i][j];
        }
    }
    printf("%lf\n", sum);
        
    return 0;

}

void initialize_coordinates(Point coordinates[C])
{
    #pragma omp parallel for
    for (int c = 0; c < C; c++)
    {
        coordinates[c].x = 100.0*sin(c);
        coordinates[c].y = 101.0*cos(c*c);
    }
}

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

// DEBUG FUNCTIONS

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

