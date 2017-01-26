/* Anima@ IITK --Dec 2010--The code detects the face and the facial features in a given image. Locates the facial components like
   mouth, nose, eyes, eye pupils, nostrils, eyebrows, eye corner points... */  
//=================================================================================================
#include "eye_mouth_feature_detect.h"
#include "global.h"
#include <stdlib.h>
/* functions starts here-----------------------------------
   Eye detection function is to localize the position of eyes . Initially it will localize the face. After face is obtained it will loca-
   lize the eyes. The coordinates of the rectangular box of face , and eyes are passed to the function. 
   The function returns -1 if right eye is detected first, 1 if left eye is detected first and 0 if no eye is detected. */

//=================================================================
int  detectEyes(IplImage *img,CvPoint2D32f *lip_point, CvRect& r_i0, CvRect& r_i1, CvRect *face_rect, CvRect *r2, int extra_eye_width, int extra_eye_height,  int lip_width_extra, CvRect& nose, int &chk_sign, bool &is_first_frame )
{
  
  int i;
  CvPoint cnt, r1;
  CvPoint2D32f eye_cnt[2];
  //  CvRect roi_eye;
  //  int chk_sign;
  //  bool right_eye_first=false;
  CvSeq *faces = cvHaarDetectObjects(
				     img, cascade_f, storage,
				     1.1, 3, CV_HAAR_DO_CANNY_PRUNING, cvSize( 40, 40 ) );
  //  cout<<"total face found="<<faces->total<<endl;
  /* return if not found */

  if (faces->total == 0)
    {
      std::cout<<"no face is detected in this frame!" <<std::endl; 
      return 0;
    }
  /* draw a rectangle */
  CvRect *r = (CvRect*)cvGetSeqElem(faces, 0);

  //here r corresponds to the rectangle w.r.t the whole face. 
  //update the face rectange data.

  face_rect->x=r->x;
  face_rect->y=r->y;
  face_rect->width=r->width;
  face_rect->height=r->height;
  

  //set the face location as previous face location if the x or y diff
  //between two frame's face location is too large=======//
    if(!is_first_frame)
      {

	std::cout<<"a =\t"<< face_rect->x<<"\t"<<face_rect_prev->x<<std::endl;
      if ( abs(face_rect->x-face_rect_prev->x)>50 || abs(face_rect->y-face_rect_prev->y)>50)
	{
	  std::cout<<"prev face rect: \t" <<face_rect_prev->x<<"\t"<<face_rect_prev->y<<std::endl;
	
	 
	 

 face_rect->x=face_rect_prev->x;
 face_rect->y=face_rect_prev->y;
 face_rect->width=face_rect_prev->width;
 face_rect->height=face_rect_prev->height;

 //===============================//

 r->x=face_rect_prev->x;
 r->y=face_rect_prev->y;
 r->width=face_rect_prev->width;
 r->height=face_rect_prev->height;
 //==================================//

  
	}

      else
	{
	face_rect_prev->x=face_rect->x;
      face_rect_prev->y=face_rect->y;
      face_rect_prev->width=face_rect->width;
      face_rect_prev->height=face_rect->height;}
	
	}
    else
      
      {face_rect_prev->x=face_rect->x;
      face_rect_prev->y=face_rect->y;
      face_rect_prev->width=face_rect->width;
      face_rect_prev->height=face_rect->height;}
 

  
 
  //r1 is used to get the eye center
  r1.x=r->x;
  r1.y=r->y;
 
  //get the r2 values to be used  for mouth detection //first rectangle after face detection
  r2->x=r->x;
  r2->y=r->y;
  r2->width=r->width;
  r2->height=r->height;
  /* reset buffer for the next object detection */
  // cout<<"The r2 values are :"<<r2->x<<", "<<r2->y<<" ,"<<r2->width<<" ,"<<r2->height<<endl; 

  cvClearMemStorage(storage);
  

/* Set the Region of Interest: estimate the eyes' position */
  cvSetImageROI(img, cvRect(r->x, r->y + (r->height/5.5), r->width, r->height/3.0));
  
  //face images are not same. so use different scale factor for different face images.
  //for eye detection, the scale factor is changes according to the size of the  face obtained.
  //As of now it is only done for threshold value =250. Later it can be extended for adaptive threshold value. It is working well now. 
  float scale_factor=0.0;
  if (r2->width<250)
    scale_factor=1.10;
  else
    scale_factor=1.15;


  /* detect eyes */
  CvSeq* eyes = cvHaarDetectObjects( 
				    img, cascade_e, storage,
				    scale_factor, 3, 0, cvSize(25, 15));
  // cout << __FILE__ << " " << __FUNCTION__ << " " << __LINE__ << endl;
  
  if(eyes->total>=2)
{
 
  //copy the 2nd eye seq roi into r_i1
  //copy the 1st eye seq roi into r_i0
  CvRect* r_ik;
  r_ik=(CvRect*)cvGetSeqElem(eyes,1);
 
 r_i1=*r_ik;
 
  CvRect* r_it;
  r_it = (CvRect *)cvGetSeqElem( eyes, 0 );
 
  r_i0 = *r_it;
  
 

 
    if((r_ik->x-r_it->x)<0)
      {
	//	right_eye_first=true;
    chk_sign=-1;//i.e right eye is detected first
    r_i0=*r_ik;//left eye_roi
    r_i1=*r_it;//right eye roi
      } 
    else
      {
    chk_sign=1;//i.e left eye is detected first
    r_i0=*r_it;//left eye roi
    r_i1=*r_ik;// right eye roi
      }


    r_i0.x = r_i0.x - extra_eye_width;
    r_i1.x = r_i1.x - extra_eye_width;

    r_i0.width = r_i0.width + 2*extra_eye_width;
    r_i1.width = r_i1.width + 2*extra_eye_width;

    r_i0.y = r_i0.y + extra_eye_height;
    r_i1.y = r_i1.y + extra_eye_height;


    r_i0.height = r_i0.height + extra_eye_height;
    r_i1.height = r_i1.height + extra_eye_height;

 
  //**********************************************************************
 
  for( i = 0; i < (eyes ? eyes->total : 0); i++ ) {
    //  cout << __FILE__ << " " << __FUNCTION__ << " " << __LINE__ << endl;
    //  cout<<i+1<<" total number of eye_detected="<<"\t"<<eyes->total<<endl;
   
    if (i==0)
      r=r_it;
    else
      r=r_ik;
   
   cnt=get_centr(r);//get the eye center
    
    eye_cnt[i].x=cnt.x+r1.x;
    eye_cnt[i].y=cnt.y+r1.y;

  }
  //**************************************************************************************//
  //estimate the lip position having known the eye centers
  float lip_scale=1.0;
  
  if (r2->width<250)
    lip_scale=1.0;
  else
    lip_scale=1.4;
  //  int sign_chk=eye_cnt[1].x-eye_cnt[0].x;
 
  if ( chk_sign<0)
    {
      //  cout<< "info:  Right eye is detected first" <<endl;
     
      lip_point[0].x=eye_cnt[1].x-lip_width_extra;//-10 is extra 
      lip_point[0].y=eye_cnt[1].y + (-1)* lip_scale *(eye_cnt[1].x-eye_cnt[0].x);
      lip_point[1].x=eye_cnt[0].x+lip_width_extra;// +10 is extra
      lip_point[1].y=eye_cnt[0].y + (-1)*lip_scale *(eye_cnt[1].x-eye_cnt[0].x);
    }

  else

    {
      //  cout<< "info:  Left eye is detected first" <<endl;
      lip_point[0].x=eye_cnt[0].x-lip_width_extra;// -10 is extra
      lip_point[0].y=eye_cnt[0].y + lip_scale *(eye_cnt[1].x-eye_cnt[0].x);
      lip_point[1].x=eye_cnt[1].x+lip_width_extra;// +10 is extra
      lip_point[1].y=eye_cnt[1].y + lip_scale *(eye_cnt[1].x-eye_cnt[0].x);
    }
 
  //lip position estimation done.


  //clear the storage for next object detection
  cvClearMemStorage(storage);
  cvResetImageROI(img);

	
    
  //detect mouth using haar cascade :used in the function detectMouth

  r2->x=lip_point[0].x;
  r2->y=lip_point[0].y;
  r2->width=(lip_point[1].x- lip_point[0].x);
 
  // r2->height=80;//check// it should be some scale of face height
  r2->height=face_rect->height/3.0;

  // //nose rectangle w.r.t to whole image is // as the eye detection sequence is not know we need to check which one is left/right eye. 

      nose.x=r_i0.x+r_i0.width/2+face_rect->x;
      nose.y=r_i0.y+r_i0.height/2+face_rect->y+face_rect->height/5.5;
      nose.width=r_i1.x-r_i0.x;
      nose.height=face_rect->height/3;

      std::cout<<"prev eye loc =\t"<<eye1r0_prev.x <<"\t"<<eye1r0_prev.y<<std::endl;
      std::cout<<"current eye loc =\t"<<r_i0.x <<"\t"<<r_i0.y<<std::endl;
      //set the previous eye location here

      eye1r0_prev = r_i0;
      eye2r1_prev = r_i1;
      nose_prev = nose;

      lip_position_prev->x= lip_point->x;
      lip_position_prev->y= lip_point->y;
 

      lip_rect_prev->x = r2->x;
      lip_rect_prev->y = r2->y;
      lip_rect_prev->height = r2->height;
      lip_rect_prev->width = r2->width;
      //=======================================//

 }

  else

    {cout<<"No eyes are detected in the frame!! previous eye's location is set here  "<<endl;

      r_i0 = eye1r0_prev;
      r_i1 = eye2r1_prev;
      nose = nose_prev;
      
      lip_point->x = lip_position_prev->x;
      lip_point->y = lip_position_prev->y; 

      r2->x = lip_rect_prev->x;
      r2->y = lip_rect_prev->y;
      r2->height = lip_rect_prev->height;
      r2->width = lip_rect_prev->width;
  
    }
  
 
  
  //=======================================//
  
  return chk_sign;

}//end of function detect_eyes.

