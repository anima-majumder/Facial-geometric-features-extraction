#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
#include <iostream>

using namespace std;
// //include the files here
 #include "segmented_data.h"



//void find_contour_rect(IplImage * img, CvRect& obj_rect1, CvRect& obj_rect2, CvSeq* lip_contour);


void find_lip_contour_pts(IplImage * img, CvSeq* contour, IplImage * inp_img_4_snake, IplImage * roi_img, CvSeq* largest_contour, int &contour_count, CvPoint &lip_upper_mid,CvPoint &lip_lower_mid,CvPoint &lip_left,CvPoint & lip_right );

// void find_lip_contour_pts(IplImage * img, CvSeq* contour, IplImage * inp_img_4_snake, IplImage * roi_img, CvSeq* largest_contour, CvPoint* point_array,int &contour_count );
void adaptive_threshold(IplImage* img,float& mean, IplImage* thres, int k_w);

void draw_lip_contour(IplImage* img, CvSeq* contours);

void local_mean_st_dev(IplImage* img, float global_mean, IplImage* thres_img, int k_w);
void draw_circle(IplImage* img, CvPoint eyecentroid);
void detect_lip_corner(IplImage *img,CvRect &lip_rect, CvPoint& lip_mid_point,CvPoint &left_corner_feature,CvPoint &right_corner_feature );

void sudo_hue(IplImage* hue_plane);

int detect_lip_mid_pts(IplImage* src, CvPoint& upper_corner_feature, CvPoint& lower_corner_feature,  CvPoint& origin);
//void snake_algo(IplImage* img,CvSeq* contour, IplImage* snake_img );
void snake_algo(IplImage* img,CvSeq* contour, IplImage* snake_img, float &alpha, float &beta, float &gamma );


void locate_pts(int const contour_count, CvPoint* contour_array, CvPoint & rect_center, const float & constraint_height,  CvPoint &upper_mid,CvPoint &lower_mid, CvPoint &left,CvPoint &right, const bool &flag_constraint );

//Function starts here//=====//

