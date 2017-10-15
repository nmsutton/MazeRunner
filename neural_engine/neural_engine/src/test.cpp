/*
 * test.cpp

 *
 *  Created on: Oct 14, 2017
 *      Author: nmsutton
 */

#include "test.h"
//#include <iostream>
#include <boost/numeric/odeint.hpp>

using namespace std;
using namespace boost::numeric::odeint;

/*
 * uncomment out below lines to get plotting working
#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;
 */

test::test()
{

}

test::~test()
{
}

/* we solve the simple ODE x' = 3/(2t^2) + x/(2t)
 * with initial condition x(1) = 0.
 * Analytic solution is x(t) = sqrt(t) - 1/t
 */

void test::rhs( const double x , double &dxdt , const double t )
{
    dxdt = 3.0/(2.0*t*t) + x/(2.0*t);
}

void test::write_cout( const double &x , const double t )
{
    cout << t << '\t' << x << endl;
    //x_data.push_back(x[0]);
}

// state_type = double
typedef runge_kutta_dopri5< double > stepper_type;

void test::run_test()
{
    double x = 0.0;
    runge_kutta_dopri5< double > rk2;
    //integrate_adaptive( make_controlled( 1E-12 , 1E-12 , stepper_type() ) , rhs , x , 1.0 , 10.0 , 0.1 , write_cout );
	double t = 1.0;
	const double dt = 0.1;//0.0025;
	int time_span = 90;

	//integrate_const( rk2 , rhs , x , 1.0 , 10.0 , 0.1, write_cout );
	for (int i = 0; i < time_span; i++) {
		t += dt;
		x_data.push_back(x);
		//y_data.push_back(0);
		integrate_const( rk2 , rhs , x , t , (t+dt) , dt);//, write_cout );
	}
	/*
	 * uncomment out below lines to get plotting working
    plt::plot(x_data);
    plt::show();
    */
}


