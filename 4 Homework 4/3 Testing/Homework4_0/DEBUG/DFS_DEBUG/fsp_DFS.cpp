#include <stdio.h>
#include <stdlib.h>
// #include <iostream>
#include <cmath>
#include <limits>
#include <vector>
#include <stack>
#include <queue>
#include <omp.h>
#include <ctime>
#include <string>

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
void fprint_path(FILE *f,Path path);

char* gettime(char *buffer);

int main(int argc, char const *argv[])
{
    Path best_path;
    best_path.cost = std::numeric_limits<float>::infinity();
    std::priority_queue<Path,std::vector<Path>,ComparePaths> initial_unexplored_paths;
    std::stack<Path> global_unexplored_paths;
    double begin, end;
    Point coordinates[C];
    double distances[C][C];
    long busy_workers;
    int num_threads;

    int* best_changes; //DEBUG
    int* paths_explored; //DEBUG
    int* pushes; //DEBUG

    #pragma omp parallel shared(best_path, global_unexplored_paths,coordinates,distances,busy_workers,num_threads)
    {
        std::stack<Path> unexplored_paths;
        Path current_path, new_path;

        #pragma omp master
        {
            begin = omp_get_wtime();
            num_threads = omp_get_num_threads();
            busy_workers = ( 1 << num_threads ) -1;
            best_changes = new int[num_threads]; //DEBUG
            paths_explored = new int[num_threads]; //DEBUG
            pushes = new int[num_threads]; //DEBUG
            for(int i = 0; i < num_threads; i++)//DEBUG
            {//DEBUG
                best_changes[i] = paths_explored[i] = pushes[i] = 0;//DEBUG
            }//DEBUG
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
            
            initial_unexplored_paths.push(origin);
            
            bool updated_best = false;
        // master_BFS_search
            while( !updated_best )
            {
                current_path = initial_unexplored_paths.top();
                initial_unexplored_paths.pop();
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
                                initial_unexplored_paths.push(new_path);
                            }
                            else
                            {
                                best_path = new_path;
                                updated_best = true;
                            }
                        }
                    }
                }
            }
            while(!initial_unexplored_paths.empty())
            {
                global_unexplored_paths.push(initial_unexplored_paths.top());
                initial_unexplored_paths.pop();
            }
        }

        #pragma omp barrier


        int thread_num = omp_get_thread_num();
        long busy_mask = 0x1 << thread_num;
        long idle_mask = busy_workers ^ busy_mask;
        long all_working = ( 1 << num_threads ) -1;


    // worker_DFS_Search
        while(busy_workers > 0 || !global_unexplored_paths.empty())
        {
            if(!global_unexplored_paths.empty())
            {
                #pragma omp critical(global_unexplored_paths)
                {
                    if(!global_unexplored_paths.empty())
                    {
                        unexplored_paths.push(global_unexplored_paths.top());
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
                            paths_explored[thread_num]++;//DEBUG
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
                                        pushes[thread_num]++; //DEBUG
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
                                           best_changes[thread_num]++;//DEBUG
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
    
    // master output
        #pragma omp master
        {
            end = omp_get_wtime();
            char buffer[80];

            FILE *f = fopen("Results.txt", "a");
            if(f != NULL)
            {
                fprintf(f, "%s\n", gettime(buffer));
                fprintf(f,"%d classrooms\n",C);
                fprintf(f,"%d threads\n",num_threads);
                fprint_path(f,best_path);
                fprintf(f,"Proc Changes      Paths    Pushes\n");//DEBUG
                for(int i = 0; i < num_threads; i++)//DEBUG
                {//DEBUG
                    fprintf(f,"  %02d     %03d   %8d    %03d\n",i,best_changes[i],paths_explored[i],pushes[i]);//DEBUG
                }//DEBUG
                fprintf(f,"Time : %f seconds\n", end-begin);
                fprintf(f,"\n=====================\n");
            }
        }
    }
    delete[] best_changes; //DEBUG
    delete[] paths_explored; //DEBUG
    delete[] pushes; //DEBUG
    // print_matrix(distances);
    return 0;

}

void initialize_coordinates(Point coordinates[C])
{
    #pragma omp for
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
    #pragma omp for
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

void fprint_path(FILE *f,Path path)
{
    for(int i = 0 ; i < path.order.size() ; i++)
    {
        fprintf(f,"%d, ", path.order[i]);
    }
    fprintf(f," : %lf\n",path.cost);
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

