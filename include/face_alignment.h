# ifndef _FACE_ALIGN

# define _FACE_ALIGN

#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <stdio.h>
#include <cmath>

# ifndef PI
#define PI 3.14159265359
# endif

void face_alignment(IplImage* img, CvPoint2D32f& eye1_cnt, CvPoint2D32f& eye2_cnt, IplImage* img_out, const double& stand_eye_space );


# endif
