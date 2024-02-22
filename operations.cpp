#include <cstdlib>
#include <random>
#include <algorithm>
#include <time.h>
#include "Environment.h"
#include "utils.cpp"
#include "seed.cpp"

using namespace std;


static void population_initialization()
{
	swarm_optimum = MINIMUM;
	global_minimum = MINIMUM;
	global_flag = false;
	
	//initialization of group_size (will dinamically change)
	group_size = possible_pop_sizes[rand() % dim_sizes];
	k = DIM_SIZE / group_size;

	//initialization of swarm individuals' positions
	for (int i = 0; i < POP_SIZE; i++)
		for (int j = 0; j < DIM_SIZE; j++)
		{
			population[i][j] = float(low_bound + ((rand() % 100000) / 100000.0) * (high_bound - low_bound));
			it_population[i][j] = 0;
		}
			//5 decimals precision
	//create copies of population
	for (int i = 0; i < POP_SIZE; i++)
		for (int j = 0; j < DIM_SIZE; j++)
		{
			y_population[i][j] = population[i][j]; //for personal bests
			local_population[i][j] = population[i][j]; //for local bests
		}
	for (int j = 0; j < DIM_SIZE; j++)
	{
		swarm[j] = population[0][j];
		global_swarm[j] = population[0][j]; //best for each dimension is needed
		dim_index[j] = j; //permutations will follow
	}

	//create initial fitness statistics
	for (int i = 0; i < POP_SIZE; i++)
		for (int j = 0; j < NO_GROUPS; j++)
		{
			fitness[i][j] = MINIMUM; //subpopulation (made of dimensions' group) fitness
		}
	for (int i = 0; i < POP_SIZE; i++)
		for (int j = 0; j < NO_GROUPS; j++)
		{
			y_fitness[i][j] = fitness[i][j]; //dimension group's memory
		}
}


static void b_x(int j, int i, float particles[DIM_SIZE + 1]) //curent personal value
{
	for (int lj = 0; lj < DIM_SIZE; lj++)
		particles[lj] = swarm[lj];
	for (int d = j * group_size; d < (j + 1) * group_size; d++)
		particles[dim_index[d]] = population[i][dim_index[d]];
}


static void b_y(int j, int i, float particles[DIM_SIZE + 1]) //best personal value
{
	for (int lj = 0; lj < DIM_SIZE; lj++)
		particles[lj] = swarm[lj];
	for (int d = j * group_size; d < (j + 1) * group_size; d++)
		particles[dim_index[d]] = y_population[i][dim_index[d]];
}


static int eval_best(int j, int i) //lbest ring topology
{
	float vi = y_fitness[i][j];
	float vim1 = MINIMUM;
	float vip1 = MINIMUM;
	if (i != 0)
		vim1 = y_fitness[i - 1][j];
	if (i == 0)
		vim1 = y_fitness[POP_SIZE - 1][j];
	if (i != POP_SIZE - 1)
		vip1 = y_fitness[i + 1][j];
	if (i == POP_SIZE - 1)
		vip1 = y_fitness[0][j];

	if (vi < vip1 && vi < vim1)
		return i;
	else if (vip1 < vim1)
	{
		if (i + 1 >= POP_SIZE)
			return 0;
		return i + 1;
	}
	else
		return i - 1;
}


static void shuffle_swarm()
{
	/*for (int i = 0; i < DIM_SIZE; i++)
	{
		int j = i + int(rand() / (RAND_MAX / (DIM_SIZE - i) + 1));
		int aux = dim_index[i];
		dim_index[j] = dim_index[i];
		dim_index[i] = aux;
	}*/

	std::random_shuffle(dim_index, dim_index + DIM_SIZE);
}


static float rand_normal()
{
	//std::default_random_engine generator(seed());
	return std::normal_distribution<float>(0.0, 1.0)(generator);
}


static float rand_cauchy()
{
	//std::default_random_engine generator(seed());
	return std::cauchy_distribution<float>(0.0, 1.0)(generator);
}


