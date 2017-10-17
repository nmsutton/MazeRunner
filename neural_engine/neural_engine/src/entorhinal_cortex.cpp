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

void entorhinal_cortex::time_step()
{
	/*
	 * Synaptic connections have input neural signals processed through them
	 *
	 * Each neuron’s membrane potential (V m ) using equ.:
	 * C_m*V_m = I m + I_syn + I ext + η
	 * For testing I ext is detected_moves
	 *
	 * I_syn(t) = g_GABA_A(t)(E_GABA_A − Vm) + g_AMPA(t)(E_AMPA − Vm) + g_NMDA(t)(E_NMDA − Vm)
	 *
	 * i2 and j2 wrap around if at the borders of the population of cells.
	 * Col_length can either be sqrt_cols or (sqrt_cols + 1) depending on the number of cells
	 * in the last row.
	 */

	double i2, j2;
	double g_AMPA, g_NMDA, g_GABA_A;
	double Isyn = 0.0;
	double E_AMPA = 0.0, E_NMDA = 0.0, E_GABA_A = -75.0;
	double Vm = 0.0;
	double Im = 0.0, Iext = 1.1, n = 0.0;
	double Cm_exc = 211.389, Cm_inh = 227.3;
	double row_max = floor(sqrt(GRID_POPULATION_SIZE));
	double col_max = 0.0;
	double sqrt_cols = floor(sqrt(GRID_POPULATION_SIZE));
	double row = 0, col = 0;

	if (GRID_POPULATION_SIZE % (int) sqrt_cols > .2)
	{col_max = sqrt_cols + 1;}
	else {col_max = sqrt_cols;}
	//cout<< "row_max " << row_max << " col_max " << col_max << "\n";

	for (int i = 0; i < GRID_POPULATION_NUMBER; i++)
	{
		for (int j = 0; j < GRID_POPULATION_SIZE; j++)
		{
			row = floor(j / row_max);
			col = j - (row*row_max);
			//if (row < row_max) {i2 = i + 1;} else {i2 = 0;}
			//if (col < col_max) {j2 = j + 1;} else {j2 = 0;}
			i2 = i;
			if (j < (GRID_POPULATION_SIZE-1)) {j2 = j + 1;} else {j2 = 0;}
			//cout << " i2 " << i2 << " j2 " << j2 << "\n";

			g_AMPA = synapse(grid_cell_populations, i, i2, j, j2, "AMPA");
			g_NMDA = synapse(grid_cell_populations, i, i2, j, j2, "NMDA");
			g_GABA_A = synapse(grid_cell_populations, i, i2, j, j2, "GABA");

			Vm = grid_cell_populations[(int) i][(int)j]->V;

			Isyn = g_GABA_A*(E_GABA_A-Vm)+g_AMPA*(E_AMPA-Vm)+g_NMDA*(E_NMDA-Vm);


			Vm = (Im + Isyn + Iext + n)/Cm_exc;
			//grid_cell_populations[(int) i][(int) j]->V = Vm;
			grid_cell_populations[(int) i][(int) j]->V += Vm; // using += for testing
		}
	}
	//grid_cell_populations[0][0]->V = 22;
}

double entorhinal_cortex::distance(entorhinal_cortex::grid_cells ***grid_cell_populations, int i, int i2, int j, int j2, string syn_type)
{
	/*
	 * Distance formula:
	 * d(i,j,C,e_j_p)
	 */

	//cout << "calc dist";
	//double x1 = 1.0, x2 = 1.0, y1 = 1.0, y2 = 1.0, z1 = 1.0, z2 = 1.0;
	double x1 = grid_cell_populations[i][j]->pos_x;
	double x2 = grid_cell_populations[i2][j2]->pos_x;
	double y1 = grid_cell_populations[i][j]->pos_y;
	double y2 = grid_cell_populations[i2][j2]->pos_y;
	double z1 = grid_cell_populations[i][j]->pos_z;
	double z2 = grid_cell_populations[i2][j2]->pos_z;
	double C = grid_cell_populations[i][j]->C;
	double e_i_p = grid_cell_populations[i][j]->e_i_p;
	if (syn_type == "ie") {C = 0.0; e_i_p = 0.0;}
	double euclidean = sqrt(pow((x2-x1),2)+pow((y2-y1),2)+pow((z2-z1),2));
	//cout << "dist i: " << i << " j: " << j << "n";

	return euclidean - (C * e_i_p);
}

