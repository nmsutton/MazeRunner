#include "hippocampus.h"

using namespace std;

hippocampus::hippocampus()
{
	grid_cells** new_pop;
	grid_cells*** grid_cell_populations = new grid_cells**[GRID_POPULATION_SIZE];
	for (int i = 0; i < GRID_POPULATION_NUMBER; i++) {
		new_pop = create_grid_population();
		grid_cell_populations[i] = new_pop;
	}
}

hippocampus::~hippocampus()
{
}

hippocampus::grid_cells** hippocampus::hippocampus::create_grid_population()
{
	grid_cells** grid_population = new grid_cells*[GRID_POPULATION_SIZE];
	for (int i = 0; i < GRID_POPULATION_SIZE; i++) {
		grid_population[i] = new grid_cells[GRID_POPULATION_SIZE];
	}

	return grid_population;
}

