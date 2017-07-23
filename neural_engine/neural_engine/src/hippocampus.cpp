#include "hippocampus.h"

using namespace std;

hippocampus::hippocampus()
{
	double** new_pop;
	double*** grid_cell_populations = new double**[GRID_POPULATION_SIZE];
	for (int i = 0; i < GRID_POPULATION_NUMBER; i++) {
		new_pop = create_grid_population();
		grid_cell_populations[i] = new_pop;
	}
}

hippocampus::~hippocampus()
{
}

double** hippocampus::create_grid_population()
{
	double** grid_population = new double*[GRID_POPULATION_SIZE];
	for (int i = 0; i < GRID_POPULATION_SIZE; i++) {
		grid_population[i] = new double[GRID_POPULATION_SIZE];
	}

	return grid_population;
}
