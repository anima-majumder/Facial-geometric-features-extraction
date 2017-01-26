//************anima@ iitk ****** dated jan 27th 2012*****//
//***this part of the code is used to obtain the movements of eyebrow points w.r.t the eye centers. *****************************************//
#include <cv.h>
#include <iostream>
#include <iostream>
#include <stdio.h>
#include "global.h"
using namespace std;


void lip_movement_fn (CvPoint2D32f* lip_point, CvPoint &eye_cnt1, CvPoint &eye_cnt2, CvPoint2D32f* lip_movement,  CvPoint2D32f* ntl_lip_dist  )
{



  // CvPoint  ntl_eye_brow_dist;
  CvPoint lip_pt_dist=cvPoint(0,0);

  int x_ntl=(eye_cnt2.x+eye_cnt1.x)/2;//here eye_cnt1 is the left eye's cnt
  int y_ntl=(eye_cnt2.y+eye_cnt1.y)/2;

  for(int i=0; i<4; i++)
    {
 
        
      lip_pt_dist.y= ( lip_point[i].y- y_ntl );//always positive
    
      lip_pt_dist.x =(x_ntl -lip_point[i].x);//positive for left point and negative for right point
    


 
    
	//************************calculate the movement***************************************//

	lip_movement[i].y=( ntl_lip_dist[i].y-  lip_pt_dist.y  );//if +ve it is moving upward else it is moving downward.

	lip_movement[i].x = (ntl_lip_dist[i].x - lip_pt_dist.x);


   
	//scale the movement data to normalize the distance variation due to use of different cameras*********//
	//*****************************************************************************************************//
	lip_movement[i].y = lip_movement[i].y*y_scale;
	lip_movement[i].x = lip_movement[i].x*x_scale;





 }





  return;
}