double entorhinal_cortex::synapse(entorhinal_cortex::grid_cells ***grid_cell_populations, int i, int i2, int j, int j2, string syn_type)
{
	/*
	 * Using synapse formulas:
	 * E->I: w_ij_AMPA = g_e * exp (-(d(i,j,C,e_j_p)-u)^2/2*siq_exc^2)
	 * E->I: w_ij_NMDA = C_NMDA * w_ij_AMPA
	 * I->E: w_ij_GABA = g_i * exp (-d(i,j,0,0)^2/2*siq_inh^2)
	 * E->E: w_ij_ee =   g_e * exp (-d(i,j,C,e_j_p)^2/2*siq_ee^2)
	 */

	double u = 0.433;
	double sig_exc = 1.0, sig_inh = 1.0, sig_ee = 1.0;
	double ge = 1.0, gi = 1.0;
	double C_NMDA = 0.02;
	double weight = 0.0, ampa = 1.0;

	if (syn_type == "AMPA") {
		weight = ge*exp(-pow((distance(grid_cell_populations, i, i2, j,j2,"ei")-u),2)/(2*pow(sig_exc, 2)));
	}
	else if (syn_type == "NMDA") {
		ampa = ge*exp(-pow((distance(grid_cell_populations, i, i2, j,j2,"ei")-u),2)/(2*pow(sig_exc, 2)));;
		weight = C_NMDA * ampa;
	}
	else if (syn_type == "GABA") {
		weight = gi*exp(-pow((distance(grid_cell_populations, i, i2, j,j2,"ie")),2)/(2*pow(sig_inh, 2)));
	}
	else if (syn_type == "EE") {
		weight = ge*exp(-pow((distance(grid_cell_populations, i, i2, j,j2,"ee")),2)/(2*pow(sig_ee, 2)));
	}

	return weight;
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
			if (((int) row % 2) > 0.2) {stagger = .5 * row_spacing;} else {stagger = 0;}
			//double V = grid_cell_populations[0][0]->V;
			grid_cell_populations[(int) i][(int) j]->pos_x = (col * row_spacing) + stagger;
			grid_cell_populations[(int) i][(int) j]->pos_y = row * col_spacing;
			grid_cell_populations[(int) i][(int) j]->pos_z = 0.0;

			// print locations
			//cout << " i: " << i << " j: " << j << " x: " << grid_cell_populations[(int) i][(int) j]->pos_x << " y: " << grid_cell_populations[(int) i][(int) j]->pos_y << "\n";
		}
	}
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

double entorhinal_cortex::refractory(double x, double refrac_threshold)
{
	if (x >= refrac_threshold) {x = 0;}
	return x;
}

void entorhinal_cortex::spike_train() {
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
    x = &grid_cell_populations[0][0]->V;
    //entorhinal_cortex::state_type x = { 0.0 , 1.0 , 1.0 };//{ 10.0 , 1.0 , 1.0 };

    //integrate_adaptive( make_controlled( 1E-12 , 1E-12 , stepper_type() ) , rhs , x , 1.0 , 10.0 , 0.1 , write_cout );
	double t = 0.0;
	const double dt = 0.0025;//0.1;//0.0025;
	int time_span = 2000;

	//integrate_const( rk2 , rhs , x , 1.0 , 10.0 , 0.1, write_cout );
	for (int i = 0; i < time_span; i++) {
		t += dt;
		*x = refractory(*x, refrac_threshold);

		time_step();
		//*x = 15.0;
		//grid_cell_populations[0][0]->V = 22;
		integrate_const( rk2 , sys2 , *x , t , (t+dt) , dt);
		x_data.push_back(*x);
	}

	//std::vector<double> x1 = {{1, 2, 3, 4}};
	//plt::plot(x1);

	plt::plot(x_data);//, y_data);
	plt::show();


}

void entorhinal_cortex::process_activity(std::vector<double> detected_moves)
{
	/*
	 * Create spike train
	 */

    //double V = grid_cell_populations[0][0]->V;
    spike_train();
}
