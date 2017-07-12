#include <stdio.h>
#include <iostream>
#include <boost/array.hpp>

#include <boost/numeric/odeint.hpp>
#include "vision.h"

using namespace std;

int main(int argc, char **argv)
{
    vision vision_module;
    vision_module.stream_input();
    
	return 0;
}
