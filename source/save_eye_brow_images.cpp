//function to locate the eye brow. 
#include "save_eye_brow_images.h"

void save_eye_brow_images(IplImage *input, CvRect& eye_brow1, CvRect eye_brow2)
{
  //save eye brow 1 image  ROI to obtain the eye brow shape
  //========================================================================//
  /* set the image ROI */
  cvSetImageROI(input, cvRect(eye_brow1.x, eye_brow1.y , eye_brow1.width, eye_brow1.height));
  cvSaveImage( "eye_brow1.jpeg", input );
   /*reset image */
   cvResetImageROI(input);
   //======================================================================//
   /* set the image ROI */
   cvSetImageROI(input, cvRect(eye_brow2.x, eye_brow2.y , eye_brow2.width, eye_brow2.height));
   cvSaveImage( "eye_brow2.jpeg", input );
   /*reset image */
   cvResetImageROI(input);
   //======================================================================//

return;
}





