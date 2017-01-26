#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <stdio.h>
#include <cmath>



#define PI 3.14159265359
void face_alignment(IplImage* img, CvPoint2D32f& eye1_cnt, CvPoint2D32f& eye2_cnt, IplImage* img_out, const double& stand_eye_space)

{


  CvMat* rot_mat = cvCreateMat(2,3, CV_32FC1);

  img_out->origin = img->origin;

  cvZero (img_out);
  
  double theta=0.0;

  std::cout<<"y space and x space are: "<<eye2_cnt.y-eye1_cnt.y<<"\t"<<eye2_cnt.x-eye1_cnt.x<<std::endl;

  double k= (eye2_cnt.y-eye1_cnt.y)/(eye2_cnt.x-eye1_cnt.x);
  
  theta= (180/PI)*atan(k); //radian to degree

 
  
  std::cout<<"rotation angle =" <<theta<< "for " <<(eye2_cnt.y-eye1_cnt.y)/(eye2_cnt.x-eye1_cnt.x)<<std::endl;
  //===========rotate the face image at this angle=========//

  double scale = stand_eye_space/(eye2_cnt.x-eye1_cnt.x);
  
  scale=1.0; // this is done to remove scaling effect
  
  CvPoint2D32f center=cvPoint2D32f(img->width/2, img->height/2);

  cv2DRotationMatrix( center, theta, scale, rot_mat );

  cvWarpAffine( img, img_out, rot_mat );

   cvNamedWindow("rotated_image", 1);
   cvShowImage("rotated_image", img_out) ;
   cvWaitKey(0);


   
 cvDestroyWindow("rotated_image");
 


  return;
}

//EOF