//*****************************************************************************************//


//function get the center for eyes
// here the inside rectangle is the reference point for other centers. 
CvPoint get_centr(CvRect *r)
{
  CvPoint point;
  point= cvPoint (r->x + (r->width)/2,r->y + (r->height)/2);
  

  return point;
}//end of function get_centr.


//=======================================================================
//Function to draw a circle

// void draw_circle(IplImage* img, CvPoint eyecentroid)
// {

//   cvCircle( img, eyecentroid, 4, CV_RGB(255,0,0), 0.5, CV_AA, 0 );

// }
//======================================================================
//======================================================================
//function to detect mouth
void detectMouth(IplImage *img, CvRect *r2, CvRect& lip_r_out)
{
  cvClearMemStorage( storage );
  /* Set the Region of Interest: estimate the lip position */
  cvSetImageROI(img, cvRect(r2->x, r2->y , r2->width, r2->height));

  // cout<<"The estimated lip positions are: "<<r2->x<<", " <<r2->y<<", " <<r2->width <<", " <<r2->height<<endl;
  // CvSeq* mouth = cvHaarDetectObjects( 
  // 				     img, cascade_m, storage,
  // 				     1.1, 10, 0, cvSize(20, 20));
 
  // IplImage*  gray_mouth_img = cvCreateImage( cvGetSize(img), 8, 1 );
  //  //gray image is the intensity image needed to be used for corner detection function
  //  cvCvtColor(img, gray_mouth_img, CV_RGB2GRAY);
 
  CvSeq* mouth = cvHaarDetectObjects( 
				     img, cascade_m, storage,
				     1.1, 2, CV_HAAR_DO_CANNY_PRUNING, cvSize(40, 40));
 


  // cvNamedWindow("mouth", CV_WINDOW_AUTOSIZE);

  // cvShowImage("mouth",img);

  // cvWaitKey(wait_ms);
  show_image(img, wait_ms);
 
  CvRect *lip_r;

  for(int i = 0; i < (mouth ? mouth->total : 0); i++ ) {
    //  cout << __FILE__ << " " << __FUNCTION__ << " " << __LINE__ << endl;
    // cout<<i+1<<" mouth _detected"<<endl;
    lip_r= (CvRect*)cvGetSeqElem( mouth, i );
    
    // cvRectangle(img, 
    // 		cvPoint(lip_r->x, lip_r->y), 
    // 		cvPoint(lip_r->x + lip_r->width, lip_r->y + lip_r->height),
    // 		CV_RGB(255, 0, 0), 1, 8, 0);
    //  cout<<"the detected mouth position is "<<lip_r->x<<", " <<lip_r->y<<", " <<lip_r->width <<", " <<lip_r->height<<endl;
  
  }


  cvResetImageROI(img);

  if (mouth->total>=1)
    {
      
    //The obtained lip position w.r.t the original image is as folows: 
 
  lip_r_out.x=r2->x+lip_r->x;
  lip_r_out.y=r2->y+lip_r->y;
  lip_r_out.width=lip_r->width;
  lip_r_out.height=lip_r->height;

    }

  else
    {

      std::cout<<"no mouth is detected !"<<std::endl;
      return;
    }
  // cvResetImageROI(gray_mouth_img);
  // cvDestroyWindow("mouth");
  return;
}// End if the function for mouth detection.


