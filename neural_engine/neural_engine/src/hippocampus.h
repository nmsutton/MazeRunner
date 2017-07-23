#ifndef HIPPOCAMPUS_H
#define HIPPOCAMPUS_H

class hippocampus
{
public:
	struct grid_cells {
		double V;
	};
	int GRID_POPULATION_NUMBER = 6;
	int GRID_POPULATION_SIZE = 25;
    hippocampus();
    ~hippocampus();
    double** create_grid_population();

};

#endif // HIPPOCAMPUS_H
