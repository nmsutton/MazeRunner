#include <stdio.h>
#include <iostream>
#include <boost/array.hpp>

#include <boost/numeric/odeint.hpp>
#include "vision.h"
#include "hippocampus.h"
//#include "test.h"

using namespace std;

int main(int argc, char **argv)
{

    vision vision_module;
    //vision_module.stream_input();

    

    hippocampus hippocampus_module;
    //hippocampus_module.spike_train();
    hippocampus_module.spike_train();


	/*
	test test_module;
	test_module.run_test();
	*/

	return 0;
}