//=================================================================================
//=================================================================================
//function to savethe corresponding eye images into files.
void save_eyes_images(IplImage *img, CvRect& r_i0, CvRect& r_i1, CvRect *face_rect,CvRect *eye1_rect, CvRect *eye2_rect )
{
  // CvRect *eye1_rect = new CvRect();
  // CvRect *eye2_rect = new CvRect();

  IplImage *eye1;
  IplImage *eye2;
  //rectangle enclosing the 1st eye. The rectangle is w.r.t the whole image. 
  eye1_rect->x=face_rect->x+r_i0.x;
  eye1_rect->y=face_rect->y + face_rect->height/5.5+r_i0.y;
  eye1_rect->width=r_i0.width;//width of the eye should be same.
  eye1_rect->height=r_i0.height;//height of the eye should remain same.

  //rectangle enclosing the 2nd eye w.r.t the original image
  eye2_rect->x=face_rect->x+r_i1.x;
  eye2_rect->y=face_rect->y + face_rect->height/5.5+r_i1.y;
  eye2_rect->width=r_i1.width;//width of the eye should be same.
  eye2_rect->height=r_i1.height;//height of the eye should remain same.

  std::cout<<"face rect "<<face_rect->x<<"\t"<<face_rect->y<<"\t"<<face_rect->height<<"\t"<<face_rect->width<<std::endl;
  //**************
  //  cout<<"eye rect data "<< eye1_rect->x<<", "<<eye1_rect->y<< "," <<r_i0.width << "," << r_i0.height<<endl;
  eye1 = cvCreateImage( cvSize(eye1_rect->width, eye1_rect->height),img->depth, 3 );
  eye2 = cvCreateImage( cvSize(eye2_rect->width, eye2_rect->height),img->depth, 3 );

  std::cout<<"eye rect "<<eye1_rect->x<<"\t"<<eye1_rect->y<<"\t"<<eye1_rect->height<<"\t"<<eye1_rect->width<<std::endl;

  // cvCopy( img, roi_gray,NULL ); 

  cvSetImageROI(img,*eye1_rect);

  cvCopy( img, eye1,NULL ); 
  //Reset the image back to its original size
  cvResetImageROI(img);


  // cvNamedWindow("eye1", CV_WINDOW_AUTOSIZE);

  // cvShowImage("eye1",eye1);

  // cvWaitKey(wait_ms);
  show_image(eye1, wait_ms);

  //set the image ROI for 2nd eye 
    
  cvSetImageROI(img,*eye2_rect);

  cvCopy( img, eye2,NULL ); 
  //Reset the image
  cvResetImageROI(img);
  // show image eye 2
  // cvNamedWindow("eye2", CV_WINDOW_AUTOSIZE);

  // cvShowImage("eye2",eye2);

  // cvWaitKey(wait_ms);
  show_image(eye2, wait_ms);
  
  //save the image eye1 and eye 2

  cvSaveImage( "eye1.jpeg", eye1 );
  cvSaveImage( "eye2.jpeg", eye2 );

  

  cvReleaseImage(&eye1);
  cvReleaseImage(&eye2);
  //cvReleaseImage(&eye2);
  // cvDestroyWindow("eye1");
  // cvDestroyWindow("eye2");
}

