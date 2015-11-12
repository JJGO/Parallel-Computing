
#include <stdio.h>
#include <stdlib.h>
// #include <iostream>
#include <time.h>
#include <limits>
#include <cmath>
#include <vector>
#include <stack>
#include <queue>

#define C 15

// using namespace std;

struct Point {
    double x;
    double y;
};
// typedef struct Point Point;

struct Path
{
    int order[C];
    int size;
    bool visited[C];
    double cost;
} ;


void initialize_coordinates(Point coordinates[C]);
void calculate_distances(Point coordinates[C], double distances[C][C]);
void print_matrix(double A[C][C]);
void print_path(Path path);

struct ComparePaths // LessThan
{
    bool operator()(const Path &l, const Path &r) {
        if(l.size == r.size)
            return l.cost > r.cost;
        else
            return l.size < r.size;
    }
};

int main(int argc, char const *argv[])
{
    Point coordinates[C];
    double distances[C][C];

    std::stack<Path> unexplored_paths;
    // std::priority_queue<Path,std::vector<Path>,ComparePaths> unexplored_paths;

    Path best_path;
    best_path.cost = std::numeric_limits<float>::infinity();

    clock_t begin, end;
    double time_spent;

    begin = clock();

    Path origin, current_path, new_path;
    origin.cost = 0.0;
    origin.size = 1;
    origin.order[0] = 0;
    for(int i = 0; i < C; i++)
        origin.visited[i] = false;

    
    initialize_coordinates(coordinates);
    calculate_distances(coordinates,distances);

    unexplored_paths.push(origin);

    while(!unexplored_paths.empty())
    {
        current_path = unexplored_paths.top();
        unexplored_paths.pop();
        if(current_path.cost < best_path.cost)
        {
            for(int node = 1 ; node < C ; node++)
            {
                if(!current_path.visited[node])
                {
                    new_path = current_path;
                    new_path.cost += distances[new_path.order[new_path.size-1]][node];
                    if(new_path.cost < best_path.cost)
                    {
                        new_path.visited[node] = true;
                        new_path.order[new_path.size] = node;
                        new_path.size += 1;
                        if(new_path.size < C)
                        {
                            unexplored_paths.push(new_path);
                        }
                        else
                        {
                            best_path = new_path;
                        }
                    }
                }
            }
        }
    }

    // printf("\n SUM : %lf\n", sumA );
    // printf("\n MIN : %lf\n", minA );

    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    print_matrix(distances);
    print_path(best_path);
    double check_cost = 0.0;
    for(int i = 0 ; i < C-1 ; i++)
    {
        check_cost += distances[best_path.order[i]][best_path.order[i+1]];
    }
    printf("%lf\n",check_cost );
    printf("\nTIME : %lf seconds\n", time_spent);

    return 0;

}


void initialize_coordinates(Point coordinates[C])
{
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
    int i,j;
    double dist, deltaX, deltaY, iX, iY;
    for( i = 0; i < C; i++)
    {
        iX = coordinates[i].x;
        iY = coordinates[i].y;
        distances[i][i] = 0.0;
        for(j = 0; j < i; j++)
        {
            deltaX = pow(fabs(iX-coordinates[j].x),1.5);
            deltaY = pow(fabs(iY-coordinates[j].y),1.5);
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
    for(int i = 0 ; i < path.size; i++)
    {
        printf("%d, ", path.order[i]);
    }
    printf("\b\b : %lf\n",path.cost);
}

