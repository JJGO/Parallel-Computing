
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
    // std::queue<Path> global_unexplored_paths;
    std::priority_queue<Path,std::vector<Path>,ComparePaths> global_unexplored_paths;
    double begin, end;
    Point coordinates[C];
    double distances[C][C];
    long busy_workers;
    long all_working;
    int num_threads;

    #pragma omp parallel shared(best_path, global_unexplored_paths,coordinates,distances,busy_workers,all_working ,num_threads)
    {
        std::stack<Path> unexplored_paths;
        Path current_path, new_path;

        #pragma omp master
        {
            num_threads = omp_get_num_threads();
            printf("This program is running in %d threads\n", num_threads);
            begin = omp_get_wtime();
            all_working = ( 1 << num_threads ) -1;
            busy_workers = all_working;
        }
        
    // INIT VARS
        
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
            
            bool updated_best = false;
        // master_BFS_search
            while(!updated_best)//( global_unexplored_paths.size() < num_threads )
            {
                current_path = global_unexplored_paths.top();//front();
                global_unexplored_paths.pop();
                for(int node = 1 ; node < C ; node++)
                {
                    new_path = current_path;
                    if(!new_path.visited[node])
                    {
                        new_path.cost += distances[new_path.order.back()][node];
                        new_path.visited[node] = true;
                        new_path.order.push_back(node);
                        // print_path(new_path); //DEBUG
                        if(new_path.cost < best_path.cost)
                        {
                            if(new_path.order.size() < C)
                            {
                                global_unexplored_paths.push(new_path);
                            }
                            else
                            {
                                best_path = new_path;
                                updated_best = true;
                                // printf("H ");
                                // print_path(new_path); //DEBUG
                            }
                        }
                    }
                }
            }
        }

        #pragma omp barrier


        long busy_mask = 0x1 << omp_get_thread_num();
        long idle_mask = busy_workers ^ busy_mask;

        // int best_changes = 0, paths_explored = 0, pushes = 0; //DEBUG

    // worker_DFS_Search
        while(busy_workers > 0 || !global_unexplored_paths.empty())
        {
            if(!global_unexplored_paths.empty())
            {
                #pragma omp critical(global_unexplored_paths)
                {
                    if(!global_unexplored_paths.empty())
                    {
                        unexplored_paths.push(global_unexplored_paths.top());//front());
                        global_unexplored_paths.pop();
                        busy_workers |= busy_mask;
                    }
                }
            }
            
            while(!unexplored_paths.empty())
            {
                current_path = unexplored_paths.top();
                unexplored_paths.pop();
                if(current_path.cost < best_path.cost)
                {
                    for(int node = 1 ; node < C ; node++)
                    {
                        new_path = current_path;
                        if(!new_path.visited[node])
                        {
                            // paths_explored++;//DEBUG
                            new_path.cost += distances[new_path.order.back()][node];
                            new_path.visited[node] = true;
                            new_path.order.push_back(node);
                            if(new_path.cost < best_path.cost)
                            {
                                if(new_path.order.size() < C)
                                {
                                    if( busy_workers < all_working )
                                    {
                                        #pragma omp critical(global_unexplored_paths)
                                        {
                                            global_unexplored_paths.push(new_path);
                                        }
                                        // pushes++; //DEBUG
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
                                           // best_changes++;//DEBUG
                                           // printf("%d %d : ",omp_get_thread_num(),paths_explored ); //DEBUG
                                           // print_path(new_path); //DEBUG
                                        }
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
    
        // #pragma omp master//DEBUG
        // {//DEBUG
            // printf("Proc Changes      Paths    Pushes\n");//DEBUG
        // }//DEBUG
        // #pragma omp barrier//DEBUG
        // #pragma omp critical(print)//DEBUG
        // {//DEBUG
            // printf("   %d     %03d   %8d    %03d\n",omp_get_thread_num(),best_changes,paths_explored,pushes);//DEBUG
        // }//DEBUG
        // #pragma omp barrier//DEBUG
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
    printf("%s", global_unexplored_paths.empty() ? "true" : "false");
        
    return 0;

}

void initialize_coordinates(Point coordinates[C])
{
    #pragma omp parallel for
    for (int c = 0; c < C; c++)
    {
        coordinates[c].x = 100.0*sin(c);
        coordinates[c].y = 101.0*cos(c*c);
        // coordinates[c].x = 1.1*(c*c % 17);
        // coordinates[c].y = 0.5*(c*c*c % 23);
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

// PRINT FUNCTIONS

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