static void movement_update(int run)
{
	float particles[DIM_SIZE + 1];
	//what happens per each iteration:
	{
		if (!global_flag)
		{
			group_size = possible_pop_sizes[rand() % dim_sizes];
			k = DIM_SIZE / group_size;
		}

		//random shuffle
		shuffle_swarm();
		//reinitialization
		for (int i = 0; i < POP_SIZE; i++)
			for (int j = 0; j < NO_GROUPS; j++)
			{
				fitness[i][j] = MINIMUM;
				y_fitness[i][j] = fitness[i][j];
			}
		global_flag = false;


		for (int j = 0; j < k; j++)
		{
			swarm_optimum = MINIMUM;
			for (int i = 0; i < POP_SIZE; i++)
			{
				b_x(j, i, particles);
				fitness[i][j] = apply_function(particles); //for j-th group the i-th particle was selected
				b_y(j, i, particles);
				y_fitness[i][j] = apply_function(particles);
				//no_fit_eval += 2;
				util_it += 2;

				if (fitness[i][j] < y_fitness[i][j])
				{
					//replace personal bests
					for (int d = j * group_size; d < (j + 1) * group_size; d++)
						y_population[i][dim_index[d]] = population[i][dim_index[d]];
					y_fitness[i][j] = fitness[i][j];
				}

				if (fitness[i][j] < swarm_optimum)
				{
					//replace swarm best
					for (int d = j * group_size; d < (j + 1) * group_size; d++)
						swarm[dim_index[d]] = y_population[i][dim_index[d]];
					swarm_optimum = y_fitness[i][j];
				}
			}

			for (int i = 0; i < POP_SIZE; i++)
			{
				int local = eval_best(j, i);
				for (int d = j * group_size; d < (j + 1) * group_size; d++)
					local_population[i][dim_index[d]] = y_population[local][dim_index[d]];
			}

			if (swarm_optimum < global_minimum)
			{
				for (int d = j * group_size; d < (j + 1) * group_size; d++)
					global_swarm[dim_index[d]] = swarm[dim_index[d]];
				global_minimum = swarm_optimum;
				global_flag = true;
			}
		}

		float avg = compute_avg_fitness();
		for (int j = 0; j < k; j++)
			for (int i = 0; i < POP_SIZE; i++)
				for (int d = j * group_size; d < (j + 1) * group_size; d++)
				{
					int dup = dim_index[d];
					int prob = rand() % 100000;
					if (prob < probability * 100000)
					{
						if (fitness[i][j] >= avg + 0.1 * avg)
						{
							population[i][dup] = y_population[i][dup] + rand_cauchy() * abs(y_population[i][dup] - local_population[i][dup]);
							while (population[i][dup] < low_bound || population[i][dup] > high_bound)
							{
								population[i][dup] = y_population[i][dup] + rand_cauchy() * abs(y_population[i][dup] - local_population[i][dup]);
							}
							it_population[i][dup] = 0;
						}
						else
							if (fitness[i][j] <= avg - 0.1 * avg)
							{
								population[i][dup] = local_population[i][dup] + rand_cauchy() * abs(y_population[i][dup] - swarm[dup]);
								while (population[i][dup] < low_bound || population[i][dup] > high_bound)
								{
									population[i][dup] = local_population[i][dup] + rand_cauchy() * abs(y_population[i][dup] - swarm[dup]);
								}
								it_population[i][dup]++;
							}
							else
							{
								population[i][dup] = y_population[i][dup] + rand_cauchy() * abs(y_population[i][dup] - swarm[dup])
									+ rand_cauchy() * abs(y_population[i][dup] - local_population[i][dup]);
								while (population[i][dup] < low_bound || population[i][dup] > high_bound)
								{
									population[i][dup] = y_population[i][dup] + rand_cauchy() * abs(y_population[i][dup] - swarm[dup])
										+ rand_cauchy() * abs(y_population[i][dup] - local_population[i][dup]);
								}
								it_population[i][dup] = 0;
							}
					}
					else
					{
						if (fitness[i][j] >= avg + 0.1 * avg)
						{
							population[i][dup] = y_population[i][dup] + rand_normal() * abs(y_population[i][dup] - local_population[i][dup]);
							while (population[i][dup] < low_bound || population[i][dup] > high_bound)
							{
								population[i][dup] = y_population[i][dup] + rand_normal() * abs(y_population[i][dup] - local_population[i][dup]);
							}
							it_population[i][dup] = 0;
						}
						else
							if (fitness[i][j] <= avg - 0.1 * avg)
							{
								population[i][dup] = local_population[i][dup] + rand_normal() * abs(local_population[i][dup] - swarm[dup]);
								while (population[i][dup] < low_bound || population[i][dup] > high_bound)
								{
									population[i][dup] = local_population[i][dup] + rand_normal() * abs(local_population[i][dup] - swarm[dup]);
								}
								it_population[i][dup]++;
							}
							else
							{
								population[i][dup] = local_population[i][dup] + rand_normal() * abs(local_population[i][dup] - swarm[dup])
									+ rand_normal() * abs(y_population[i][dup] - local_population[i][dup]);
								while (population[i][dup] < low_bound || population[i][dup] > high_bound)
								{
									population[i][dup] = local_population[i][dup] + rand_normal() * abs(local_population[i][dup] - swarm[dup])
										+ rand_normal() * abs(y_population[i][dup] - local_population[i][dup]);
								}
								it_population[i][dup] = 0;
							}
					}

					if (it_population[i][dup] > ((no_fit_eval_max - run) * 1.0 / no_fit_eval_max) * 50)
					{
						//population[i][dup] = mutate(global_swarm[dup]);
						population[i][dup] = (swarm[dup]);
						it_population[i][dup] = 0;
					}

					/*if (population[i][dup] < low_bound)
						population[i][dup] = low_bound;
					if (population[i][dup] > high_bound)
						population[i][dup] = high_bound;*/
				}
		//global_swarm_history[run] = global_minimum;

		/*int smin = 2;
		int kmax = DIM_SIZE / smin;
		if (no_fit_eval + kmax * POP_SIZE * 2 > no_fit_eval_max)
			break;*/
	}
}
