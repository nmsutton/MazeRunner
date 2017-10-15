/*
 * test.h
 *
 *  Created on: Oct 14, 2017
 *      Author: nmsutton
 */

#ifndef SRC_TEST_H_
#define SRC_TEST_H_

#include <iostream>
#include <boost/array.hpp>

#include <boost/numeric/odeint.hpp>
//#include "matplotlibcpp.h"
#include <vector>
#include <cmath>

using namespace std;
using namespace boost::numeric::odeint;

class test
{
public:
    test();
    ~test();
    static void rhs( const double x , double &dxdt , const double t );
    static void write_cout( const double &x , const double t );
    void run_test();
    int t_iter = 0;
    std::vector<double> x_data, y_data;
};


#endif /* SRC_TEST_H_ */
