#ifndef _CONSTANT_
#define _CONSTANT_
//#define WAIT(10)

#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
//#include <assert.h>
//#include <math.h>
//#include <float.h>
//#include <limits.h>
//#include <time.h>
//#include <ctype.h>

# include <iostream>
# include <vector>
using namespace std;
void show_image(IplImage* img,unsigned int const wait_ms=10 );

double vertical_projection(IplImage* img);
double horizontal_projection (IplImage* img);
void invert_img(IplImage* img);
int write_scalar_data_to_file(char* filename,  const double data );
#endif
