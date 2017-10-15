#include "hippocampus.h"
#include "matplotlibcpp.h"
#include <cmath>

using namespace std;
using namespace boost::numeric::odeint;
namespace plt = matplotlibcpp;

hippocampus::hippocampus()
{
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

void hippocampus::rhs( const double x , double &dxdt , const double t )
{
    dxdt = 3.0/(2.0*t*t) + x/(2.0*t);
}

void hippocampus::sys( const hippocampus::state_type &x , hippocampus::state_type &dxdt , const double t )
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

void hippocampus::sys2( const double x , double &dxdt , const double t )
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

void hippocampus::time_step(int i, runge_kutta4< hippocampus::state_type > rk, hippocampus::state_type x, double t, double dt) {
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

double hippocampus::refractory(double x, double refrac_threshold)
{
	if (x >= refrac_threshold) {x = 0;}
	return x;
}

void hippocampus::spike_train(double V) {
	/*
	runge_kutta_dopri5< double > rk2;
	hippocampus::state_type x = { 0.0 , 1.0 , 1.0 };//{ 10.0 , 1.0 , 1.0 };
	double t = 0;
	const double dt = 0.1;//0.0025;

	for (int i = 0; i < time_span; i++) {
		t += dt;
		x_data.push_back(0);
		y_data.push_back(0);
		hippocampus::time_step(i, rk, x, t, dt);
	}
	*/

	runge_kutta_dopri5< double > rk2;
    double * x;
    x = &V;//&grid_cell_populations[0][0]->V;//0.0;
    //hippocampus::state_type x = { 0.0 , 1.0 , 1.0 };//{ 10.0 , 1.0 , 1.0 };

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

void hippocampus::process_activity()
{
    double V = grid_cell_populations[0][0]->V;
    spike_train(V);
}
