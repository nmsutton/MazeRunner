/*
 * entorhinal_cortex.cpp
 *
 *  Created on: Oct 15, 2017
 *      Author: nmsutton
 */

#include "hippocampus.h"
#include <cmath>

using namespace std;

hippocampus::hippocampus()
{
	for (int i = 0; i < PLACE_POPULATION_NUMBER; i++) {
		new_pop = create_place_population();
		place_cell_populations[i] = new_pop;
	}
}

hippocampus::~hippocampus()
{
}

hippocampus::place_cells** hippocampus::hippocampus::create_place_population()
{
	place_cells** place_population = new place_cells*[PLACE_POPULATION_SIZE];
	for (int i = 0; i < PLACE_POPULATION_SIZE; i++) {
		place_population[i] = new place_cells[PLACE_POPULATION_SIZE];
	}

	return place_population;
}

void hippocampus::compute_cell_locations(hippocampus::place_cells ***place_cell_populations, int PLACE_POPULATION_NUMBER, int PLACE_POPULATION_SIZE,
		double row_spacing, double col_spacing)
{
	/*
	 * Initially generate default twisted torus locations, for initial work the cells will be on a 2d
	 * sheet for math convenience but a 3d torus can be added later.
	 */

	double row_length = floor(sqrt(PLACE_POPULATION_SIZE));
	double row = 0, col = 0;
	double stagger = 0;

	for (double i = 0; i < PLACE_POPULATION_NUMBER; i++)
	{
		stagger = 0;
		for (double j = 0; j < PLACE_POPULATION_SIZE; j++)
		{
			row = floor(j / row_length);
			col = j - (row*row_length);
			if (((int) row % 2) > 0.2) {stagger = .5 * row_spacing;} else {stagger = 0;}
			place_cell_populations[(int) i][(int) j]->pos_x = (col * row_spacing) + stagger;
			place_cell_populations[(int) i][(int) j]->pos_y = row * col_spacing;
			place_cell_populations[(int) i][(int) j]->pos_z = 0.0;

			// print locations
			//cout << " i: " << i << " j: " << j << " x: " << grid_cell_populations[(int) i][(int) j]->pos_x << " y: " << grid_cell_populations[(int) i][(int) j]->pos_y << "\n";
		}
	}
}

double hippocampus::distance(int i, int j, int i2, int j2)
{
	//place_cell_populations[(int) i][(int) j]->pos_x
	double * x1 = &place_cell_populations[i][j]->pos_x;
	double * x2 = &place_cell_populations[i2][j2]->pos_x;
	double * y1 = &place_cell_populations[i][j]->pos_y;
	double * y2 = &place_cell_populations[i2][j2]->pos_y;
	double * z1 = &place_cell_populations[i][j]->pos_z;
	double * z2 = &place_cell_populations[i2][j2]->pos_z;
	return sqrt(pow((*x2-*x1),2)+pow((*y2-*y1),2)+pow((*z2-*z1),2));
}

double hippocampus::process_activity(int i, int j, int i2, int j2)
{
	double dist_from_center = distance(i, j, i2, j2);
	double r = hippocampus::R_MAX * exp(-(pow(dist_from_center, 2)/(2*(pow(hippocampus::FIELD_WIDTH,2)))));

	r *= place_to_grid_strength(i, j, i2, j2);

	return r;
}

double hippocampus::place_to_grid_strength(int i, int j, int i2, int j2)
{
	double dist_place_to_grid = distance(i, j, i2, j2);
	/*double * connection_weight;
	connection_weight = &hippocampus::PLACE_TO_GRID_STRENGTH_MAX * exp(-(pow(dist_place_to_grid, 2)/2*(pow(hippocampus::SYNAPTIC_PROFILE_WIDTH, 2))));

	return *connection_weight;*/
	return hippocampus::PLACE_TO_GRID_STRENGTH_MAX * exp(-(pow(dist_place_to_grid, 2)/2*(pow(hippocampus::SYNAPTIC_PROFILE_WIDTH, 2))));
}
