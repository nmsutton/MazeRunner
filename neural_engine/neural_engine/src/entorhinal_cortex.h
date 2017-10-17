#ifndef entorhinal_cortex_H
#define entorhinal_cortex_H

#include <iostream>
#include <boost/array.hpp>

#include <boost/numeric/odeint.hpp>
//#include "matplotlibcpp.h"
#include <vector>
#include <cmath>

using namespace std;
using namespace boost::numeric::odeint;

//namespace entorhinal_cortex {
	class entorhinal_cortex
	{
	public:
		typedef boost::array<double, 3> state_type;

		struct grid_cells {
			double V;
			double pos_x = 0.0, pos_y = 0.0, pos_z = 0.0;
			double C = 0.03;
			double e_i_p = 1.0;
			double g_AMPA = 1.0, g_NMDA = 1.0, g_GABA_A = 1.0, g_EE = 1.0;
			//runge_kutta4< boost::array<double, 3> > rk;
		};
		int GRID_POPULATION_NUMBER = 6;
		int GRID_POPULATION_SIZE = 25;
		double row_spacing = 3.0;
		double col_spacing = 4.0;
		grid_cells** new_pop;
		grid_cells*** grid_cell_populations = new grid_cells**[GRID_POPULATION_SIZE];
		//int active_grid_cell[2] = {{0,0}};

		entorhinal_cortex();
		~entorhinal_cortex();
		entorhinal_cortex::grid_cells** create_grid_population();
		typedef runge_kutta_dopri5< double > stepper_type;
		runge_kutta4< entorhinal_cortex::state_type > rk;
		static void sys( const entorhinal_cortex::state_type &x , entorhinal_cortex::state_type &dxdt , const double t );
		static void sys2( const double x , double &dxdt , const double t );
		void write_cout();
		void spike_train();
		double refractory(double x, double refrac_threshold);
		void process_activity(std::vector<double> detected_moves);
		void compute_cell_locations(entorhinal_cortex::grid_cells ***grid_cell_populations, int GRID_POPULATION_NUMBER, int GRID_POPULATION_SIZE,
				double row_spacing, double col_spacing);
		double distance(entorhinal_cortex::grid_cells ***grid_cell_populations, int i, int j, int j2, string syn_type);
		void synapse(entorhinal_cortex::grid_cells ***grid_cell_populations, int i, int j, string syn_type);
		void time_step();
		double dirac(double t1, double t2);

		int time_span = 200;//200;
		double refrac_threshold = 9.9;
		std::vector<double> x_data, y_data;
	};
//}

#endif // entorhinal_cortex_H