//======================================================================//
/*function to detect eye pupil. It loads the coloured image, gets the HSV transformation. Extract the Saturation (S) Image. Saturation image gives a good information
  about the eye pupil. as the middle portion is black. The contour of the middle portion is extracted by thhresholding followed by enclosing Rectangle wiith in the
  contour.  The centroid of the rectangular box gives the eye pupil. */

//Function starts here---------------------------------------------


void  detectEyes_pupil(IplImage *img, CvPoint& eye_pupl_cnt, IplImage *t_img )
{
 
 
  cvSmooth(img, img, CV_GAUSSIAN,  3, 0, 0, 0); //smooth the image
  // Compute the HSV image and decompose it into separate planes.
  //
  IplImage* hsv = cvCreateImage( cvGetSize(img), 8, 3 );
  cvCvtColor( img, hsv, CV_RGB2HSV );
  /*(source,destination*/

  IplImage* h_plane = cvCreateImage( cvGetSize(img), 8, 1 );
  IplImage* s_plane = cvCreateImage( cvGetSize(img), 8, 1 );
  IplImage* v_plane = cvCreateImage( cvGetSize(img), 8, 1 );
  //IplImage* t_img = cvCreateImage( cvGetSize(img), 8, 1 );
 
  //Split out hue component and store in h_plane
  cvSplit(hsv, h_plane,s_plane,v_plane, 0);
  // cvEqualizeHist( h_plane, h_plane );
  //  cvEqualizeHist( s_plane, s_plane );
  //threshold the hue plane image
 
  cvSaveImage( "h_plane.jpeg",h_plane ); 
  //s_plane is working fine for ritu image and h_plane is working fine with jk image. for both threshold is taken =80
  // use adaptive thresholding

  // //***********************************************************************//
  // float threshold_value=0; 
  // int k=0;//k value is 5 
  // //use adaptive thresholding 
  //  adaptive_threshold(s_plane, threshold_value, t_img, k);

  // //***********************************************************************//  

  // cvThreshold( h_plane, t_img, 80, 255,CV_THRESH_BINARY );
    cvThreshold( h_plane, t_img, 60, 255,CV_THRESH_BINARY );// only for ritu image
    // cvNamedWindow("HSV", CV_WINDOW_AUTOSIZE);

  cvSaveImage( "t_img.jpeg",t_img ); 


  // cvShowImage("HSV",h_plane);
  // cvWaitKey(wait_ms);

  show_image(h_plane, 0);


  //  cvThreshold( v_plane, t_img, 80, 255,CV_THRESH_BINARY_INV);
  // cvErode(t_img, t_img, 0, 1);
  //dilation is done to remove the cracks.  
  // cvDilate(t_img, t_img, 0, 2);
  // cvErode(t_img, t_img, 0, 2);
 

  //Get the rectangle which contains the white region 
  //obtain the pupil center by obtaining the bounding rectangle and then calculating the  center of the rectangle.
  CvRect contour_rect1, contour_rect2;
  int number_of_contour=0; 
  find_contour_rect(t_img, contour_rect1, contour_rect2, number_of_contour);
 

  //CvRect r1 = cvBoundingRect(t_img, 0); 
  CvRect r1 = contour_rect1; 
  //  cout<<"The pupil -rectangular region :"<<r1.x<<","<<r1.y<<","<<r1.width<<","<<r1.height<<endl;
   // CvPoint    eye_cnt ;
  //eye center is w.r.t the eye image only. 
  eye_pupl_cnt.x=r1.x+r1.width/2;
  eye_pupl_cnt.y=r1.y+r1.height/2;
  // cout<<"eye pupil center is :"<<eye_pupl_cnt.x <<","<<eye_pupl_cnt.y<<endl;

  ///draw a circle on the eye with the obtained eye pupil center.
  //  cvCircle( img, eye_pupl_cnt, 4, CV_RGB(255,0,0), 0.5, CV_AA, 0 );

  cvReleaseImage (&h_plane);
  cvReleaseImage (&s_plane);
  cvReleaseImage (&v_plane);
  // cvReleaseImage (&t_img);
  return;
}


