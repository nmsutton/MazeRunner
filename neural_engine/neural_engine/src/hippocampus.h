#ifndef HIPPOCAMPUS_H
#define HIPPOCAMPUS_H

class hippocampus
{
public:
	struct grid_cells {
		double V;
	}; // NOTE: struct not in use currently
	int GRID_POPULATION_NUMBER = 6;
	int GRID_POPULATION_SIZE = 25;
	//int active_grid_cell[2] = {{0,0}};

    hippocampus();
    ~hippocampus();
    hippocampus::grid_cells** create_grid_population();

};

#endif // HIPPOCAMPUS_H
