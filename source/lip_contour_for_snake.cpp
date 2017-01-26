#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
#include <iostream>
#include "global.h"
 using namespace std;

void find_lip_contour_pts(IplImage * img, CvSeq* contour, IplImage * inp_img_4_snake, IplImage * roi_img, CvSeq* largest_contour, int &contour_count, CvPoint &lip_upper_mid,CvPoint &lip_lower_mid,CvPoint &lip_left,CvPoint & lip_right );
// void find_lip_contour_pts(IplImage * img, CvSeq* lip_contour,IplImage * inp_img_4_snake, IplImage * roi_img, CvSeq* largest_contour, CvPoint* point_array, int &contour_count);

void adaptive_threshold(IplImage* img,float& mean, IplImage* thres, int k_w);
void draw_lip_contour(IplImage* img, CvSeq* contours);
//*************************************************************************************************//
void lip_contour(IplImage* sudo_h_plane, IplImage* t_img, IplImage*t_img_clone, IplImage * inp_img_4_snake,IplImage*lip_roi  )

{
 
  cvSmooth( sudo_h_plane,  sudo_h_plane, CV_GAUSSIAN, 3, 3 );//optional to use


  CvSeq* lip_contour=0;

  //Threshold the image using either adaptive thresholding or convesional thresholding
  float threshold_value=0; 
  int k=5;//k value is 5 //k=0 for ritu image 
  //use adaptive thresholding 
  adaptive_threshold(sudo_h_plane, threshold_value, t_img, k);// 5 is used to add weatage to the global value. 
 
  //*************************************************************************************************//
  //erode the image to remove particle noise.   
  cvErode(t_img, t_img, 0, 3);// use erode 3 for ritu image else 2
  cvDilate(t_img, t_img, 0, 10);// use dilate for ritu image else 2
  //copy the threshold image.
    //=================================================================================================//
  cvCopy(t_img,t_img_clone);
    //    int  size_window=0;
 //    // CvPoint upper_corner_feature, lower_corner_feature, center;
 // CvPoint  center;

 // // detect_lip_mid_pts(t_img_clone, size_window,  upper_corner_feature,  lower_corner_feature, center);

    //====================================================================================================//

    //dilation is require to remove the inside blocks.
    //dilate the image 
    // cvDilate(t_img,t_img,0,2);
    // cvSaveImage("lip_sudo_thres.jpeg",t_img_clone);

    cvNamedWindow("thres", CV_WINDOW_AUTOSIZE);

    cvShowImage("thres",t_img_clone);

    cvWaitKey(wait_ms);


    //Call the contour extraction function 
    // CvRect contour_rect1, contour_rect2;
    CvSeq* largest_lip_contour=0;// CvPoint* contour_array=0;
    int contour_count;
    CvPoint lip_upper_mid,lip_lower_mid,lip_left, lip_right;
    //find_lip_contour_pts(t_img, lip_contour, inp_img_4_snake, lip_roi, largest_lip_contour,  contour_count);


    find_lip_contour_pts(t_img, 
			 lip_contour, 
			 inp_img_4_snake,
			 lip_roi,
			 largest_lip_contour, 
			 contour_count,
			 lip_upper_mid,
			 lip_lower_mid,
			 lip_left, lip_right );

    //********************************************************************************************************//
    //free (lip_contour);
    // cvDestroyWindow("thres");
    return;
}
