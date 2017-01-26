//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#ifndef _LIP_FEATURES_HEADER

#define _LIP_FEATURES_HEADER

#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
#include<iostream>
#include "utils.h"
#include "global.h"

using namespace std;



int write_movement_data_to_file(char* filename,  CvPoint2D32f* movement_array, const int inp_dimn=4);

#endif
