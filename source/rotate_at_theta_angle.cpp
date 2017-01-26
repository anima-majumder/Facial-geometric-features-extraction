#include "cv.h"
#include "highgui.h"

void rotate_at_theta_ang(IplImage* src, IplImage* dst, double &angle)
{
 
 
  CvMat* rot_mat = cvCreateMat(2,3,CV_32FC1);
 
  dst->origin = src->origin;
  cvZero( dst );
  // Create scale
    double scale = 1.0;
  // Compute rotation matrix
   CvPoint2D32f center = cvPoint2D32f( src->width/2, src->height/2 );
   cv2DRotationMatrix( center, angle, scale, rot_mat );
  // Do the transformation
   cvWarpAffine( src, dst, rot_mat ); 
  

   return ;
}// End of main 
//========================================================================
