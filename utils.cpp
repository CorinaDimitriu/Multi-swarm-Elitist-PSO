#include "environment.h"
#include <algorithm>
#include <math.h>

static float apply_function(float individual[DIM_SIZE + 1])
{
	//De Jong's 1
	/*float sum = 0.0;

	for (int dim = 0; dim < DIM_SIZE; dim++)
	{
		sum += pow(individual[dim], 2);
	}

	return sum;*/


	//Rastrigin's
	/*float pi = 2 * acos(0.0);
	float result = 10.0 * DIM_SIZE;

	for (int dim = 0; dim < DIM_SIZE; dim++)
	{
		result += pow(individual[dim], 2) - 10 * cos(2 * pi * individual[dim]);
	}

	return result;*/


	//Michalewicz's
	/*float result = 0.0;
	float pi = 2 * acos(0.0);
	for (int dim = 0; dim < DIM_SIZE; dim++)
	{
		result += (-1) * sin(individual[dim]) * pow(sin(((dim + 1) * pow(individual[dim], 2)) / pi), 20);
	}

	return result;*/


	//Rosenbrock's valley (De Jong's 2)
	float sum = 0.0;

	for (int dim = 0; dim < DIM_SIZE - 1; dim++)
	{
		sum += (100 * pow(individual[dim + 1] - pow(individual[dim], 2), 2) + pow(1 - individual[dim], 2));
	}

	return sum;

	//Schwefel's
	/*float result = 0.0;
	for (int dim = 0; dim < DIM_SIZE; dim++)
	{
		result += (-1) * individual[dim] * sin(sqrt(abs(individual[dim])));
	}

	return result;*/

	//Griewangk's
	/*float result = 1.0;
	float prod = 1.0;
	for (int dim = 0; dim < DIM_SIZE; dim++)
	{
		result += pow(individual[dim], 2) / 4000;
	}
	for (int dim = 0; dim < DIM_SIZE; dim++)
		prod *= cos(individual[dim] / sqrt(dim + 1));
	result -= prod;

	return result;*/
}


static float compute_avg_fitness()
{
	float sum = 0.0;
	for (int j = 0; j < k; j++)
		for (int i = 0; i < POP_SIZE; i++)
			sum += fitness[i][j];
	return sum / (k * POP_SIZE);
}


static void convert_to_binary(float number, unsigned short int chromosome[16])
{
	//convert the number into order number
	int integer_part = int(number);
	int first_dec = int((number - integer_part) * 10);
	int second_dec = int((number - integer_part) * 100);
	int third_dec = int((number - integer_part) * 1000);
	int fourth_dec = int((number - integer_part) * 10000);
	int fifth_dec = int((number - integer_part) * 100000);
	int genes = (integer_part + 2048) * pow(10, 5) + first_dec * pow(10, 4) * second_dec * pow(10, 3)
		+ third_dec * pow(10, 2) + fourth_dec * pow(10, 1) + fifth_dec * pow(10, 0);

	//conversion
	for (int j = 29; j >= 0; j--)
		chromosome[j] = 0;
	for (int j = 29; j >= 0; j--)
	{
		float power_2 = pow(2, j);
		if (power_2 > genes)
		{
			chromosome[j] = 1;
			genes -= power_2;
		}
	}
}


static float convert_to_number(unsigned short int chromosome[16])
{
	float number = 0.0;
	for (int j = 29; j >= 0; j--)
		number += chromosome[j] * pow(2, j);
	number *= (4096.0 / (pow(2, 29) - 1));
	number -= 2048.0;
	return number;
}


static void generate_bit_mask(unsigned short int mask[30], float p, int dec) //probability has to be of the form 0.0*
{
	for (int i = 0; i < 30; i++)
	{
		int prob = rand() % dec;
		if (prob < p * dec * 10)
			mask[i] = 1;
		else
			mask[i] = 0;
	}
}


static float mutate(float genes)
{
	unsigned short int chromosome[30];
	convert_to_binary(genes, chromosome);

	//mutation
	unsigned short int mask[30];
	generate_bit_mask(mask, 0.04, 100000);
	bool flag = false;
	for (int bit = 0; bit < 30; bit++)
	{
		if (mask[bit] == 1)
		{
			chromosome[bit] = 1 - chromosome[bit];
		}
	}

	return convert_to_number(chromosome);
}