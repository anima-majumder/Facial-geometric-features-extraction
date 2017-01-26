//************anima@ iitk ****** dated jan 27th 2012*****//
//***this part of the code is used to obtain the movements of eyebrow points w.r.t the eye centers. *****************************************//
#include <cv.h>
#include <iostream>
#include <iostream>
#include <stdio.h>
#include "global.h"
using namespace std;


void eyebrow_movement_fn (CvPoint2D32f* left_eye_brow_point, CvPoint2D32f* right_eye_brow_point, CvPoint &eye_cnt1, CvPoint &eye_cnt2, CvPoint2D32f* left_eyebrow_movement, CvPoint2D32f* right_eyebrow_movement,  CvPoint2D32f* left_ntl_eye_brow_dist, CvPoint2D32f* right_ntl_eye_brow_dist  )


{



  // CvPoint  ntl_eye_brow_dist;
  CvPoint left_eye_brow_pt_dist=cvPoint(0,0);
  CvPoint right_eye_brow_pt_dist=cvPoint(0,0);
  int x_ntl=(eye_cnt2.x+eye_cnt1.x)/2;//here eye_cnt1 is the left eye's cnt


  for(int i=0; i<4; i++)
    {
 
        
	left_eye_brow_pt_dist.y= (eye_cnt1.y - left_eye_brow_point[i].y);
    
	left_eye_brow_pt_dist.x =(x_ntl -left_eye_brow_point[i].x);
    


 
	right_eye_brow_pt_dist.y= (eye_cnt2.y - right_eye_brow_point[i].y);
    	right_eye_brow_pt_dist.x =(right_eye_brow_point[i].x - x_ntl);
    
	//************************calculate the movement***************************************//

	left_eyebrow_movement[i].y=( left_eye_brow_pt_dist.y-
				     left_ntl_eye_brow_dist[i].y );//if +ve it is moving upward else it is moving downward.

	left_eyebrow_movement[i].x = (left_ntl_eye_brow_dist[i].x
				      -left_eye_brow_pt_dist.x);

	right_eyebrow_movement[i].y=( right_eye_brow_pt_dist.y-
				      right_ntl_eye_brow_dist[i].y );//if +ve it is moving upward else it is moving downward.

	right_eyebrow_movement[i].x = (right_ntl_eye_brow_dist[i].x
				       -right_eye_brow_pt_dist.x);

   
	//scale the movement data to normalize the distance variation due to use of different cameras*********//
	//*****************************************************************************************************//
	left_eyebrow_movement[i].y = left_eyebrow_movement[i].y*y_scale;
	left_eyebrow_movement[i].x = left_eyebrow_movement[i].x*x_scale;

	right_eyebrow_movement[i].y = right_eyebrow_movement[i].y*y_scale;
	right_eyebrow_movement[i].x = right_eyebrow_movement[i].x*x_scale;
	//*******************************************************************************************************//




 }





  return;
}