// // end of eye_pupil detection function ===========================================
//=================================================================================


/********function to detect the eye corner points.. It is obtained by using the good feature to track function..
	 the function obtains all the good corner points that can be tracked nicely. 
	 The left and right eye corner is obtained by taking the max distance corner point from the eye pupil center.*/ 

//--------Corner detection check----------------//

int CornerDetection(IplImage* src, int size_window, CvPoint& left_corner_feature,CvPoint& right_corner_feature,  CvPoint& origin){
 
  int MAX_CORNERS = 50;
  CvPoint2D32f* src_features_CD = new CvPoint2D32f[ MAX_CORNERS ];
  CvSize src_size = cvGetSize(src);
  IplImage* eig_image = cvCreateImage(src_size, IPL_DEPTH_32F, 1);
  IplImage* temp_image = cvCreateImage(src_size, IPL_DEPTH_32F, 1);
  int number_of_corners = 0;
  IplImage *histEq_img= cvCreateImage( cvGetSize(src), 8, 1 );
  IplImage*  gray_img = cvCreateImage( cvGetSize(src), 8, 1 );
   //*********************************************************************************************************************//
  //modified on 10_02_2011----anima---------------------------------------------------------------------------------------//
  //gray image is the intensity image needed to be used for corner detection function
   cvCvtColor(src, gray_img, CV_RGB2GRAY);
   // cvEqualizeHist( gray_img, histEq_img );
  
  // sudo_hue(gray_img);// don't call this 
   cvEqualizeHist( gray_img, gray_img );

  //call the adaptive threshold function here.
  //**********************************************************************************************************************//
  
   // float threshold_value=0; 
   // int e_k=0;//k value is 5 
  //use adaptive thresholding 
  // adaptive_threshold(gray_img, threshold_value, histEq_img, e_k);

  // use simple threshold technique
 cvThreshold(gray_img,histEq_img,60,255,CV_THRESH_BINARY);



  // cvNamedWindow("hist_image", CV_WINDOW_AUTOSIZE);
  // cvShowImage("hist_image",histEq_img);
  // cvWaitKey(wait_ms);

 show_image(histEq_img, wait_ms);
  //********************************************************************************************************************// 



  //the function uses gray image 
  // cvGoodFeaturesToTrack(histEq_img, eig_image, temp_image, src_features_CD, &number_of_corners, .1, size_window/5, NULL);
  cvGoodFeaturesToTrack(gray_img, eig_image, temp_image, src_features_CD, &number_of_corners, .1, size_window/5, NULL);

   
  printf( "\nNumber of corner features found = %d\n",number_of_corners );
  /*
    calculate the distance between the corner point found and the eye pupil center. before calculating check if the 
    corner point left is the pupil or right of the pupil. the max of the points belongs to the left of the pupil is 
    is considered as the left corner point and the max of the points belongs to the right of pupil is considered as
    right corner point. Right now only distance along x direction is taken into consideration. Y distance of each orner w.r.t origin is 
    calculated to check that the corner point should not exceed certain limit.We know that the corners of the eye does has certain
    limited y- distance from the pupil origin.Here some contrain is provided in the y -axis distance. i.e the corner point found should 
    not exceed the range of the y distance provided. then only it will be considered as eye corner point. 
  */
  double left_distance[number_of_corners];
  double right_distance[number_of_corners];
  double  distance[number_of_corners];
  double left_y_distance[number_of_corners];
  double right_y_distance[number_of_corners];
 
 
  int cr, k;
  // k is used to initialize the data. Left distance is the +ve distance as it is calculated from the origin of eyes. Similarly right
  //distance. 
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
	{
	  left_distance[cr]=distance[cr];
	  left_y_distance[cr]=origin.y-src_features_CD[cr].y;
	}
      else
	{   
	  right_distance[cr]=distance[cr];
	  right_y_distance[cr]=origin.y-src_features_CD[cr].y;

	}	   
    }



  while(cr>0)
    {
      // cout<<"left corner points distance are : "<<left_distance[cr]<<"at the index "<<cr<<endl; 
      //   cout<<"right corner points distance are : "<<right_distance[cr]<<endl; 
      // check if the distance is maximum provided the y coordinate of the corner is not more +3 and -8 from the origin. 
      if (left_distance[cr]>left_max)
	{
	  if (left_y_distance[cr]<3&&left_y_distance[cr]>-8)
	    {
	      left_max=left_distance[cr];
	      left_index=cr;
	      // cout<<"left y dist"<<left_y_distance[cr]<<endl;	   
	    }
	}
      else
	left_max=left_max;
     
      if (right_distance[cr]<right_max)
	{
	  if(right_y_distance[cr]<3&&right_y_distance[cr]>-8){
	    right_max=right_distance[cr];
	    right_index=cr;
	    //  cout<<"right y dist"<<right_y_distance[cr]<<endl;
	  }
	}
      else
	right_max=right_max;
      cr--;
     
    }

  // cout<<"left max ="<<left_max<<"at the index "<<left_index<<endl;
  // cout<<"right max ="<<right_max<<"at the index"<<right_index<<endl;
  //  left max coordinate point is :
  left_corner_feature.x=src_features_CD[left_index].x;
  left_corner_feature.y=src_features_CD[left_index].y;
  right_corner_feature.x=src_features_CD[right_index].x;
  right_corner_feature.y=src_features_CD[right_index].y;

 
  //     distance[cr] = EuclideanDistance(featuresexp, src_features_CD[cr]);
 
 
  cvCircle( src, left_corner_feature, 4, CV_RGB(150,255,0), 0.5, CV_AA, 0 );
  cvCircle( src, right_corner_feature, 4, CV_RGB(150,255,0), 0.5, CV_AA, 0 );
   
 
 
  cvReleaseImage(&eig_image);
  cvReleaseImage(&temp_image);
  cvReleaseImage(&gray_img);
  cvReleaseImage (&histEq_img);
  delete [] src_features_CD;
  // cvDestroyWindow("hist_image");
  return number_of_corners;
}// end of the function corner detection

