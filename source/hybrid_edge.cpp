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

using namespace std;




//get the hybrid image from sudo hue obtained and the intensity image of the lips. This is given by 
//r_top(i,j)=delta[(sudo_hue(i,j)-im_intensity(i,j)/N]

void hybrid_edge(IplImage * img_roi, IplImage * sudo_hue_img, IplImage * gradient_img)
{
IplImage * laplace=  cvCreateImage(cvGetSize(sudo_hue_img),IPL_DEPTH_16S ,1);
float sudo_hue_value, intensity_value;

  //Note that here roi image is a gray scale image
  uchar * img_roi_data;
  int img_roi_height, img_roi_width;//, img_roi_channels, img_roi_step;
  // get the image data
  img_roi_height    = img_roi->height;
  img_roi_width     = img_roi->width;
  // img_roi_step      = img_roi->widthStep;
  // img_roi_channels  = img_roi->nChannels;
  img_roi_data= (uchar*)img_roi->imageData;
  //=================================================================//

  //get the sudo hue image data======================================//
  uchar * data;
  // int height, width;
    int channels, step;

  // get the sudo hue image data
    //  height    = sudo_hue_img->height;
    // width     = sudo_hue_img->width;
  step      = sudo_hue_img->widthStep;
  channels  = sudo_hue_img->nChannels;
  data    = (uchar*)sudo_hue_img->imageData;


  //==================================================================// 
  //===hybrid edge is calculated from sudo hue and intensity value as follows 
  float hybrid_edge[img_roi_width][img_roi_height];
  int grad;
  for(int i=0; i<img_roi_width; i++)
    {
      for(int j=0;j<img_roi_height;j++)
	{
	  intensity_value=(float)img_roi_data[j*step + i*channels+ 0 ];
	  
	  sudo_hue_value=(float)data[j*step + i*channels+ 0 ];

	  hybrid_edge[i][j]=(sudo_hue_value-intensity_value);
	  grad=hybrid_edge[i][j];
	  cvSetReal2D(gradient_img, j, i,grad );
	}
    }


 //calculate the gradient of image obtained after subtraction and normalization. 
  cvSobel(gradient_img,laplace,0,1,3);//taking derivative along y ditection only.As there is more edge info along y-direction 
  //  cvSmooth( gradient_img, gradient_img, CV_GAUSSIAN, 3, 3 );
 

   
  //   cvLaplace(gradient_img, laplace, 3 );
     cvConvertScale(laplace,gradient_img);
  // cvCanny(gradient_img,gradient_img,255,50);
  // cvNamedWindow("gradient", CV_WINDOW_AUTOSIZE);

  // cvShowImage("gradient",gradient_img);

  // cvWaitKey(wait_ms);
  
 //============================================================================//

  cvReleaseImage(&laplace);
  //cvDestroyWindow("gradient");
  return;
}
