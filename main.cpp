#pragma warning(disable:4996)
#include <time.h>
#include <stdlib.h>
#include <random>
#include "environment.h"
#include "operations.cpp"

using namespace std;

int main()
{
	//srand(time(NULL));
	srand(seed());
	float avg = 0;
	float minimum_of_all = MINIMUM;
	float maximum_of_all = -MINIMUM;
	float results[31];
	float it_avg = 0.0;
	float time_avg = 0.0;
	float util_it_avg = 0.0;
	time_t start, end;
	int run;
	//FILE* f = fopen("acc5.txt", "w");
	for (int it = 0; it < no_runs; it++)
	{
		util_it = 0;
		population_initialization();
		for (run = 0; run < no_fit_eval_max && util_it < DIM_SIZE * POP_SIZE * 2 * 1000; run++)
		{
			time(&start);
			movement_update(run);
			//fprintf(f, "Iteration: %d, Evaluation: %d, Minimum: %f\n", run, util_it, global_minimum);
			//if (((int)(global_minimum * 100000.0)) / 100000.0 - float(-99.56129) <= pow(10, -7))
			if(global_minimum - 0.0 <= pow(10, -7))
			{
				it_avg += run;
				time(&end);
				time_avg += float(end - start);
				break;
			}
			time(&end);
			time_avg += float(end - start);
		}
		if (run >= no_fit_eval_max)
			it_avg += no_fit_eval_max;
		avg += global_minimum;
		if (global_minimum < minimum_of_all)
			minimum_of_all = global_minimum;
		if (global_minimum > maximum_of_all)
			maximum_of_all = global_minimum;
		results[it] = global_minimum;
		util_it_avg += 1.0 * util_it;
	}
	//fclose(f);
	avg /= no_runs;
	it_avg /= no_runs;
	time_avg /= no_runs;
	util_it_avg /= no_runs;
	float std = 0.0;
	for (int iteration = 0; iteration < no_runs; iteration++)
		std += pow(results[iteration] - avg, 2);
	std /= (no_runs - 1);

	printf("Average: %f\n", avg);
	printf("Standard deviation: %f\n", std);
	printf("Minimum: %f\n", minimum_of_all);
	printf("Maximum: %f\n", maximum_of_all);

	printf("Number of fitness evaluations: %d\n", util_it);
	printf("Number of iterations: %f\n", util_it_avg * 1.0 / (DIM_SIZE * POP_SIZE * 2));
	printf("Elased time: %f\n", time_avg);
	return 0;
}