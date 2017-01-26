#include "pseudo_hue_data.h"

//function to get sudo hue image from a color image \\File name can be given as an argument 

 
void sudo_hue(IplImage* inp_img, IplImage* hue_plane, char* brow_h_plane_name)
{  
  //===========not used====================================//
  //IplImage* inp_img = cvLoadImage("./a1.jpeg", 1 ); 
  //IplImage* inp_img = cvLoadImage("./eye_brow1.jpeg", 1 );  
  //========================================================//

//use a low pass filter to remove noise. 
  // // Compute the HSV image and decompose it into separate planes.
  //  IplImage* filter_img = cvCreateImage( cvGetSize(inp_img), 8, 3 );

	      // IplImage* hsv = cvCreateImage( cvGetSize(img), 8, 3 );

  //  cvSmooth(inp_img, inp_img, CV_GAUSSIAN,  3, 0, 0, 0); 
 
 

  IplImage* sudo_h_plane = cvCreateImage( cvGetSize(inp_img), 8, 1 );
	     
 

  // cvNamedWindow("RGB", CV_WINDOW_AUTOSIZE);

  // cvShowImage("RGB",inp_img);

  // cvWaitKey(wait_ms);



  // call the function pseudo_hue_data here=================//
  // ***************************************************************//
 pseudo_hue_data( inp_img, hue_plane);

  
// cvSaveImage("h_plane1.jpeg",hue_plane);
 cvSaveImage(brow_h_plane_name, hue_plane);


  //Display the hue plane image

  // cvNamedWindow("sudo_hue_eyebrow", CV_WINDOW_AUTOSIZE);

  // cvShowImage("sudo_hue",hue_plane);

  // cvWaitKey(wait_ms);


  // cvReleaseImage (&filter_img);
  cvReleaseImage (&sudo_h_plane);
  //  cvReleaseImage (&s_plane);

 // cvDestroyWindow("RGB");
 // cvDestroyWindow("sudo_hue_eyebrow");
  return;

}
