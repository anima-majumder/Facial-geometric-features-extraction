//************anima@ iitk ****** dated jan 27th 2012*****//
//***this part of the code is used to obtain the neutral /ref movement needed to calculate the movements  of the successive eyebrow points w.r.t the eye centers. *****************************************//
#include <cv.h>
#include <iostream>
#include <iostream>
#include <stdio.h>

using namespace std;


void calculate_neutral_lip_movement_pts ( CvPoint2D32f* lip_points_array,  CvPoint2D32f* ntl_lip_dist, CvPoint &eye_cnt1, CvPoint &eye_cnt2 )
{



  // CvPoint  ntl_eye_brow_dist;
  
float x_ntl=(eye_cnt2.x+eye_cnt1.x)/2;//here eye_cnt1 is the left eye's cnt
  
float y_ntl= (eye_cnt2.y+eye_cnt1.y)/2;
  
  for(int i=0; i<4;i++)
    {
 
 
      ntl_lip_dist[i].x= (x_ntl - lip_points_array[i].x);//left point is +ve always and right point is negative always
      ntl_lip_dist[i].y = (lip_points_array[i].y - y_ntl);//both upper and lower points will be positive
     

      
    }

    
    

  return;
}