void  detect_lip(IplImage *img, IplImage *t_img, CvRect &lip_rect, CvSeq* lip_contour, CvPoint& upper_corner_feature, CvPoint& lower_corner_feature, CvPoint& left_corner_feature, CvPoint &right_corner_feature )
{
  std::cout<<"lip_rect= "<<lip_rect.x <<"\t"<<lip_rect.y<<"\t"<<lip_rect.height <<"\t"<<lip_rect.width<<std::endl;
   
   cvSmooth(img, img, CV_GAUSSIAN,  3, 0, 0, 0); //smooth the image

   IplImage* lip_seg=cvCreateImage (cvSize(lip_rect.width, lip_rect.height), 8, 3);

  
   
   cvCopy(img, lip_seg);// prob
   
 std::cout<<"i m fine"<<std::endl;
   cvSaveImage("lip.jpeg",img);// this saving is done to use for teeth segmentation... without saving it takes whole face image 
   //call the teeth segmentation unction here *******************************//
   int pixel_count=0;// number of pixel which falls into teeth pixel location. This is done to set the k value for adaptive thresholding of lip image. for lip image without showing teeth, the threshold value will be different. 


   //********segment the teeth region ******************//


   segmented_data(pixel_count, lip_seg );//load the lip image and segment the teeth portion and save the image back for further use





   //   std::cout<<"the pixels count for teeth data is : "<<pixel_count<<std::endl;
   //****************** end of calling teeth segmentation function***********//

   // cvSaveImage("lip.jpeg",img);// lip ROI is saved in order to use it for pseudo_hue plane extraction
  IplImage* sudo_h_plane = cvCreateImage( cvGetSize(img), 8, 1 );
  IplImage* t_img_clone = cvCreateImage( cvGetSize(img), 8, 1 );
  IplImage* img_copy=cvCreateImage( cvGetSize(img), 8, 3 );
  cvCopy(img, img_copy);
  //  cout<<lip_rect.x<<endl;// this part is to get ride of compilation err. as it is not used
  //**call the sudo_hue function here********************// 
  
  sudo_hue( sudo_h_plane);
  
  //Threshold the image using either adaptive thresholding or convesional thresholding
  float threshold_value=0;    int k=5;//k value is 5 //k=0 for ritu image 
  if (pixel_count>50)
    k=5;
  else 
    k=8;

  adaptive_threshold(sudo_h_plane, threshold_value, t_img, k);
 
  if (pixel_count>50)
    {
  //***************************************************************//
  // //erode the image to remove particle noise.   
    cvErode(t_img, t_img, 0, 2);
    cvDilate(t_img, t_img, 0, 5); 
   // cvErode(t_img, t_img, 0, 2);// use erode 3 for ritu image else 2

   // cvDilate(t_img, t_img, 0, 3);// use dilate 3 for ritu image else 2 

    }


  else 

    {


     
      cvDilate(t_img, t_img, 0, 3); 
      cvErode(t_img, t_img, 0, 2);// use erode 3 for ritu image else 2

      cvDilate(t_img, t_img, 0, 3);// use dilate 3 for ritu image else 2

    }

  // cvMorphologyEx( t_img, t_img, 0, 0, CV_MOP_CLOSE, 4 );
  // cvMorphologyEx( t_img, t_img, 0, 0, CV_MOP_OPEN , 3 );


  //  cvDilate(t_img,t_img,0,1);
  
  //copy the threshold image.
  //=================================================================================================//

  cvCopy(t_img,t_img_clone);

  
  
  // cvNamedWindow("thres", CV_WINDOW_AUTOSIZE);

  // cvShowImage("thres",t_img_clone);

  // cvWaitKey(wait);
  //*************************************************************//
  //*********Call the contour extraction function 
  CvSeq* largest_lip_contour=0; 
  int contour_count=0;//largest_lip_contour->total;
 
  find_lip_contour_pts(t_img, lip_contour,sudo_h_plane, img, largest_lip_contour,  contour_count, upper_corner_feature, lower_corner_feature, left_corner_feature, right_corner_feature );

  //******** update the lip contour points w.r.t the whole image here*************************************//
  // this part of the code need to be called only when find_lip_contour_pts function is called
  upper_corner_feature.x=lip_rect.x + upper_corner_feature.x;
  upper_corner_feature.y=lip_rect.y + upper_corner_feature.y;


  lower_corner_feature.x=lip_rect.x + lower_corner_feature.x;
  lower_corner_feature.y=lip_rect.y + lower_corner_feature.y;

 
  left_corner_feature.x=lip_rect.x + left_corner_feature.x;
  left_corner_feature.y=lip_rect.y + left_corner_feature.y;

  right_corner_feature.x=lip_rect.x + right_corner_feature.x;
  right_corner_feature.y=lip_rect.y + right_corner_feature.y;

      
 

 


  // //************** this part is to detect the lip corners using simple edge detection method***************//
  // IplImage *histEq_img= cvCreateImage( cvGetSize(img_copy), 8, 1 );
  // IplImage*  gray_img = cvCreateImage( cvGetSize(img_copy), 8, 1 );
  // cvCvtColor(img_copy, gray_img, CV_RGB2GRAY);
  // cvEqualizeHist( gray_img, histEq_img);

  // cvThreshold( histEq_img, histEq_img, 5, 255,CV_THRESH_BINARY );//threshold value for ritu image is 5 rest 20


  // CvPoint lip_mid_point;

  // detect_lip_corner(histEq_img,lip_rect, lip_mid_point,left_corner_feature,right_corner_feature );

 
  // //**********************************************************************************************************//
  //    cvReleaseImage (&histEq_img);
  //   cvReleaseImage (&gray_img);
  // //*********************************************************************************************************//




  // cvReleaseImage (&filter_img);
  cvReleaseImage (&sudo_h_plane);
  // cvReleaseImage (&s_plane);
  //  cvReleaseImage (&v_plane);
  cvReleaseImage (&t_img_clone);
  //  cvReleaseImage (&contour_img);
  cvReleaseImage(& lip_seg);
  // cvDestroyWindow("RGB");
  //  cvDestroyWindow("thres");
  //  cvDestroyWindow("sudo_hue");
  // cvDestroyWindow("t_img");
  // delete hue_rg_arr;

  return;

}



