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

#include"pseudo_hue_data.h"
 using namespace std;

//function to get sudo hue image from a color image 

 
void sudo_hue( IplImage* hue_plane)
{  
  // char file_name[7]="a1.jpeg";
   IplImage* inp_img = cvLoadImage("./lip.jpeg", 1 );  

  //use a low pass filter to remove noise. 
  // // Compute the HSV image and decompose it into separate planes.
  IplImage* filter_img = cvCreateImage( cvGetSize(inp_img), 8, 3 );

  // IplImage* hsv = cvCreateImage( cvGetSize(img), 8, 3 );

  cvSmooth(inp_img, inp_img, CV_GAUSSIAN,  3, 0, 0, 0); 
  //cvSmooth(img, filter_img, CV_BILATERAL,  3, 0, 0, 0);
 
  // cvCvtColor( img, hsv, CV_RGB2HSV );
  // /*(source,destination*/

  IplImage* sudo_h_plane = cvCreateImage( cvGetSize(inp_img), 8, 1 );
  // IplImage* h_plane = cvCreateImage( cvGetSize(img), 8, 1 );
  //  IplImage* s_plane = cvCreateImage( cvGetSize(img), 8, 1 );
  // IplImage* v_plane = cvCreateImage( cvGetSize(img), 8, 1 );
  //  IplImage* t_img = cvCreateImage( cvGetSize(img), 8, 1 );
 
  //====================================================//

  //call the pseudo_hue_data  file here==============//

  //*************************************************//
   pseudo_hue_data( inp_img, hue_plane );

 
  // //Split out hue component and store in h_plane
  // cvSplit(hsv, h_plane,s_plane,0, 0);
  

  // cvNamedWindow("RGB", CV_WINDOW_AUTOSIZE);

  // cvShowImage("RGB",inp_img);

  // cvWaitKey(wait_ms);

  cvSaveImage("h_plane1.jpeg",hue_plane);
  //Display the hue plane image

  // cvNamedWindow("sudo_hue", CV_WINDOW_AUTOSIZE);

  // cvShowImage("sudo_hue",hue_plane);

  // cvWaitKey(0);


  cvReleaseImage (&filter_img);
  cvReleaseImage (&sudo_h_plane);
  //  cvReleaseImage (&s_plane);

  // cvDestroyWindow("RGB");
   cvDestroyWindow("sudo_hue");
  return;
}
