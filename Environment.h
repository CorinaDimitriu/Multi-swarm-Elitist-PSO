#pragma once
#define MINIMUM 200000000000000000000000000000000000000.0
#define POP_SIZE 100 // a maximum value
#define DIM_SIZE 30
#define NO_GROUPS 50 // a maximum value

static int group_size;
static int possible_pop_sizes[] = { 5, 6, 10, 15, 30 }; //{2, 5, 10, 50, 100};
static int dim_sizes = 5;
//static int possible_pop_sizes[] = { 2 }; //{2, 5, 10, 50, 100};
//static int dim_sizes = 1;
//static int possible_pop_sizes[] = { 5, 10, 20, 25, 50, 100 }; //{2, 5, 10, 50, 100};
//static int dim_sizes = 6;

static int k;
static bool global_flag = false;
static int dim_index[DIM_SIZE];

static float population[POP_SIZE + 1][DIM_SIZE + 1];
static float it_population[POP_SIZE + 1][DIM_SIZE + 1];
static float y_population[POP_SIZE + 1][DIM_SIZE + 1];
static float local_population[POP_SIZE + 1][DIM_SIZE + 1];
static float swarm[DIM_SIZE + 1];
static float global_swarm[DIM_SIZE + 1];
//static float global_swarm_history[1000];
static float fitness[POP_SIZE + 1][NO_GROUPS];
static float y_fitness[POP_SIZE + 1][NO_GROUPS];
static float global_minimum;
static float swarm_optimum;

//static int no_fit_eval = 0;
static int no_fit_eval_max = 30000;
static int no_runs = 30; //1
static int util_it = 0;

//misc
static float probability = 0.5;
static float low_bound = -2048.0; //will be changed depending on the function
static float high_bound = 2048.0; //will be changed depending on the function
//static float low_bound = -5.12; //will be changed depending on the function
//static float high_bound = 5.12; //will be changed depending on the function
//static float low_bound = 0.0; //will be changed depending on the function
//static float high_bound = 3.14159; //will be changed depending on the function
//static float low_bound = -600.0; //will be changed depending on the function
//static float high_bound = 600.0; //will be changed depending on the function


//Source of inspiration when got stuck in understanding the papers:
/*https://github.com/WenjieOoo/CCPSO2*/
