#include "hippocampus.h"

using namespace std;
using namespace boost::numeric::odeint;

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

void rhs( const double x , double &dxdt , const double t )
{
    dxdt = 3.0/(2.0*t*t) + x/(2.0*t);
}

void sys( const hippocampus::state_type & /*x*/ , hippocampus::state_type & /*dxdt*/ , const double /*t*/ )
{
    // ...
}

void write_cout( const double &x , const double t )
{
    cout << t << '\t' << x << endl;
}

void time_step() {
	runge_kutta4< hippocampus::state_type > rk;
	//double x = 0.0;
	hippocampus::state_type x;
	//typedef runge_kutta_dopri5< double > stepper_type;
	//typedef boost::array< double , 3 > state_type;
	//runge_kutta4< boost::array<double, 3> > rk;
	//typedef boost::array<double, 3> state_type_2;
	//runge_kutta4< state_type_2 > rk_2;
	double t = 0;
	//hippocampus::state_type t;
	const double dt = 0.01;
	//rk_2.do_step( rhs , x , t , dt );
	rk.do_step( sys , x , t , dt );

	//integrate_adaptive( make_controlled( 1E-12 , 1E-12 , stepper_type() ) , rhs , x , 1.0 , 10.0 , 0.1 , write_cout );
}
