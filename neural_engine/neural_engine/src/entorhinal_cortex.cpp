#include "entorhinal_cortex.h"

#include "matplotlibcpp.h"
#include <cmath>

using namespace std;
using namespace boost::numeric::odeint;
//using namespace entorhinal_cortex;
namespace plt = matplotlibcpp;

entorhinal_cortex::entorhinal_cortex()
{
	for (int i = 0; i < GRID_POPULATION_NUMBER; i++) {
		new_pop = create_grid_population();
		grid_cell_populations[i] = new_pop;
	}
}

entorhinal_cortex::~entorhinal_cortex()
{
}

entorhinal_cortex::grid_cells** entorhinal_cortex::entorhinal_cortex::create_grid_population()
{
	grid_cells** grid_population = new grid_cells*[GRID_POPULATION_SIZE];
	for (int i = 0; i < GRID_POPULATION_SIZE; i++) {
		grid_population[i] = new grid_cells[GRID_POPULATION_SIZE];
	}

	return grid_population;
}

void entorhinal_cortex::rhs( const double x , double &dxdt , const double t )
{
    dxdt = 3.0/(2.0*t*t) + x/(2.0*t);
}

void entorhinal_cortex::sys( const entorhinal_cortex::state_type &x , entorhinal_cortex::state_type &dxdt , const double t )
{
	/*
	 * https://math.stackexchange.com/questions/1009989/how-to-take-the-integral-of-a-derivative-to-obtain-desired-result
	 * d(V)/d(t) = (-V(t) + W)/Z
	 */
	dxdt[0] = 3.0 / (2.0*t*t) + x[0] / (2.0*t);
	double W = 10.0;
	double Z = 0.02;
	//dxdt[0] = (x[1] + W) / Z;
	//dxdt[0] = 0;//20;
	//dxdt[0] = x[0]*x[0];
}

void entorhinal_cortex::sys2( const double x , double &dxdt , const double t )
{
	/*
	 * https://math.stackexchange.com/questions/1009989/how-to-take-the-integral-of-a-derivative-to-obtain-desired-result
	 * d(V)/d(t) = (-V(t) + W)/Z
	 */
	//dxdt = 3.0 / (2.0*t*t) + x / (2.0*t);
	double W = 10.0;
	double Z = 0.02;
	dxdt = (-x + W) / Z;
	//dxdt[0] = 0;//20;
	//dxdt[0] = x[0]*x[0];
}

void write_cout( const double &x , const double t )
{
    cout << t << '\t' << x << endl;
}

/*void save_x()
{
	x_data[i]
}*/

void entorhinal_cortex::time_step(int i, runge_kutta4< entorhinal_cortex::state_type > rk, entorhinal_cortex::state_type x, double t, double dt) {
	//rk.do_step( sys , x , t , dt);//, save_x );
	//integrate_const( rk , sys , x , t , (t+dt) , dt );
	//integrate_adaptive( make_controlled( 1E-12 , 1E-12 , stepper_type() ) ,rhs , x , 1.0 , 10.0 , 0.1);
	//integrate_adaptive( rk,rhs , x , 1.0 , 10.0 , 0.1);
	integrate_const( rk , sys , x , t , (t+dt) , dt );
	/*x_data[i] = x[0];
	y_data[i] = x[1];*/
	x_data.at(i) = x[0];
	y_data.at(i) = x[1];
	//integrate_adaptive( make_controlled( 1E-12 , 1E-12 , stepper_type() ) , rhs , x , 1.0 , 10.0 , 0.1 , write_cout );
}

double entorhinal_cortex::refractory(double x, double refrac_threshold)
{
	if (x >= refrac_threshold) {x = 0;}
	return x;
}

void entorhinal_cortex::spike_train(double V) {
	/*
	runge_kutta_dopri5< double > rk2;
	entorhinal_cortex::state_type x = { 0.0 , 1.0 , 1.0 };//{ 10.0 , 1.0 , 1.0 };
	double t = 0;
	const double dt = 0.1;//0.0025;

	for (int i = 0; i < time_span; i++) {
		t += dt;
		x_data.push_back(0);
		y_data.push_back(0);
		entorhinal_cortex::time_step(i, rk, x, t, dt);
	}
	*/

	runge_kutta_dopri5< double > rk2;
    double * x;
    x = &V;//&grid_cell_populations[0][0]->V;//0.0;
    //entorhinal_cortex::state_type x = { 0.0 , 1.0 , 1.0 };//{ 10.0 , 1.0 , 1.0 };

    //integrate_adaptive( make_controlled( 1E-12 , 1E-12 , stepper_type() ) , rhs , x , 1.0 , 10.0 , 0.1 , write_cout );
	double t = 0.0;
	const double dt = 0.0025;//0.1;//0.0025;
	int time_span = 2000;

	//integrate_const( rk2 , rhs , x , 1.0 , 10.0 , 0.1, write_cout );
	for (int i = 0; i < time_span; i++) {
		t += dt;
		x_data.push_back(*x);
		*x = refractory(*x, refrac_threshold);
		//if (grid_cell_populations[0][0]->V >= refrac_threshold) {grid_cell_populations[0][0]->V = 0;}
		//x_data.push_back(x[0]);
		//y_data.push_back(0);
		//integrate_const( rk2 , rhs , x , t , (t+dt) , dt);
		integrate_const( rk2 , sys2 , *x , t , (t+dt) , dt);
	}

	//std::vector<double> x1 = {{1, 2, 3, 4}};
	//plt::plot(x1);

	plt::plot(x_data);//, y_data);
	plt::show();


}

void time_step()
{
	/*
	 * Synaptic connections have input neural signals processed through them
	 */

}

void entorhinal_cortex::compute_cell_locations(entorhinal_cortex::grid_cells ***grid_cell_populations, int GRID_POPULATION_NUMBER, int GRID_POPULATION_SIZE,
		double row_spacing, double col_spacing)
{
	/*
	 * Initially generate default twisted torus locations, for initial work the cells will be on a 2d
	 * sheet for math convenience but a 3d torus can be added later.
	 */
	double row_length = floor(sqrt(GRID_POPULATION_SIZE));
	double row = 0, col = 0;
	double stagger = 0;

	for (double i = 0; i < GRID_POPULATION_NUMBER; i++)
	{
		stagger = 0;
		for (double j = 0; j < GRID_POPULATION_SIZE; j++)
		{
			row = floor(j / row_length);//floor(((i * GRID_POPULATION_NUMBER) + j) / row_length);
			col = j - (row*row_length);//((i * GRID_POPULATION_NUMBER) + j) - (row * GRID_POPULATION_SIZE);
			if (((int) row % 2) > 0.2) {stagger = .5 * row_spacing;}
			//double V = grid_cell_populations[0][0]->V;
			grid_cell_populations[(int) i][(int) j]->pos_x = (col * row_spacing) + stagger;
			grid_cell_populations[(int) i][(int) j]->pos_y = row * col_spacing;
			grid_cell_populations[(int) i][(int) j]->pos_z = 0.0;
			cout << "i: " << i << " j: " << j << " x: " << grid_cell_populations[(int) i][(int) j]->pos_x << " y: " << grid_cell_populations[(int) i][(int) j]->pos_y << "\r\n";
		}
	}
}

void entorhinal_cortex::process_activity(std::vector<double> detected_moves)
{
	/*
	 * Each neuron’s membrane potential (V m ) using equ.:
	 * C m V _ m = I m + I syn + I ext + η
	 * For testing I ext is detected_moves
	 */

    double V = grid_cell_populations[0][0]->V;
    spike_train(V);
}
