#include "gray_eyebrow_image_threshold.h"
#include "global.h"

void gray_eyebrow_image_threshold(IplImage* input, IplImage *contour_img, int& k, char* name, char* contour_name)
{

 IplImage* gray_img = cvCreateImage( cvGetSize(input), 8, 1 ); 
 IplImage* thres_img = cvCreateImage( cvGetSize(input), 8, 1 ); 
cvNamedWindow("thres_gray_eye_brow", CV_WINDOW_AUTOSIZE);
 cvCvtColor(input, gray_img, CV_RGB2GRAY);
 
 cvShowImage("thres_gray_eye_brow",gray_img);
  cvWaitKey(wait_ms);
 IplImage* gray_inv_img = cvCreateImage( cvGetSize(input), 8, 1 ); 

 invert_img(gray_img);
   //cvInvert(gray_img, gray_inv_img, CV_SVD_SYM);//use CV_SVD instead
 cvCopyImage(gray_img, gray_inv_img);

 float threshold_value=0; 
  // adaptive_threshold(gray_img, threshold_value, thres_img, k);
  adaptive_threshold(gray_inv_img, threshold_value, thres_img, k);
  // adaptive_threshold(h_plane, threshold_value, thres_img, k);



   cvErode(thres_img, thres_img, 0, 3);//erosion should be followed by dilation not vice versa 
   cvDilate(thres_img, thres_img, 0,3);  
   //  cvErode(thres_img, thres_img, 0, 3);
  //  cout<<"char is "<<url<<endl;
  // cvSaveImage("eye_brow_thres.jpeg",thres_img ); 
  cout<<"name is "<<name<<endl;

  cvSaveImage(name,thres_img ); 
  //copy the image in another file 
  cvCopyImage(thres_img, contour_img);
   
  // cvSaveImage( "eye_brow_thres.jpeg",thres_img ); 

  //**************************************************************************//
  //find the eye brow contour================================================//
  CvSeq* contour = 0;
  CvMemStorage* storage = cvCreateMemStorage(0);
  //CvRect object;
  // int cts = cvFindContours(img, storage, &contour,
  // 			   sizeof(CvContour),
  // 			   CV_RETR_CCOMP,
  // 			   CV_CHAIN_APPROX_SIMPLE);
  



  int count = cvFindContours(thres_img, storage, &contour);
  cout<<"contour detected"<<"number of contours detected : "<<count<<endl;
  cvClearMemStorage(storage);
  //=========================================================================//
  //Draw contour=========================================================//
    
  if( contour )
    cvDrawContours(
		   gray_img,
		   contour,
		   cvScalarAll(255),
		   cvScalarAll(255),
		   100
		   ); 
  //======================================================================//
    
 
 
  //  cvSaveImage( "eye_brow_contour.jpeg",gray_img );
  cvSaveImage( contour_name,gray_img );
  //release all memory

  cvReleaseImage(&gray_img);
cvReleaseImage(&gray_inv_img);
  cvReleaseImage(&thres_img);
 
  cvDestroyWindow("thres_gray_eye_brow");
 
  return;
}