//=================================================================================
//void find_contour_rect(IplImage * img, CvRect& obj_rect1, CvRect& obj_rect2,CvSeq* contour)
void find_lip_contour_pts(IplImage * img, CvSeq* contour, IplImage * inp_img_4_snake, IplImage * roi_img, CvSeq* largest_contour, int &contour_count, CvPoint &lip_upper_mid,CvPoint &lip_lower_mid,CvPoint &lip_left,CvPoint & lip_right )
{
  //  cout<<"height of inp_img_4_snake this is to avoid compilation err:"<<inp_img_4_snake->height;
  // CvSeq* largest_contour=0;
  contour = 0;
  CvMemStorage* storage = cvCreateMemStorage(0);
  //Obtain the largest area of all the contours found. 

  // double ContourArea=0.0, maxArea=0.0;
  // int maxCount=0;
  int cts=0;// contour_count=0;
  cts =  cvFindContours( img, storage, &contour, sizeof(CvContour),
			 CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );

  cvZero(img);

  std::cout<<"cts is "<<cts<<std::endl;
  //****************another way of finding largest contour*************//
  // if(cts > 0)
  //   {

  //     int count = 0;

  //     while(contour)//do as long as contours are there
  // 	{

  // 	  //compute contour area
  // 	  ContourArea = fabs(cvContourArea(contour,CV_WHOLE_SEQ));
  // 	  // cout<<"contour area is :"<<ContourArea<<endl;
  // 	  if(count == 0)
  // 	    {
  // 	      maxArea = ContourArea;
  // 	      maxCount = count;
  // 	      largest_contour=contour;
 
  // 	    }
  // 	  else
  // 	    {
  // 	      if(maxArea < ContourArea)
  // 		{
  // 		  maxArea = ContourArea;
  // 		  maxCount = count;
  // 		  largest_contour=contour;
  // 		}

  // 	    }
  // 	  contour = contour->h_next;
  // 	  count++;
  // 	}





  //   }
  //****************another way of finding largest contour*************//

  if( contour )//find the largest contour
    {
      double area, max=0;
      for( ; contour != 0; contour = contour->h_next ){
      	area = cvContourArea( contour , CV_WHOLE_SEQ );

      	if (area > max ) 
      	  {max = area;
	  
      	    largest_contour=contour;

      	  }
   
    
      }
      //      cout<<"max area for lip contour is :"<<max<<endl;

      //draw the contour image
      draw_lip_contour(roi_img , largest_contour); 
      //cvSaveImage("sudo_hue_lip_contour.jpeg",img);

      CvPoint* point_array=new CvPoint[largest_contour->total];
      //******read the coordinate values of the sequence element for largest_contour******//
      CvPoint  pt; 
      CvSeqReader reader;
      //  CvPoint point_array[largest_contour->total];
      
      cvStartReadSeq( largest_contour, &reader, 0);
      
      for (int i=0; i<largest_contour->total;i++)
	{
	  CV_READ_SEQ_ELEM(pt, reader);
	  printf(" seq point x = %d  seq point y= %d\n", pt.x, pt.y);

	}
      //********************************************************************//
      // Get contour point set.  
      //fprintf(stderr,"cvCvtSeqToArray\n");  
      cvCvtSeqToArray(largest_contour, point_array, CV_WHOLE_SEQ);  

      // //**************moving average calculation for point array************//
      // //*********** this is done for smoothing the contour ****************//
      // for (int j=0; j<largest_contour->total-1; j++)
      // 	{

      // 	  point_array[j+1].x=(point_array[j].x + point_array[j+1].x)/2;
      // 	  point_array[j+1].y=(point_array[j].y + point_array[j+1].y)/2;


      // 	}

      // //*********end of moving average code *******************************//




      //   cout<<"seq array element are :"<<endl;


      //******find the bounding rectangle of the contour ************//
      CvRect boundbox;
      boundbox = cvBoundingRect(largest_contour);
      CvPoint lip_center;
      lip_center.x=(boundbox.x + boundbox.width)/2;
      lip_center.y=(boundbox.y + boundbox.height)/2;
      float constraint_height=boundbox.height/3;
      bool flag=true;
      //************end of bounding rectangele***********************//


      for(int j=0; j<largest_contour->total; j++)
	//      cout<<point_array[j].x<<"\t"<<point_array[j].y<<endl;

      contour_count=largest_contour->total;
      //      cout<<"total number of pts in largest lip contour is :"<<"\t"<<contour_count<<endl;

      locate_pts( contour_count,  point_array,lip_center, constraint_height,  lip_upper_mid, lip_lower_mid, lip_left, lip_right, flag);

      delete [] point_array;
    }//end of the if contour loop
  //****************this function locates all four lip points from largest contour****************************************//
  //  contour_count=largest_contour->total;

  // //************************************************************************//
  // // call snake algorithm here========================================//
 
  // float alpha = 0.01;
  // float beta = 1.5;
  // float gamma = 0.1;
  // snake_algo( inp_img_4_snake, largest_contour,  roi_img, alpha, beta, gamma );
  // //show the snake image here=============================================//

  // // cvNamedWindow("snake_lip", CV_WINDOW_AUTOSIZE);

  // // cvShowImage("snake_lip",roi_img);

  // // cvWaitKey(0);


  //=================================================================//
 
  cvClearMemStorage(storage);

  //  cvDestroyWindow("snake_lip");
  // free (largest_contour);
  return;
}// end of find contour function********************************// 




