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
void sudo_hue(IplImage* inp_img, IplImage* hue_plane, char* brow_h_plane_name);
void adaptive_threshold(IplImage* img,float& mean, IplImage* thres, int k_w);
void snake_algo(IplImage* img,CvSeq* contour, IplImage* snake_img, float &alpha, float &beta, float &gamma ); 
//void find_largest_contour(IplImage* img, CvSeq** contour, CvSeq* largest_contour, CvMemStorage* storage);
void draw_contour(IplImage* img, CvSeq* contours);

void locate_pts(int const contour_count, CvPoint* contour_array, CvPoint & rect_center, const float & constraint_height,  CvPoint &upper_mid,CvPoint &lower_mid, CvPoint &left,CvPoint &right, const bool &flag_constraint );

/* void locate_pts(int const contour_count, CvPoint *contour_array, CvPoint &upper_mid,CvPoint &lower_mid, CvPoint &left,CvPoint &right); */
void draw_circle(IplImage* img, CvPoint eyecentroid);
void invert_img(IplImage* img);
