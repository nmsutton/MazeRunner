#include <stdio.h>
#include <iostream>
#include <boost/array.hpp>

#include <boost/numeric/odeint.hpp>
#include "vision.h"
#include "hippocampus.h"
//#include "matplotlibcpp.h"

using namespace std;

int main(int argc, char **argv)
{
    vision vision_module;
    vision_module.stream_input();
    
    hippocampus hippocampus_module;
    //hippocampus_module.spike_train();
    hippocampus_module.spike_train();

	return 0;
}