void draw_lip_contour(IplImage* img, CvSeq* contours)
{
  //  cout<<"contour detected:"<<endl;
  //cvZero( img );
 
  // try to draw on a gray image
  // IplImage* lip_img = cvLoadImage("./sudo_hue_lip_contour.jpeg", 1 );
  //  cvCvtColor( lip_img, img, CV_RGB2GRAY );
  
  if( contours )
    //   cvDrawContours(
    // 		   img,
    // 		   contours,
    // 		   cvScalarAll(255),
    // 		   cvScalarAll(255),
    // 		   100
    // 		   );

    cvDrawContours(
		   img,
		   contours,
		   cvScalarAll(255),
		   cvScalarAll(255),
		   -1,

		   1,
		   8,
		   cvPoint(0,0)
		   );
 


  else
    {
      cout<<"no lip contours detected "<<endl;

      //      cvReleaseImage (&lip_img);
      return;
    }
  //Show the contour image 

  // cvNamedWindow("contour", CV_WINDOW_AUTOSIZE);

  // cvShowImage("contour",img);

    
  // cvWaitKey(wait);
  // cvDestroyWindow("contour");

  return;

}
 

//function to draw circle 

void draw_circle(IplImage* img, CvPoint eyecentroid)
{
  cvCircle( img, eyecentroid, 4, CV_RGB(255,0,0), 0.5, CV_AA, 0 );
}


//==============================================================//


int detect_lip_mid_pts(IplImage* src, CvPoint& upper_corner_feature, CvPoint& lower_corner_feature,  CvPoint& origin)
{
  // size_window=20;
  int MAX_CORNERS = 500;// usually it is 25
  CvPoint2D32f* src_features_CD = new CvPoint2D32f[ MAX_CORNERS ];
  CvSize src_size = cvGetSize(src);
  IplImage *eig_image, *temp_image;
  //,*gray_img;
  eig_image = cvCreateImage(src_size, 8, 1);
  temp_image = cvCreateImage(src_size, 8, 1);
  // gray_img = cvCreateImage( src_size, 8, 1 );
  // cvCvtColor(src, gray_img, CV_RGB2GRAY);

  int number_of_corners = 0;
  
  // Obtain the origin of the bounding rectangle
  CvRect r1 = cvBoundingRect(src, 0);
  
  origin.x=r1.x+r1.width/2;
  origin.y=r1.y+r1.height/2;
  //  cout<<"chk"<<endl;
  //  cout<<origin.x<<","<<origin.y<<endl;

  cvGoodFeaturesToTrack(src,eig_image,temp_image,src_features_CD, &number_of_corners,0.01,5.0,0,3,0,0.04);


  //  cout<<"nc="<<number_of_corners<<endl;
 
  //calculate the distance along y axis with constrain along x axis.


  //===================================================================================================================//
  double upper_distance[number_of_corners];
  double lower_distance[number_of_corners];
  double  distance[number_of_corners];
  double x_distance[number_of_corners];
  //double right_x_distance[number_of_corners];
 
 
  int cr, k;
  // k is used to initialize the data. Left distance is the +ve distance as it is calculated from the origin of eyes. Similarly right
  //distance. 
  k=number_of_corners;

  // Initialize with zeros to avoid garbage values. 
  while(k>0){
    upper_distance[k]=0.0;
    lower_distance[k]=0.0;
    k--;
  }

  
  int upper_index=0,lower_index=0;

  float upper_max=0,lower_max=0;

  for(cr=0;cr<number_of_corners;cr++)
    {
      distance[cr] =origin.y-src_features_CD[cr].y;

      if( distance[cr]>0)
	{
	  upper_distance[cr]=distance[cr];
	  x_distance[cr]=origin.x-src_features_CD[cr].x;
	}
      else
	{   
	  lower_distance[cr]=distance[cr];
	  x_distance[cr]=origin.x-src_features_CD[cr].x;
	  //	  cout<<lower_distance[cr]<<"and"<<x_distance[cr]<<endl;
	}	   
    }



  while(cr>0)
    {
    
      // check if the distance is maximum provided the x- coordinate of the corner is not more +8 and -8 from the origin. 
      if (upper_distance[cr]>upper_max)
	{
	  if (x_distance[cr]<8 && x_distance[cr]>-8)
	    {
	      upper_max=upper_distance[cr];
	      upper_index=cr;
	      //	      cout<<" x dist"<<x_distance[cr]<<endl;	   
	    }
	}
      else
	upper_max=upper_max;
     
      if (lower_distance[cr]<lower_max)
	{
	  if(x_distance[cr]<12 && x_distance[cr]>-12){
	    lower_max=lower_distance[cr];
	    lower_index=cr;
	    //	    cout<<"x dist"<<x_distance[cr]<<"and"<<"lower dist is "<<lower_distance[cr]<<endl;
	  }
	}
      else
	lower_max=lower_max;
 
      cr--;
     
    }


  //  left max coordinate point is :
  upper_corner_feature.x=src_features_CD[upper_index].x;
  upper_corner_feature.y=src_features_CD[upper_index].y;
  lower_corner_feature.x=src_features_CD[lower_index].x;
  lower_corner_feature.y=src_features_CD[lower_index].y;

  /*
  cout<<upper_corner_feature.x<<endl;
  cout<<upper_corner_feature.y<<endl;
  cout<<lower_corner_feature.x<<endl;
  cout<<lower_corner_feature.y<<endl;
  */
  //===================================================================================================================//

  //release memories
  // cvDestroyWindow("image");
  cvReleaseImage(& temp_image);
  cvReleaseImage(& eig_image);

  //  cvReleaseImage(& gray_img);
  delete [] src_features_CD;
  return number_of_corners;
 
}// end of the function