//==================================================================================
//---------------------------------------------------------------------------------

/* the function below finds the contour enclosing the white object. Then it obtains the 
   rectangular box containing the contour and return it's coordinate values. */


void find_contour_rect(IplImage * img, CvRect& obj_rect1, CvRect& obj_rect2, int& cts)
{
  CvSeq* contour = NULL;
  CvMemStorage* storage = cvCreateMemStorage(0);
  // CvRect object;

  // int cts = cvFindContours(img, storage, &contour,
  // 			   sizeof(CvContour),
  // 			   CV_RETR_CCOMP,
  // 			   CV_CHAIN_APPROX_SIMPLE);
  



  cts = cvFindContours(img, storage, &contour);
  cout<<"function find contour rect"<<"number of contours detected : "<<cts<<endl;
  // cvClearMemStorage(storage);
  
  if(contour){

    obj_rect1 = cvBoundingRect(contour,1);
    if (cts>1){
      contour = contour->h_next;
      obj_rect2 = cvBoundingRect(contour,1);
    }

    else
      std::cout<<" second contour is not detected"<<std::endl;

    
  }

  else
    {
      std::cout<<"no contour detected in function find_contour_rect!"<<std::endl;
    
    return;

    }
  // cout<<"box containing contour"<< obj_rect1.x<<", "<< obj_rect1.y<<","  <<obj_rect1.width<<" ," <<obj_rect1.height<<endl;

  //clear the sequence 
 

  cvClearMemStorage(storage);
  cvReleaseMemStorage(&storage);
  cvClearSeq( contour);

  return;
}// end of fine contour function 
//======================================================================================
// starting of nose detection function 
void detect_nose(IplImage *img, CvRect *face_rect, CvRect& nose_rect, CvRect& nose_estm)
{
  //can set the estimated position of nose.  
  cvClearMemStorage( storage );
  /* Set the Region of Interest: estimate the lip position */
  // cvSetImageROI(img, cvRect(face_rect->x, face_rect->y , face_rect->width, face_rect->height));
  cvSetImageROI(img, cvRect(nose_estm.x, nose_estm.y , nose_estm.width, nose_estm.height));

   
  CvSeq* nose = cvHaarDetectObjects( 
				    img, cascade_n, storage,
				    1.1, 2, CV_HAAR_DO_CANNY_PRUNING, cvSize(40, 40));
 


 
 
  CvRect *r_nose;

  if (nose->total>0)
    {
  for(int i = 0; i < (nose ? nose->total : 0); i++ ) {
    cout << __FILE__ << " " << __FUNCTION__ << " " << __LINE__ << endl;
    cout<<i+1<<" nose _detected"<<endl;
    r_nose= (CvRect*)cvGetSeqElem( nose, i );
    
    // cvRectangle(img, 
    // 		cvPoint(r_nose->x, r_nose->y), 
    // 		cvPoint(r_nose->x + r_nose->width, r_nose->y + r_nose->height),
    // 		CV_RGB(255, 180, 150), 1, 8, 0);

    // cvNamedWindow("nose", CV_WINDOW_AUTOSIZE);

    // cvShowImage("nose",img);

    // cvWaitKey(wait_ms);

    show_image(img, wait_ms);

  }

 
  //estimate the nose position with respect to the whole image is 
  // which is equivalent to the origin of face image rect + nose image rectangle , width and height remains same. 
  // nose_rect.x=face_rect->x+r_nose->x;
  // nose_rect.y=face_rect->y+r_nose->y;  
  // nose_rect.width=r_nose->width;
  // nose_rect.height=r_nose->height;
  //anima-----------25th dec---------------detected Nose location w.r.t the whole image.  
  nose_rect.x=nose_estm.x+r_nose->x;
  nose_rect.y=nose_estm.y+r_nose->y;  
  nose_rect.width=r_nose->width;
  nose_rect.height=r_nose->height;

    }

  else

    {
      std::cout<<"nose is not detected in this frame. previous estimated nose location is taken here:"<<std::endl;
      nose_rect.x=nose_estm.x;
      nose_rect.y=nose_estm.y;  
      nose_rect.width=nose_estm.width;
      nose_rect.height=nose_estm.height;
    }
  

  cvResetImageROI(img);
  // cvResetImageROI(gray_mouth_img);
  // cvDestroyWindow("nose");
  return;
}// End if the function for mouth detection.
//============================================================================================
//============================================================================================


