//This part of the code is used to detect smile in a frame
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

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
void detect_lip_corner(IplImage *img,CvRect& lip_rect, CvPoint& lip_mid_point, CvPoint &left_corner_feature,CvPoint &right_corner_feature);
void  detect_lip(IplImage *img, IplImage *t_img, CvRect &lip_rect, CvSeq* lip_contour, CvPoint& upper_corner_feature, CvPoint& lower_corner_feature, CvPoint& left_corner_feature, CvPoint &right_corner_feature );
void draw_circle(IplImage* img, CvPoint eyecentroid);

int write_movement_data_to_file(char* filename,  CvPoint2D32f* movement_array, const int inp_dimn=4);

void detect_smile(IplImage* frame, CvRect &lip_roi,const CvPoint &ntl_left_lip_corner, const CvPoint &ntl_right_lip_corner, CvPoint &left_lip_corner, CvPoint &right_lip_corner,  CvPoint &lip_upper_mid, CvPoint &lip_lower_mid,  bool &smile)
{
  // ntl_left_lip_corner and ntl_right_lip_corner are the left and right lip corners of neutral face. Here it is taken as first frame's corners.  

  // // CvPoint lip_cnt;
  // CvPoint left_lip_corner;
  // CvPoint right_lip_corner;
  // CvPoint lip_upper_mid;
  // CvPoint lip_lower_mid;

 // // CvPoint left_lip_corner,right_lip_corner;
 
 //  detect_lip_corner(frame,lip_roi, lip_cnt, left_lip_corner, right_lip_corner);

  //*******************************************************************************//

  cvSetImageROI(frame, cvRect( lip_roi.x, lip_roi.y ,  lip_roi.width,  lip_roi.height));

   cvNamedWindow("for_lip", CV_WINDOW_AUTOSIZE);
   cvShowImage("for_lip", frame);
   cvWaitKey(wait_ms);
   IplImage *t_img=cvCreateImage( cvGetSize(frame), 8, 1 );
   CvSeq* lip_contour=0;
   detect_lip(frame, t_img, lip_roi, lip_contour,  lip_upper_mid, lip_lower_mid,  left_lip_corner,right_lip_corner );




 //   //***********this part of the code calculates the lip movement with respect to the neutral frame ***********//
 //   //**********************************************************************************************************//
  

 // CvPoint2D32f* lip_movement_array=new CvPoint2D32f[ 4 ];
 // char filename[]="lip_feature_points.txt";
 // lip_movement_array[0].x = ntl_left_lip_corner.x - left_lip_corner.x;//left corner movement  +positive if outward movement
 // lip_movement_array[0].y = ntl_left_lip_corner.y - left_lip_corner.y;//+ve if moves up 

 // lip_movement_array[1].x = ntl_upper_lip_mid_pt.x - lip_upper_mid.x;
 // lip_movement_array[1].y = ntl_upper_lip_mid_pt.y - lip_upper_mid.y;//upper mid point movement  +ve if outward movement

 // lip_movement_array[2].x = right_lip_corner.x - ntl_right_lip_corner.x;//right corner movement  +ve if outward movement
 // lip_movement_array[2].y = ntl_right_lip_corner.y - right_lip_corner.y;//+ve if moves up

 // lip_movement_array[3].x = ntl_lower_lip_mid_pt.x - lip_lower_mid.x;//lower mid point movement
 // lip_movement_array[3].y = lip_lower_mid.y - ntl_lower_lip_mid_pt.y ;// positive if downward movement 
 // //****************************************//

 // for(int i=0; i<4; i++)
 //   {

 //     lip_movement_array[i].x=lip_movement_array[i].x*x_scale;
 //     lip_movement_array[i].y=lip_movement_array[i].y*y_scale;
 //   }


 // //write the movements data to a file *******************//
 // write_movement_data_to_file( filename,lip_movement_array, 4);

 //   //*********************************************************************************************************//






   cvResetImageROI(frame);

   draw_circle( frame, left_lip_corner);
   draw_circle( frame, right_lip_corner);
   draw_circle( frame, lip_upper_mid);
   draw_circle( frame, lip_lower_mid);


   //=====write the lip coordinates into a file ===================//
   FILE *flp;
 flp = fopen("lip_coordinate_data.txt", "a");
 
      

 
 fprintf(flp, "%d \t %d \t  %d \t  %d \t  %d \t  %d \t  %d \t  %d \t", left_lip_corner.x, left_lip_corner.y, lip_upper_mid.x, lip_upper_mid.y, right_lip_corner.x, right_lip_corner.y, lip_lower_mid.x, lip_lower_mid.y );
    
  
  fprintf(flp, " \n" );


  fclose(flp);



  

   //=======lip coordinate data written in a file ========//
   

   //***********************************************************************************//
// int left_lip_x_dist=abs(left_lip_corner.x-ntl_left_lip_corner.x);
  // int right_lip_x_dist=abs(right_lip_corner.x-ntl_right_lip_corner.x); 
  // cout<<"left corner dist is :"<<"\t"<<left_lip_x_dist<<endl;
  // cout<<"right corner dist is :"<<"\t"<<right_lip_x_dist;
  //if(left_lip_x_dist>5 && right_lip_x_dist>5)
  int lip_x_dist=abs(right_lip_corner.x-left_lip_corner.x);
  int ntl_lip_x_dist=abs(ntl_right_lip_corner.x-ntl_left_lip_corner.x); 
  int lip_left_y_dist=ntl_left_lip_corner.y-left_lip_corner.y;//as going down y value increases
  int diff=lip_x_dist-ntl_lip_x_dist;
  
  
  cout<<"lip spread is ="<<"\t"<<diff<<endl;
  cout<<"the y dist of the lip is ="<<"\t"<<lip_left_y_dist<<endl;
  if(diff>=8 && lip_left_y_dist>0) 
    
      smile=true;
  else 
      smile=false;
  cvReleaseImage ( &t_img);
  cvDestroyWindow("for_lip");


  // delete [] lip_movement_array;
  return;
}
