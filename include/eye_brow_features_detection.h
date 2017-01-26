
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
using namespace std;
void locate_eye_brow(CvRect *face_rect, CvRect* eye_rect, CvRect& eye_brow);
void draw_circle(IplImage* img, CvPoint eyecentroid);
void save_eye_brow_images(IplImage *input, CvRect& eye_brow1, CvRect eye_brow2);
void extract_eye_brow_contour(IplImage *input, IplImage *contour_img, int& k, char* name, char* contour_name, char* brow_h_plane_name,  CvRect &eye_brow_r,CvPoint &brow_left, CvPoint &brow_upper_mid, CvPoint &brow_lower_mid,  CvPoint &brow_right);

//void detect_eye_brow_points(IplImage* src, CvRect& eye_brow_rect, CvPoint& origin, CvPoint &brow_upper_mid,CvPoint &brow_lower_mid, CvPoint &brow_left,CvPoint &brow_right);
void gray_eyebrow_image_threshold(IplImage* input, IplImage *contour_img, int& k, char* name, char* contour_name);

void calculate_neutral_eyebrow_movement_pts ( CvPoint2D32f* left_eyebrow_points_array, CvPoint2D32f* right_eyebrow_points_array, CvPoint2D32f* left_ntl_eye_brow_dist,CvPoint2D32f* right_ntl_eye_brow_dist, CvPoint &eye_cnt1, CvPoint &eye_cnt2 );

void eyebrow_movement_fn (CvPoint2D32f* left_eye_brow_point, CvPoint2D32f* right_eye_brow_point, CvPoint &eye_cnt1, CvPoint &eye_cnt2, CvPoint2D32f* left_eyebrow_movement, CvPoint2D32f* right_eyebrow_movement,  CvPoint2D32f* left_ntl_eye_brow_dist, CvPoint2D32f* right_ntl_eye_brow_dist  );

int write_to_text_file(char* filename, CvPoint &eye_brow_left, CvPoint &eye_brow_upper_mid, CvPoint &eye_brow_lower_mid, CvPoint &eye_brow_right );


int write_movement_data_to_file(char* filename,  CvPoint2D32f* movement_array, const int inp_dimn=4 );
