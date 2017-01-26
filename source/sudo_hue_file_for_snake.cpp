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
#include "global.h"
using namespace std;

//function to get sudo hue image from a color image 

 
void sudo_hue_snake(IplImage* hue_plane)
{  
  // char file_name[7]="a1.jpeg";
  IplImage* inp_img = cvLoadImage("./lip_sudo_hue.jpeg", 1 );  

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
 
 
 
  // //Split out hue component and store in h_plane
  // cvSplit(hsv, h_plane,s_plane,0, 0);
  

  cvNamedWindow("RGB", CV_WINDOW_AUTOSIZE);

  cvShowImage("RGB",inp_img);

  cvWaitKey(wait_ms);


  //Get data of the images of each plane.
  // This part of the code is used to obtain the sudo hue plane of the image. 
   
  uchar * data;
  int height, width, channels, step;
  float r_data, g_data;
 
  float hue_rg, hue_rg_smallest=0, hue_rg_largest=0;
  int sudo_hue;

  // get the image data
  height    = inp_img->height;
  width     = inp_img->width;
  step      = inp_img->widthStep;
  channels  = inp_img->nChannels;
  data    = (uchar*)inp_img->imageData;
  //  CvScalar s;
  float hue_rg_arr[width][height];
  printf("Processing a %dx%d image with %d channels\n",height,width,channels); 

  for(int i=0;i< width;i++)
    {
      for(int j=0;j<height;j++)
	{
	  r_data=(float)data[j*step + i*channels+ 0 ];
	  //  cout<<"red data are :"<<r_data<<endl;


	  g_data=(float)data[j*step + i*channels+ 1 ];
	  // cout<<"green data are :"<<g_data<<endl;

	  // calculate the  hue_rg value of the image as follows

	  hue_rg=r_data/(g_data+r_data);

	  if (hue_rg<hue_rg_smallest)
	    hue_rg_smallest=hue_rg;
	  else
	    hue_rg_smallest=hue_rg_smallest;
	  //calculate the largest hue_rg value


	  if (hue_rg>hue_rg_largest)
	    hue_rg_largest=hue_rg;
	  else
	    hue_rg_largest=hue_rg_largest;

	  // obtain the smallest and largest value of hue_rg
	  //  cout<<"hue rg : "<<hue_rg<<endl;
	  hue_rg_arr[i][j]=hue_rg;
	}
    }

  //Obtain the sudo hue given by sudo_hue=(hue_rg-hue_rg_smallest)/(hue_rg_largest-hue_rg_smallest)
  int s1;
  for (int i=0;i<width;i++)
    {
      for (int j=0;j<height;j++)
	{

	  //it is multiplied by 255 as (hue_rg_arr[i][j]-hue_rg_smallest)/(hue_rg_largest-hue_rg_smallest) is giving the normalized 
	  //sudo hue value. Normalized to 0-1 
	  
	  sudo_hue=255*(hue_rg_arr[i][j]-hue_rg_smallest)/(hue_rg_largest-hue_rg_smallest);

	  // s.val[0]=sudo_hue;
	  s1=sudo_hue;
	  //  cout<<"sudo hue is :"<<sudo_hue<<endl;
	  // cvSet2D(h_plane,j,i,s); // set the (i,j) pixel value for hue plane image
	  cvSetReal2D(hue_plane, j, i,s1 );

	}
    }


  cvSaveImage("h_plane1.jpeg",hue_plane);
  //Display the hue plane image

  cvNamedWindow("sudo_hue", CV_WINDOW_AUTOSIZE);

  cvShowImage("sudo_hue",hue_plane);

  cvWaitKey(wait_ms);


  cvReleaseImage (&filter_img);
  cvReleaseImage (&sudo_h_plane);
  //  cvReleaseImage (&s_plane);

  cvDestroyWindow("RGB");
  cvDestroyWindow("sudo_hue");
  return;
}