//=================================================================================================================//
// function to detect lip corners \\===============================================================================//


//The function is used to detect the lip corner points. The argument lip_rect is the rect points w.r.t the whole image
void detect_lip_corner(IplImage *img, CvRect &lip_rect, CvPoint& lip_mid_point,CvPoint &left_corner_feature,CvPoint &right_corner_feature )
{
  // /* set the image ROI */
  // cvSetImageROI(img, cvRect(lip_rect.x, lip_rect.y , lip_rect.width, lip_rect.height));
   


  // Obtain the origin of the bounding rectangle
  CvRect r1 = cvBoundingRect(img, 0);
  CvPoint origin;
  origin.x=r1.x+r1.width/2;
  origin.y=r1.y+r1.height/2;
  
  lip_mid_point.x=origin.x;
  lip_mid_point.y=origin.y;


  int size_window=20;
  // CvPoint left_lip_pt,right_lip_pt;

  //Cornern detection is done here

  int MAX_CORNERS = 100;
  CvPoint2D32f* src_features_CD = new CvPoint2D32f[ MAX_CORNERS ];
  CvSize src_size = cvGetSize(img);
  IplImage* eig_image = cvCreateImage(src_size, IPL_DEPTH_32F, 1);
  IplImage* temp_image = cvCreateImage(src_size, IPL_DEPTH_32F, 1);
  int number_of_corners = 0;
  // IplImage *histEq_img= cvCreateImage( cvGetSize(img), 8, 1 );
  // IplImage*  gray_img = cvCreateImage( cvGetSize(img), 8, 1 );
  // //gray image is the intensity image needed to be used for corner detection function
  // cvCvtColor(img, gray_img, CV_RGB2GRAY);

  // cvEqualizeHist( gray_img, histEq_img );


  // cvThreshold( histEq_img, histEq_img, 5, 255,CV_THRESH_BINARY );//threshold value for ritu image is 5 rest 20
 
  // // cvNamedWindow("hist_image", CV_WINDOW_AUTOSIZE);
  // // cvShowImage("hist_image",histEq_img);
  // // cvWaitKey(0);

 

 


  // cvSaveImage( "lip_thres.jpeg", histEq_img );
  // // Threshold the image 
  // // cvNamedWindow("hist_image", CV_WINDOW_AUTOSIZE);
  // cvShowImage("hist_image",histEq_img);
  // cvWaitKey(0);
  //the function uses gray image 
  cvGoodFeaturesToTrack(img, eig_image, temp_image, src_features_CD, &number_of_corners, .1, size_window/5, NULL);

  // use adaptive threshold
  //***********************************************************************************************//
  //Threshold the image using either adaptive thresholding or convesional thresholding

  // there is a problem in using adaptive thresholding because most of the corners are having very less 
  // color information. 

  //  IplImage* adaptive_thres = cvLoadImage("./lip_sudo_thres.jpeg", 1 );
  
  // cvGoodFeaturesToTrack(adaptive_thres,eig_image,temp_image,src_features_CD, &number_of_corners,0.01,5.0,0,3,0,0.04);
  //   //***********************************************************************************************//

  printf( "\nNumber of corner features found = %d\n",number_of_corners );
  /*
    calculate the distance between the corner point found and the eye pupil center. before calculating check if the 
    corner point left is the pupil or right of the pupil. the max of the points belongs to the left of the pupil is 
    is considered as the left corner point and the max of the points belongs to the right of pupil is considered as
    right corner point. Right now only distance along x direction is taken into consideration. 
  */
  double left_distance[number_of_corners];
  double right_distance[number_of_corners];
  double  distance[number_of_corners];
  // CvPoint left_corner_feature,right_corner_feature; 
 
 
  int cr, k;
  // k is used to initialize the data 
  k=number_of_corners;
 
  while(k>0){
    left_distance[k]=0.0;
    right_distance[k]=0.0;
    k--;
  }

  
  int left_index=0,right_index=0;

  float left_max=0,right_max=0;

  for(cr=0;cr<number_of_corners;cr++)
    {
      distance[cr] =origin.x-src_features_CD[cr].x;

      if( distance[cr]>0)
       
	left_distance[cr]=distance[cr];
	
     
      else
	   
	right_distance[cr]=distance[cr];
	
	   
    }

  //  cr--;// 

  while(cr>0)
    {
      //      cout<<"left corner points distance are : "<<left_distance[cr]<<"at the index "<<cr<<endl; 
      //      cout<<"right corner points distance are : "<<right_distance[cr]<<endl; 
      if (left_distance[cr]>left_max)
	{
	  left_max=left_distance[cr];
	  left_index=cr;
	}
      else
	left_max=left_max;
     
      if (right_distance[cr]<right_max)
	{
	  right_max=right_distance[cr];
	  right_index=cr;
	}
      else
	right_max=right_max;
      cr--;
     
    }

  //  cout<<"left max ="<<left_max<<"at the index "<<left_index<<endl;
  //cout<<"right max ="<<right_max<<"at the index"<<right_index<<endl;
  //  left max coordinate point is :
  left_corner_feature.x=src_features_CD[left_index].x;
  left_corner_feature.y=src_features_CD[left_index].y;
  right_corner_feature.x=src_features_CD[right_index].x;
  right_corner_feature.y=src_features_CD[right_index].y;

 
  //     distance[cr] = EuclideanDistance(featuresexp, src_features_CD[cr]);
 
 
  cvCircle( img, left_corner_feature, 4, CV_RGB(150,255,0), 0.5, CV_AA, 0 );
  cvCircle( img, right_corner_feature, 4, CV_RGB(150,255,0), 0.5, CV_AA, 0 );
   
 
  // /* reset the image after the work is done. */
  // cvResetImageROI(img);

  //the lip corners w.r t the original image is :

  left_corner_feature.x=lip_rect.x+left_corner_feature.x;
  left_corner_feature.y=lip_rect.y+left_corner_feature.y;
  right_corner_feature.x=lip_rect.x+right_corner_feature.x;
  right_corner_feature.y=lip_rect.y+right_corner_feature.y;
  //  cout<<"ok"<<endl;

  // cvDestroyWindow("lip");
 
  cvReleaseImage(&eig_image);
  cvReleaseImage(&temp_image);
  // cvReleaseImage(&gray_img);
  // cvReleaseImage (&histEq_img);
  // cvReleaseImage(&adaptive_thres);
  delete [] src_features_CD;
  // cvDestroyWindow("hist_image");
  



  return;

}// end of the function -=====================================================================================================//
