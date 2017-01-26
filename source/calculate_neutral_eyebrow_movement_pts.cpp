//************anima@ iitk ****** dated jan 27th 2012*****//
//***this part of the code is used to obtain the neutral /ref movement needed to calculate the movements  of the successive eyebrow points w.r.t the eye centers. *****************************************//
#include <cv.h>
#include <iostream>
#include <iostream>
#include <stdio.h>

using namespace std;


void calculate_neutral_eyebrow_movement_pts ( CvPoint2D32f* left_eyebrow_points_array, CvPoint2D32f* right_eyebrow_points_array, CvPoint2D32f* left_ntl_eye_brow_dist,CvPoint2D32f* right_ntl_eye_brow_dist, CvPoint &eye_cnt1, CvPoint &eye_cnt2 )
{



  // CvPoint  ntl_eye_brow_dist;
  
float x_ntl=(eye_cnt2.x+eye_cnt1.x)/2;//here eye_cnt1 is the left eye's cnt
  cout<<"x_ntl="<<x_ntl<<endl; 
  
  for(int i=0; i<4;i++)
    {
 
 
      left_ntl_eye_brow_dist[i].x= (x_ntl -left_eyebrow_points_array[i].x);
      left_ntl_eye_brow_dist[i].y = (eye_cnt1.y - left_eyebrow_points_array[i].y);
     

      right_ntl_eye_brow_dist[i].x= (right_eyebrow_points_array[i].x- x_ntl);
      right_ntl_eye_brow_dist[i].y = (eye_cnt2.y - right_eyebrow_points_array[i].y);


      
    }

    
    

  return;
}
