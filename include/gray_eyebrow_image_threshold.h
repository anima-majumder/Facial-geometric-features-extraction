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
#include <cstring>
#include "utils.h"
using namespace std;

void adaptive_threshold(IplImage* img,float& mean, IplImage* thres, int k_w);
void invert_img(IplImage* img);
