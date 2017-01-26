//======call the opencv snake algo here====================================//

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
void snake_algo(IplImage* img,CvSeq* contour, IplImage* snake_img, float &alpha, float &beta, float &gamma )
{

  //----------initialization part------------------------------//
  // float alpha = 0.1;
  // float beta = 2.0;
  // float gamma =0.1;

 
  CvSize win;
  CvTermCriteria criteria;
  int jumpPoint;
  CvPoint *WholePointArray;
  CvPoint *PointArray;
   int NUMBER_OF_SNAKE_POINTS= 80;

 PointArray = (CvPoint *)malloc(NUMBER_OF_SNAKE_POINTS * sizeof(CvPoint));

//=======main part of code =========================================//
 cout<<"total number of contour pt is :"<<contour ->total<<endl;

 //if ( contour ->total >= NUMBER_OF_SNAKE_POINTS)
  //	{

 if(contour ->total >0)
   {
     contour->total=NUMBER_OF_SNAKE_POINTS;

	  cout<<"i am inside snake algo"<<endl;
	  WholePointArray = (CvPoint *)malloc(contour->total * sizeof(CvPoint));
	  cvCvtSeqToArray(contour, WholePointArray, CV_WHOLE_SEQ); //Copy the contour to a array

	  // Number of jumps between the desired points (downsample only!)
	  jumpPoint = (int)(contour->total / NUMBER_OF_SNAKE_POINTS);
	  for (int i=0; i < NUMBER_OF_SNAKE_POINTS; i++)
	    {
	      PointArray[i].x = WholePointArray[ (int)( i*jumpPoint)].x;
	      PointArray[i].y = WholePointArray[ (int)( i*jumpPoint)].y;
	    }

	  // Now for the snake
	  criteria.max_iter = 100; // Do max N iterations
	  criteria.epsilon = 1; // If only N points is moved then terminate
	  criteria.type = CV_TERMCRIT_EPS|CV_TERMCRIT_ITER;
	  win.width = 21; // search for energi minimizing in this area around snake points
	  win.height = 21; // Be sure that width and heigth is uneven

	  // Iterate snake
	  cvSnakeImage(img, PointArray, NUMBER_OF_SNAKE_POINTS, &alpha, &beta, &gamma, CV_VALUE, win, criteria,1);// check this if alpha beta and gamma is to be used or not 
	  // Draw snake on image
	  cout<<"point array value :"<<PointArray[0].x<<endl;
	  int n = NUMBER_OF_SNAKE_POINTS;
	  //cvPolyLine( IplTmp3, &PointArray, &n, 1,1, 255, 3,8 ); 
	  cvPolyLine( snake_img, &PointArray, &n, 1,1, cvScalar(0,255,255), 1,8 ); 
	  free(WholePointArray);
	  	}

 return;
}




















