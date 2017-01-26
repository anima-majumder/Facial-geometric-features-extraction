/* Anima@ IITK --Dec 2010--The code detects the face and the facial features in a given image. Locates the facial components like
   mouth, nose, eyes, eye pupils, nostrils, eyebrows, eye corner points... */  
//================================================================================================================================

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




extern  CvHaarClassifierCascade *cascade_f;
extern CvHaarClassifierCascade *cascade_e;
extern  CvHaarClassifierCascade *cascade_m;
extern  CvHaarClassifierCascade *cascade_n;
extern  CvMemStorage		*storage;

int detectEyes(IplImage *img, CvPoint2D32f *lip_point,  CvRect& r_i0, CvRect &r_i1, CvRect *face_rect, CvRect *r2, int extra_eye_width, int extra_eye_height, int lip_width_extra, CvRect& nose);
void detectMouth(IplImage *img, CvRect *r2, CvRect& lip_r_out);
CvPoint get_centr(CvRect *r);

void draw_circle(IplImage* img, CvPoint eyecentroid);//need to be declaired
void save_eyes_images(IplImage *img, CvRect& r_i0, CvRect& r_i1, CvRect *face_rect, CvRect *eye1_rect, CvRect *eye2_rect);
void  detectEyes_pupil(IplImage *img,CvRect *pupl_cnt, CvPoint& eye_cnt, IplImage *t_img );

int CornerDetection(IplImage* src, int size_window, CvPoint& left_corner_feature,CvPoint& right_corner_feature,  CvPoint& origin);
void find_contour_rect(IplImage * img, CvRect& obj_rect1, CvRect& obj_rect2, int& cts);

void detect_nose(IplImage *img, CvRect *face_rect, CvRect& nose_rect, CvRect& nose_estm);
void locate_nostile(IplImage *img,CvRect& nose_rect, CvPoint& l_nostril, CvPoint& r_nostril);
void detect_lip_corner(IplImage *img,CvRect& lip_rect, CvPoint& lip_mid_point, CvPoint &left_corner_feature,CvPoint &right_corner_feature);
void draw_rectangle(IplImage* img, CvRect &rect, int r, int g, int b);

// void detect_lip_mid_points(IplImage* img, CvRect& lip_rect, CvPoint &lip_upper_mid,CvPoint &lip_lower_mid);
void detect_lip_all_points(IplImage* img, CvRect& lip_rect, CvPoint &lip_upper_mid,CvPoint &lip_lower_mid, CvPoint &left_lip_corner,CvPoint &right_lip_corner );
void save_image_roi (IplImage* input, CvRect& roi);

void  detect_lip(IplImage *img, IplImage *t_img, CvRect &lip_rect, CvSeq* lip_contour, CvPoint& upper_corner_feature, CvPoint& lower_corner_feature, CvPoint& left_corner_feature, CvPoint &right_corner_feature );

