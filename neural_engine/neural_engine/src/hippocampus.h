#ifndef HIPPOCAMPUS_H
#define HIPPOCAMPUS_H

#include <iostream>
#include <boost/array.hpp>

#include <boost/numeric/odeint.hpp>
//#include "matplotlibcpp.h"
#include <vector>
#include <cmath>

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
	grid_cells** new_pop;
	grid_cells*** grid_cell_populations = new grid_cells**[GRID_POPULATION_SIZE];
	//int active_grid_cell[2] = {{0,0}};

    hippocampus();
    ~hippocampus();
    hippocampus::grid_cells** create_grid_population();
    typedef runge_kutta_dopri5< double > stepper_type;
	runge_kutta4< hippocampus::state_type > rk;
    void time_step(int i, runge_kutta4< hippocampus::state_type > rk, hippocampus::state_type x, double t, double dt);
    static void rhs( const double x , double &dxdt , const double t );
    static void sys( const hippocampus::state_type &x , hippocampus::state_type &dxdt , const double t );
    static void sys2( const double x , double &dxdt , const double t );
    void write_cout();
    void spike_train();
    double refractory(double x, double refrac_threshold);

	int time_span = 200;
	double refrac_threshold = 9.9;
	std::vector<double> x_data, y_data;
};

#endif // HIPPOCAMPUS_H