//Function to locate the nostril points. 
void locate_nostile(IplImage *img,CvRect& nose_rect, CvPoint& l_nostril, CvPoint& r_nostril)
{
  /* Set the Region of Interest: estimate the nose position */
  cvSetImageROI(img, cvRect(nose_rect.x, nose_rect.y , nose_rect.width, nose_rect.height));
 
  /* copy the image for further processing. Can be converted to a gray scale image  */ 

  IplImage*  gray_nose_img = cvCreateImage( cvGetSize(img), 8, 1 );
  //  //gray image is the intensity image needed to be used for contour detection function
  cvCvtColor(img, gray_nose_img, CV_RGB2GRAY);
  //get the contour of the nostrils. Thresholding is done before passing the image into the contour function. 
  //threshold the image to get two nice black region containg the nostrils. 
  // cvThreshold( gray_nose_img, gray_nose_img,80, 255,CV_THRESH_BINARY );// works fine with ritu's image
  // for jk image use threshold value=5
  cvSaveImage( "nose_gray.jpeg", gray_nose_img );
  cvEqualizeHist( gray_nose_img, gray_nose_img );//don't use it for a good contrast image i.e jk image
  cvThreshold( gray_nose_img, gray_nose_img,10, 255,CV_THRESH_BINARY );// threshokd 5 is used for both the image
  // use adaptive thresholding method 
  // cvAdaptiveThreshold( img, gray_nose_img, 255,CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 3,1 );
  // cvDilate(gray_nose_img, gray_nose_img, 0, 1);
  cvSaveImage( "nose.jpeg", gray_nose_img );  
  cvErode(gray_nose_img, gray_nose_img, 0, 2);
  cvDilate(gray_nose_img, gray_nose_img, 0, 1);

 
 // cvNamedWindow("nose_r", CV_WINDOW_AUTOSIZE);

  // cvShowImage("nose_r",gray_nose_img);

  // cvWaitKey(wait_ms);

  show_image(gray_nose_img, wait_ms);

  //call the function find_contour_rect to get both the nostril contours.  

  CvRect contour_rect1, contour_rect2;
  int number_of_contour=0;
  find_contour_rect(gray_nose_img, contour_rect1, contour_rect2, number_of_contour);
  // cout<<"the nostril 1 rect is " <<contour_rect1.x<<"," <<contour_rect1.y<<endl;
  // cout<<"the nostril 2 rect is " <<contour_rect2.x<<"," <<contour_rect2.y<<endl;
  CvPoint cnt1;
  CvPoint cnt2;

  if(number_of_contour>1)
    {
  cnt1.x=contour_rect1.x+contour_rect1.width/2;
  cnt1.y=contour_rect1.y+ contour_rect1.height/2;
  draw_circle(img,cnt1);

 
  cnt2.x=contour_rect2.x+contour_rect2.width/2;
  cnt2.y=contour_rect2.y+ contour_rect2.height/2;
  draw_circle(img,cnt2);
    }

  else

    {
      cnt1.x=contour_rect1.x+contour_rect1.width/2;
      cnt1.y=contour_rect1.y+ contour_rect1.height/2;
      draw_circle(img,cnt1);

 
      cnt2.x=contour_rect1.x+2*contour_rect1.width/2;// twice the
						     // contour width 
      cnt2.y=contour_rect1.y+ contour_rect1.height/2;
      draw_circle(img,cnt2);



    }
    

  //check which one is left nostril and which one is right.here left is , left w.r.t of the image. 
  if((cnt2.x-cnt1.x)>0){
    // Calculate the nostril points location  w.r.t the original image 
    l_nostril.x=nose_rect.x+cnt1.x;
    l_nostril.y=nose_rect.y+cnt1.y;
    r_nostril.x=nose_rect.x+cnt2.x;
    r_nostril.y=nose_rect.y+cnt2.y;
  }

  else{
    l_nostril.x=nose_rect.x+cnt2.x;
    l_nostril.y=nose_rect.y+cnt2.y;
    r_nostril.x=nose_rect.x+cnt1.x;
    r_nostril.y=nose_rect.y+cnt1.y;
  
  }

  // cvNamedWindow("nose_r", CV_WINDOW_AUTOSIZE);

  // cvShowImage("nose_r",gray_nose_img);

  // cvWaitKey(wait_ms);
 

 

  /* reset the image after the work is done. */
  cvResetImageROI(img);


  // cvDestroyWindow("nose_r");
  cvReleaseImage(&gray_nose_img);
  return;
}


