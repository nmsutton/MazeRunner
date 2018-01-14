/*
 * entorhinal_cortex.h
 *
 *  Created on: Oct 15, 2017
 *      Author: nmsutton
 */

#ifndef SRC_HIPPOCAMPUS_H_
#define SRC_HIPPOCAMPUS_H_

using namespace std;

	class hippocampus
	{
	public:
		struct place_cells {
			double V;
			double pos_x = 0.0, pos_y = 0.0, pos_z = 0.0;
		};
	int PLACE_POPULATION_NUMBER = 1;
	int PLACE_POPULATION_SIZE = 25;
	place_cells** new_pop;
	place_cells*** place_cell_populations = new place_cells**[PLACE_POPULATION_SIZE];
	double row_spacing = 3.0;
	double col_spacing = 4.0;
	double R_MAX = 50;
	double FIELD_WIDTH = 20;
	double PLACE_TO_GRID_STRENGTH_MAX = 0.5;
	double SYNAPTIC_PROFILE_WIDTH = 7;

	hippocampus();
	~hippocampus();
	hippocampus::place_cells** create_place_population();
	void compute_cell_locations(hippocampus::place_cells ***place_cell_populations, int PLACE_POPULATION_NUMBER, int PLACE_POPULATION_SIZE,
			double row_spacing, double col_spacing);
	double distance(int i, int j, int i2, int j2);
	double process_activity(int i, int j, int i2, int j2);
	double place_to_grid_strength(int i, int j, int i2, int j2);

	};


#endif /* SRC_HIPPOCAMPUS_H_ */
