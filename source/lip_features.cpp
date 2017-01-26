//This part of the code is used to detect smile in a frame

#include "lip_features.h"


void lip_features( CvPoint &ntl_left_lip_corner, CvPoint &ntl_right_lip_corner,  CvPoint &ntl_upper_lip_mid_pt, CvPoint &ntl_lower_lip_mid_pt,CvPoint &left_lip_corner,  CvPoint &right_lip_corner,   CvPoint &lip_upper_mid,   CvPoint &lip_lower_mid)
{
  
   //***********this part of the code calculates the lip movement with respect to the neutral frame ***********//
   //**********************************************************************************************************//
  

  CvPoint2D32f* lip_movement_array=new CvPoint2D32f[ 4 ];
  CvPoint2D32f* lip_data = new CvPoint2D32f[ 4 ];
  
  char filename[]="lip_feature_points.txt";

  char lip_file[] = "lip_data.txt";
 lip_movement_array[0].x = ntl_left_lip_corner.x - left_lip_corner.x;//left corner movement  +positive if outward movement
 lip_movement_array[0].y = ntl_left_lip_corner.y - left_lip_corner.y;//+ve if moves up 

 lip_movement_array[1].x = ntl_upper_lip_mid_pt.x - lip_upper_mid.x;
 lip_movement_array[1].y = ntl_upper_lip_mid_pt.y - lip_upper_mid.y;//upper mid point movement  +ve if outward movement

 lip_movement_array[2].x = right_lip_corner.x - ntl_right_lip_corner.x;//right corner movement  +ve if outward movement
 lip_movement_array[2].y = ntl_right_lip_corner.y - right_lip_corner.y;//+ve if moves up

 lip_movement_array[3].x = ntl_lower_lip_mid_pt.x - lip_lower_mid.x;//lower mid point movement
 lip_movement_array[3].y = lip_lower_mid.y - ntl_lower_lip_mid_pt.y ;// positive if downward movement 
 //****************************************//

 for(int i=0; i<4; i++)
   {

     lip_movement_array[i].x=lip_movement_array[i].x*x_scale;
     lip_movement_array[i].y=lip_movement_array[i].y*y_scale;
   }


 //write the movements data to a file *******************//
 write_movement_data_to_file( filename,lip_movement_array, 4);

 //==========================================//
 lip_data[0].x = left_lip_corner.x;
 lip_data[0].y = left_lip_corner.y;

 lip_data[1].x = lip_upper_mid.x;
 lip_data[1].y = lip_upper_mid.y;

 lip_data[2].x = right_lip_corner.x;
 lip_data[2].y = right_lip_corner.y;

 lip_data[3].x = lip_lower_mid.x;
 lip_data[3].y = lip_lower_mid.y;
 
 write_movement_data_to_file( lip_file,lip_data, 4);

 //===========================================//


 
   //*********************************************************************************************************//

 delete [] lip_movement_array;
 delete [] lip_data;
  return;
}