//============================================================================================================================//


void draw_rectangle(IplImage* img, CvRect &rect, int r, int g, int b){
  // choose the r, g and b values . Ranges of all is between 0-255 
  cvRectangle(img,
  	      cvPoint(rect.x, rect.y),
  	    
  	      cvPoint(rect.x+rect.width, rect.y +rect.height),
			
  	      CV_RGB(r, g, b), 1, 8, 0);
  //can obtain any colour combination by changing RGB values. 
  

}

void detect_lip_all_points(IplImage* img, CvRect& lip_rect, CvPoint &lip_upper_mid,CvPoint &lip_lower_mid, CvPoint &left_lip_corner,CvPoint &right_lip_corner )

{
  //  cout<<"lip rect is :"<<lip_rect.x<<","<<lip_rect.y<<","<<lip_rect.width<<","<<lip_rect.height<<endl;


/* set the image ROI */
  cvSetImageROI(img, cvRect(lip_rect.x, lip_rect.y , lip_rect.width, lip_rect.height));

 // Call the lip mid points detection function here  function here
  cvSaveImage("lip_test.jpeg",img);
  //check why code is not working for img as input data to the function. Same is working for reading from a file

  IplImage* input_img = cvLoadImage("./lip_test.jpeg", 1 );
  IplImage *thres_img= cvCreateImage( cvGetSize(input_img), 8, 1 );
     
 // cvNamedWindow("lip", CV_WINDOW_AUTOSIZE);

 //  cvShowImage("lip",img);

 //  cvWaitKey(wait_ms);

  show_image(img, wait_ms);

  CvSeq* lip_c=0;
  
  
  // //the function to detect the lip mid points
  //  detect_lip(input_img,thres_img, lip_c, lip_upper_mid, lip_lower_mid);

detect_lip(input_img, thres_img, lip_rect, lip_c,  lip_upper_mid, lip_lower_mid,  left_lip_corner,right_lip_corner );


  /* reset the image after the work is done. */
  cvResetImageROI(img);

  //*****************************************************//
  //calculate the uuper mid point of the lip and the lower mid point of the lip as follows:

//We know the lip rectangle w.r.t the original image which is lip_rect here. so the two points will be as follows.
  //upper lip points 
  lip_upper_mid.x=lip_rect.x+lip_upper_mid.x;
  lip_upper_mid.y=lip_rect.y+lip_upper_mid.y;

  // lower lip point
  lip_lower_mid.x=lip_rect.x+lip_lower_mid.x;
  lip_lower_mid.y=lip_rect.y+lip_lower_mid.y;

  //*****************************************************//



 // cvShowImage("lip",img);

 //  cvWaitKey(wait_ms);

  cvSaveImage("lip_test.jpeg",img);

  cvReleaseImage(&thres_img);
 cvReleaseImage(&input_img); 
 // cvDestroyWindow("lip");
  return;

}//end of the function called
//=====================================================================================================================//


void save_image_roi (IplImage* input, CvRect& roi)

{

 /* set the image ROI */
  cvSetImageROI(input, cvRect(roi.x, roi.y , roi.width, roi.height));
   
 
  // cvNamedWindow("lip", CV_WINDOW_AUTOSIZE);

  // cvShowImage("lip",img);

  // cvWaitKey(wait_ms);
  //some problem is here while saving the lip image
 
 cvSaveImage( "lip.jpeg", input );


 

 cvResetImageROI(input);

return;
}

