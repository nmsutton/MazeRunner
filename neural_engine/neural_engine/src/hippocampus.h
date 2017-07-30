#ifndef HIPPOCAMPUS_H
#define HIPPOCAMPUS_H

#include <iostream>
#include <boost/array.hpp>

#include <boost/numeric/odeint.hpp>
//#include "matplotlibcpp.h"

using namespace std;
using namespace boost::numeric::odeint;

class hippocampus
{
public:
	typedef boost::array<double, 3> state_type;


	struct grid_cells {
		double V;
		//runge_kutta4< boost::array<double, 3> > rk;
	};
	int GRID_POPULATION_NUMBER = 6;
	int GRID_POPULATION_SIZE = 25;
	//int active_grid_cell[2] = {{0,0}};

    hippocampus();
    ~hippocampus();
    hippocampus::grid_cells** create_grid_population();
    void time_step();
    void rhs();
    void write_cout();
    void spike_train();

};

#endif // HIPPOCAMPUS_H
