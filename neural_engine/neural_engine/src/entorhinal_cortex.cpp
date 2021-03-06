#include "entorhinal_cortex.h"

#include "matplotlibcpp.h"
#include <cmath>

using namespace std;
using namespace boost::numeric::odeint;
//using namespace entorhinal_cortex;
namespace plt = matplotlibcpp;

entorhinal_cortex::entorhinal_cortex()
{
	/*
	 * Starting bump location is set for testing to intitialize attractor net.
	 * Automated starting location at beginning or other area could be added later.
	 */

	for (int i = 0; i < GRID_POPULATION_NUMBER; i++) {
		new_pop = create_grid_population();
		grid_cell_populations[i] = new_pop;
	}

	// Starting bump location
	bump_centriod_location[0] = 1; //x
	bump_centriod_location[1] = 1; //y
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

double entorhinal_cortex::velocity_current(int i, int j, int i2, int j2)
{
	/*
	 * Velocity current is calculated. injected_velocity_scaling variable is used
	 * for attaining excepted results for testing until further development on
	 * signal values are added.
	 */

	double bump_speed = 1.0;
	double * Vvel_magnitude = &grid_cell_populations[i][j]->Vvel;
	double * V = Vvel_magnitude;
	double Nx = (double) GRID_POPULATION_HORIZ_SIZE * col_spacing;
	double l_grid = col_spacing;
	double injected_velocity_scaling = .1;
	double a = bump_speed*(*Vvel_magnitude)*injected_velocity_scaling;

	double Cv = Nx / (a*l_grid);
	double e_i_p = 1.0;//grid_cell_populations[i][j]->e_i_p;
	double I_vel = Cv*(*V)*e_i_p;

	return I_vel;
}

void entorhinal_cortex::move_place(int i, int j, int i2, int j2)
{
	/*
	 * Move poatition for movement test by deactivating prior
	 * place cell position firing variable and activating one
	 * in a new location.
	 */
	double deactivation_voltage;
	deactivation_voltage = entorhinal_cortex::deactivation_voltage;
	double activation_voltage;
	activation_voltage = entorhinal_cortex::activation_voltage;
	double I_const = 0.0, I_theta = 0.0;
	//double * I_vel, * I_place;
	double I_vel, I_place;

	//grid_cell_populations[i][j]->Vvel = *activation_voltage;
	grid_cell_populations[i][j]->Vvel = activation_voltage;
	I_vel = velocity_current(i, j, i2, j2);
	//I_place = 0.0;

	I_place = hippocampus_module->process_activity(i, j, i2, j2);

	//grid_cell_populations[i][j]->Iext = *deactivation_voltage;
	grid_cell_populations[i][j]->Iext = deactivation_voltage;
	//grid_cell_populations[i2][j2]->Iext = I_const + I_theta + *I_vel + *I_place;
	grid_cell_populations[i2][j2]->Iext = I_const + I_theta + I_vel + I_place;
}

void entorhinal_cortex::move_place(int pop_i, string direction)
{
	/*
	 * Areas to investigate: Add decay of firing without input that automatically extinguishes
	 * prior active bump location firing? Firing will stop anyway with current formulas if there
	 * is not input?
	 * Note: look into avoiding hard coding neuron_targets size for ability to test different
	 * grid group sizes
	 */

	int targets_size = 2;//sizeof(neuron_targets) / sizeof(int);
	int neuron_targets[targets_size][2];
	int neuron_position, deactivation_position;
	double deactivation_voltage;
	deactivation_voltage = entorhinal_cortex::deactivation_voltage;
	double activation_voltage;
	activation_voltage = entorhinal_cortex::activation_voltage;

	if (direction == "up")
	{
		neuron_targets[0][0] = bump_centriod_location[0] - 1;
		neuron_targets[0][1] = bump_centriod_location[1] + 1;
		neuron_targets[1][0] = bump_centriod_location[0];
		neuron_targets[1][1] = bump_centriod_location[1] + 1;
	}
	else if (direction == "down")
	{
		neuron_targets[0][0] = bump_centriod_location[0] - 1;
		neuron_targets[0][1] = bump_centriod_location[1] - 1;
		neuron_targets[1][0] = bump_centriod_location[0];
		neuron_targets[1][1] = bump_centriod_location[1] - 1;
	}
	else if (direction == "left")
	{
		neuron_targets[0][0] = bump_centriod_location[0] - 1;
		neuron_targets[0][1] = bump_centriod_location[1];
		neuron_targets[1][0] = -1;
		neuron_targets[1][1] = -1;
	}
	else if (direction == "right")
	{
		neuron_targets[0][0] = bump_centriod_location[0] + 1;
		neuron_targets[0][1] = bump_centriod_location[1];
		neuron_targets[1][0] = -1;
		neuron_targets[1][1] = -1;
	}

	for (int i = 0; i < targets_size; i++)
	{
		if (neuron_targets[i][1] != -1)
		{
			neuron_position = (neuron_targets[i][1]*GRID_POPULATION_HORIZ_SIZE) + neuron_targets[i][0];
			grid_cell_populations[pop_i][neuron_position]->Iext = activation_voltage;
		}
	}

	deactivation_position = (prior_bump_location[1]*GRID_POPULATION_HORIZ_SIZE) + prior_bump_location[0];
	grid_cell_populations[pop_i][deactivation_position]->Iext = deactivation_voltage;

	// below not actual bump just testing
	prior_bump_location[0] = neuron_targets[0][0]; prior_bump_location[1] = neuron_targets[0][1];
}

void entorhinal_cortex::movement_test(int time_unit)
{
	switch(time_unit)
	{
	case 0 ... 20: {move_place(0, 1, 0, 0);break;}
	case 21 ... 40: {move_place(0, 0, 0, 1);break;}
	case 41 ... 60: {move_place(0, 1, 0, 5);break;}
	case 61 ... 80: {move_place(0, 5, 0, 6);break;}
	case 81 ... 100: {move_place(0, 6, 0, 7);break;}
	case 101 ... 120: {move_place(0, 7, 0, 13);break;}
	case 121 ... 140: {move_place(0, 13, 0, 12);break;}
	case 141 ... 160: {move_place(0, 12, 0, 17);break;}
	case 161 ... 180: {move_place(0, 17, 0, 18);break;}
	case 181 ... 199: {move_place(0, 18, 0, 23);break;}
	}

}

void entorhinal_cortex::movement_test_2(int time_unit)
{
	switch(time_unit)
	{
	case 0 ... 20: {move_place(0, "up");break;}
	case 21 ... 40: {move_place(0, "right");break;}
	case 41 ... 60: {move_place(0, "down");break;}
	case 61 ... 80: {move_place(0, "left");break;}
	case 81 ... 100: {move_place(0, "right");break;}
	case 101 ... 120: {move_place(0, "up");break;}
	case 121 ... 140: {move_place(0, "left");break;}
	case 141 ... 160: {move_place(0, "down");break;}
	case 161 ... 180: {move_place(0, "up");break;}
	case 181 ... 199: {move_place(0, "down");break;}
	}

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

	int p1, p2;
	double Isyn = 0.0, Iext = 0.0;
	double E_AMPA = 0.0, E_NMDA = 0.0, E_GABA_A = -75.0;
	double Vm = 0.0;
	double Im = 0.0, n = 0.0;
	double Cm_exc = 211.389, Cm_inh = 227.3;
	double * g_AMPA, * g_NMDA, * g_GABA_A, * g_EE;

	for (int i = 0; i < GRID_POPULATION_NUMBER; i++)
	{
		for (int j = 0; j < GRID_POPULATION_SIZE; j++)
		{
			for (int k = 0; k < GRID_POPULATION_SIZE; k++)
			{
				if (POPULATION_TYPE[i] == 'E') {
					p1 = i; p2 = i + 1;
					synapse(grid_cell_populations, p1, p2, j, "GABA");
				}
				else if (POPULATION_TYPE[i] == 'I') {
					p1 = i - 1; p2 = i;
					synapse(grid_cell_populations, p1, p2, j, "AMPA_NMDA");
				};

				g_AMPA = &grid_cell_populations[p1][j]->g_AMPA;
				g_NMDA = &grid_cell_populations[p1][j]->g_NMDA;
				g_GABA_A = &grid_cell_populations[p1][j]->g_GABA_A;

				Vm = grid_cell_populations[p1][j]->V;

				Isyn = *g_GABA_A*(E_GABA_A-Vm)+*g_AMPA*(E_AMPA-Vm)+*g_NMDA*(E_NMDA-Vm);
				Iext = *&grid_cell_populations[p1][j]->Iext;

				Vm = (Im + Isyn + Iext + n)/Cm_exc;
				grid_cell_populations[p1][j]->V += Vm; // using += for testing

				//if (i == 0) {cout<<floor(grid_cell_populations[i][j]->V*100);cout<<" ";}
				//if (i == 0) {cout<<floor(Iext*100);cout<<" ";}
			}
		}
		//if (i == 0) {cout<<"\n";}
	}
}

double entorhinal_cortex::distance(entorhinal_cortex::grid_cells ***grid_cell_populations, int i, int i2, int j, int j2, string syn_type)
{
	/*
	 * Distance formula:
	 * d(i,j,C,e_j_p)
	 */

	double x1 = grid_cell_populations[i][j]->pos_x;
	double x2 = grid_cell_populations[i2][j2]->pos_x;
	double y1 = grid_cell_populations[i][j]->pos_y;
	double y2 = grid_cell_populations[i2][j2]->pos_y;
	double z1 = grid_cell_populations[i][j]->pos_z;
	double z2 = grid_cell_populations[i2][j2]->pos_z;
	double C = grid_cell_populations[i][j]->C;
	double e_i_p = 1.0;//grid_cell_populations[i][j]->e_i_p;
	if (syn_type == "ie") {C = 0.0; e_i_p = 0.0;}
	double euclidean = sqrt(pow((x2-x1),2)+pow((y2-y1),2)+pow((z2-z1),2));
	//cout << "dist i: " << i << " j: " << j << "n";

	return euclidean - (C * e_i_p);
}

double entorhinal_cortex::dirac(double t1, double t2)
{
	/*
	 * Dirac delta formula:
	 * dirac = 1/delta_t for 0 < t < delta_t_max, dirac = 0 otherwise
	 * where delta_t is t2-t1, and delta_t_max = 50ms
	 * source: wikipedia.org/wiki/Dirac_delta_function
	 */

	double dirac = 0.0, delta_t = (t2-t1), delta_t_max = 50;

	if (delta_t == 0) {dirac = 1;}
	else if ((t2-t1) < delta_t_max) {dirac = 1/(t2-t1);}
	else {dirac = 0;}

	return dirac;
}

void entorhinal_cortex::synapse(entorhinal_cortex::grid_cells ***grid_cell_populations, int i, int i2, int j, string syn_type)
{
	/*
	 * Using synapse formulas:
	 * E->I: w_ij_AMPA = g_e * exp (-(d(i,j,C,e_j_p)-u)^2/2*siq_exc^2)
	 * E->I: w_ij_NMDA = C_NMDA * w_ij_AMPA
	 * I->E: w_ij_GABA = g_i * exp (-d(i,j,0,0)^2/2*siq_inh^2)
	 * E->E: w_ij_ee =   g_e * exp (-d(i,j,C,e_j_p)^2/2*siq_ee^2)
	 *
	 * Synaptic conductances:
	 * g_j_AMPA = -(g_AMPA/t_AMPA) + sum_j(w_ij_AMPA*dirac(t-t_i))
	 * g_j_NMDA = -(g_NMDA/t_NMDA) + sum_j(w_ij_NMDA*dirac(t-t_i))
	 * g_j_GABA_A = -(g_GABA_A/t_GABA_A) + sum_j(w_ij_GABA_A*dirac(t-t_i))
	 *
	 * using dirac with delta_t = 0.0 for testing, real values can be added later.
	 */

	double u = 0.433;
	double sig_exc = 1.0, sig_inh = 1.0, sig_ee = 1.0;
	double ge = 1.0, gi = 1.0;
	double C_NMDA = 0.02;
	double weight = 0.0;//, ampa = 1.0;//, syn_cond = 0.0;
	double t_AMPA = 1.0, t_NMDA = 100.0, t_GABA_A = 5.0, t_EE = 1.0;
	double * g_AMPA, * g_NMDA, * g_GABA_A, * g_EE;

	if (syn_type == "AMPA_NMDA") {
		g_AMPA = &grid_cell_populations[i][j]->g_AMPA;
		weight = 0.0;
		for (int j2 = 0; j2 < GRID_POPULATION_SIZE; j2++)
		{
			weight += (ge*exp(-pow((distance(grid_cell_populations, i, i2, j,j2,"ei")-u),2)/(2*pow(sig_exc, 2))))*dirac(1,1);
		}
		*g_AMPA = -(*g_AMPA/t_AMPA)+weight;

		g_NMDA = &grid_cell_populations[i][j]->g_NMDA;
		weight = 0.0;
		for (int j2 = 0; j2 < GRID_POPULATION_SIZE; j2++)
		{
			weight += C_NMDA * (*g_AMPA);
		}
		*g_NMDA = -(*g_NMDA/t_NMDA)+weight;
	}
	else if (syn_type == "GABA") {
		g_GABA_A = &grid_cell_populations[i][j]->g_GABA_A;
		weight = 0.0;
		for (int j2 = 0; j2 < GRID_POPULATION_SIZE; j2++)
		{
			weight += (gi*exp(-pow((distance(grid_cell_populations, i, i2, j,j2,"ie")),2)/(2*pow(sig_inh, 2))))*dirac(1,1);
		}
		*g_GABA_A = -(*g_GABA_A/t_GABA_A)+weight;
	}
	else if (syn_type == "EE") {
		g_EE = &grid_cell_populations[i][j]->g_EE;
		weight = 0.0;
		for (int j2 = 0; j2 < GRID_POPULATION_SIZE; j2++)
		{
			weight += ge*exp(-pow((distance(grid_cell_populations, i, i2, j,j2,"ee")),2)/(2*pow(sig_ee, 2)));
		}
		*g_EE = -(*g_EE/t_EE)+weight;
	}
}

void entorhinal_cortex::compute_cell_locations(entorhinal_cortex::grid_cells ***grid_cell_populations, int GRID_POPULATION_NUMBER, int GRID_POPULATION_SIZE,
		double row_spacing, double col_spacing)
{
	/*
	 * Initially generate default twisted torus locations, for initial work the cells will be on a 2d
	 * sheet for math convenience but a 3d torus can be added later.
	 *
	 * Note: in grid_row, grid_offset, and current_direction calculations +1, -1 are to have a starting index of 1 not 0 for
	 * modulo calculation.
	 */

	double row_length = floor(sqrt(GRID_POPULATION_SIZE));
	double row = 0, col = 0;
	int grid_row = 0, grid_offset = 0, total_dir = 8;
	int grid_offset_max = 8, grid_top_row = 5, grid_middle_row = 2;
	int current_direction = 0;
	double grid_max_row = 3.0;
	double stagger = 0;
	double left[2] = {-1.0, 0.0}, right[2] = {1.0, 0.0}, center[2] = {0.0, 0.0}, up[2] = {0.0, 1.0}, down[2] = {0.0, -1.0};
	double directions[total_dir][2] = {{down[0], down[1]}, {down[0], down[1]}, {left[0], left[1]}, {center[0], center[1]},
							   {right[0], right[1]}, {up[0], up[1]}, {up[0], up[1]}};//{-1.0, -1.0, -0.5, 0.0, 0.5, 1.0, 1.0}; // d d l c r u u

	for (double i = 0; i < GRID_POPULATION_NUMBER; i++)
	{
		stagger = 0;
		for (double j = 0; j < GRID_POPULATION_SIZE; j++)
		{
			row = floor(j / row_length);
			col = j - (row*row_length);
			grid_row = floor(row / grid_max_row);
			grid_offset = ((grid_row) % grid_offset_max);
			if (row >= 1.9 && (((int) row + 1) % 3) < 0.2)
				{
				current_direction = ((grid_top_row + grid_offset + (int) col) % total_dir);
				grid_cell_populations[(int) i][(int) j]->e_i_p[0] = directions[current_direction][0];
				grid_cell_populations[(int) i][(int) j]->e_i_p[1] = directions[current_direction][1];
				cout << "row "<<row<<" and 3\t";
				}
			else if (row >= .9 && (((int) row + 1) % 3) > 1.8 && (((int) row + 1) % 3) < 2.2)
				{
				current_direction = ((grid_middle_row + grid_offset + (int) col) % total_dir);
				grid_cell_populations[(int) i][(int) j]->e_i_p[0] = directions[current_direction][0];
				grid_cell_populations[(int) i][(int) j]->e_i_p[1] = directions[current_direction][1];
				stagger = .5 * row_spacing;
				cout << "row "<<row<<" and 2 \t";
				}
			else
				{
				current_direction = ((grid_offset + (int) col) % total_dir);
				grid_cell_populations[(int) i][(int) j]->e_i_p[0] = directions[current_direction][0];
				grid_cell_populations[(int) i][(int) j]->e_i_p[1] = directions[current_direction][1];
				stagger = 0;
				cout << "row "<<row<<" and 1\t";
				}
			grid_cell_populations[(int) i][(int) j]->pos_x = (col * row_spacing) + stagger;
			grid_cell_populations[(int) i][(int) j]->pos_y = row * col_spacing;
			grid_cell_populations[(int) i][(int) j]->pos_z = 0.0;

			// print locations
			//cout << " i: " << i << " j: " << j << " x: " << grid_cell_populations[(int) i][(int) j]->pos_x << " y: " << grid_cell_populations[(int) i][(int) j]->pos_y << "\n";
			cout << " i: " << i << " j: " << j << "||" << current_direction << "||" << grid_offset << "||" << col << "\te_i_p[0] " << grid_cell_populations[(int) i][(int) j]->e_i_p[0] << "\t\t||\t\te_i_p[1]:   " << grid_cell_populations[(int) i][(int) j]->e_i_p[1] << "\n";
		}
	}
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
	Create processing of visual input using spatial cognition relevant cells.
	*/

	runge_kutta_dopri5< double > rk2;
    double * x;
    double t = 0.0;
	const double dt = 0.0025;//0.1;//0.0025;

	//integrate_const( rk2 , rhs , x , 1.0 , 10.0 , 0.1, write_cout );
	for (int i = 0; i < time_span; i++) {
		t += dt;

		//movement_test(i);
		movement_test_2(i);
		time_step();

		for (int i = 0; i < GRID_POPULATION_NUMBER; i++)
		{
			for (int j = 0; j < GRID_POPULATION_SIZE; j++)
			{
				x = &grid_cell_populations[i][j]->V;
				*x = refractory(*x, refrac_threshold);
				integrate_const( rk2 , sys2 , *x , t , (t+dt) , dt);
			}
		}
		//x = &grid_cell_populations[0][18]->V;
		x = &grid_cell_populations[0][7]->V;
		x_data.push_back(*x);
	}

	plt::plot(x_data);//, y_data);
	plt::show();
}

void entorhinal_cortex::process_activity(std::vector<double> detected_moves)
{
	/*
	 * Create spike train
	 */

    spike_train();
}

void entorhinal_cortex::set_hippocampus_module(hippocampus * hippocampus_module_ptr)
{
	hippocampus_module = hippocampus_module_ptr;
}
