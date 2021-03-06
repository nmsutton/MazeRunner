#include <stdio.h>
#include <iostream>
#include <boost/array.hpp>

#include <boost/numeric/odeint.hpp>

#include "hippocampus.h"
#include "entorhinal_cortex.h"
#include "vision.h"
//#include "test.h"

using namespace std;

int main(int argc, char **argv)
{
	std::vector<double> detected_moves;
    vision vision_module;
	detected_moves = vision_module.moves_detected();
    //vision_module.stream_input();

	hippocampus hippocampus_module;
	hippocampus_module.compute_cell_locations(hippocampus_module.place_cell_populations,
			hippocampus_module.PLACE_POPULATION_NUMBER, hippocampus_module.PLACE_POPULATION_SIZE,
			hippocampus_module.row_spacing, hippocampus_module.col_spacing);
	hippocampus * hippocampus_module_ptr;
	hippocampus_module_ptr = &hippocampus_module;

    entorhinal_cortex entorhinal_cortex_module;
    entorhinal_cortex_module.set_hippocampus_module(hippocampus_module_ptr);
    entorhinal_cortex_module.compute_cell_locations(entorhinal_cortex_module.grid_cell_populations,
    		entorhinal_cortex_module.GRID_POPULATION_NUMBER, entorhinal_cortex_module.GRID_POPULATION_SIZE,
			entorhinal_cortex_module.row_spacing, entorhinal_cortex_module.col_spacing);
    entorhinal_cortex_module.process_activity(detected_moves);

	/*
	test test_module;
	test_module.run_test();
	*/

	return 0;
}
