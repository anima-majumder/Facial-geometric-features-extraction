#ifndef _EYE_PTS_DETECTION
#define _EYE_PTS_DETECTION

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

#include "global.h"
# include "adaptive_thresholding.h"
#include "utils.h"
#include"eye_pseudo_hue_data.h"

//void adaptive_threshold(IplImage* img,float& mean, IplImage* thres, int k_w);
void invert_img(IplImage* img);
/* void locate_pts(int const contour_count, CvPoint* contour_array, CvPoint &upper_mid,CvPoint &lower_mid, CvPoint &left,CvPoint &right); */

void locate_pts(int const contour_count, CvPoint* contour_array, CvPoint & rect_center, const float & constraint_height,  CvPoint &upper_mid,CvPoint &lower_mid, CvPoint &left,CvPoint &right, const bool &flag_constraint );


void adaptive_threshold(IplImage* img,float& mean, IplImage* thres, int k_w);
void draw_circle(IplImage* img, CvPoint eyecentroid);


#endif
