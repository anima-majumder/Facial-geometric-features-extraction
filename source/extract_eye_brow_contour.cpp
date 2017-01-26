
//Get the eye brow image
//Obtain the threshold image
//get the extreme corner points 
//along the center of eye (along y axis) ontain the extreme corner points 

//============================================================================//

#include"extract_eye_brow_contour.h"
#include "global.h"

//=========================================================================//
void extract_eye_brow_contour(IplImage *input, IplImage *contour_img, int& k, char* name, char* contour_name, char* brow_h_plane_name,  CvRect &eye_brow_r,CvPoint &brow_left, CvPoint &brow_upper_mid, CvPoint &brow_lower_mid,  CvPoint &brow_right)

{
  // cvNamedWindow("window", CV_WINDOW_AUTOSIZE);

 
  //********This part is for testing if eyebrow image can be enhanced by equalizing the brighness****************//
  IplImage* hsv = cvCreateImage( cvGetSize(input), 8, 3 );
  cvCvtColor( input, hsv, CV_RGB2HSV );
  /*(source,destination*/

  IplImage* hue_plane = cvCreateImage( cvGetSize(input), 8, 1 );
  IplImage* sat_plane = cvCreateImage( cvGetSize(input), 8, 1 );
  IplImage* v_plane = cvCreateImage( cvGetSize(input), 8, 1 );
  //IplImage* t_img = cvCreateImage( cvGetSize(img), 8, 1 );
 
  //Split out hue component and store in h_plane
  cvSplit(hsv, hue_plane,sat_plane,v_plane, 0);
  //cvEqualizeHist( h_plane, h_plane );
  cvEqualizeHist( v_plane, v_plane );

  cvMerge(hue_plane, sat_plane, v_plane, NULL, hsv);
  cvCvtColor(hsv, input, CV_HSV2RGB);

  


  cvReleaseImage(&hsv); 
  cvReleaseImage(&hue_plane); 
  cvReleaseImage(&sat_plane); 
  cvReleaseImage(&v_plane); 

  //**********above equalization part*****************************//


 
  //====================This part of the code is not used now=============// 
  //----------------------------------------------------------------------// 
  //Extract hue plane 
  // cvSmooth(input, input, CV_GAUSSIAN,  3, 0, 0, 0); 
  cvSmooth( input, input, CV_GAUSSIAN, 7, 7 );

  //==================================================================//
  // Compute the HSV image and decompose it into separate planes.
  // IplImage* hsv = cvCreateImage( cvGetSize(input), 8, 3 );
  // cvCvtColor( input, hsv, CV_RGB2HSV );
  /*(source,destination*/
  IplImage* h_plane = cvCreateImage( cvGetSize(input), 8, 1 );

  sudo_hue(input, h_plane, brow_h_plane_name);//obtains the sudo hue plane 
 
  //-----------------------------------------------------------------------//
  //=======================================================================//
  //Extract the gray image of the input image which is actually a color image
  IplImage* gray_img = cvCreateImage( cvGetSize(input), 8, 1 ); 
  IplImage* thres_img = cvCreateImage( cvGetSize(input), 8, 1 ); 
  cvCvtColor(input, gray_img, CV_RGB2GRAY);
  // IplImage* eye_brow_thres = cvCreateImage( cvGetSize(input), 8, 1 );  
 
  invert_img(gray_img);// gray plan does not work properly


    // cvShowImage("window",h_plane);
    // cvWaitKey(wait_ms);
 

  show_image(h_plane, wait_ms );//default wait value is 10 ms
  // Threshold the image 
  //  cvThreshold(gray_img, thres_img, 180, 255,CV_THRESH_BINARY );
  //  cvThreshold(h_plane, thres_img, 40, 255,CV_THRESH_BINARY );// for sudo image 
  // //====================================================================================//
  // //***********************************************************************//
  float threshold_value=0; 
  
  // //use adaptive thresholding 
  // adaptive_threshold(gray_img, threshold_value, thres_img, k);
  adaptive_threshold(h_plane, threshold_value, thres_img, k);
  cvSmooth( thres_img, thres_img, CV_GAUSSIAN, 3, 3 );
  // cvShowImage("window",thres_img);
  // cvWaitKey(wait_ms);
  show_image(thres_img, wait_ms );//default wait time is 10 ms
  
  // adaptive_threshold(h_plane, threshold_value, thres_img, k);
  // cout<<"threshold value is "<<threshold_value<<endl;
  //cout<<"name is "<<name<<endl;
  //***********************************************************************//  
  // //=====================================================================================//
  // cvErode(thres_img, thres_img, 0, 3);
  //cvDilate(thres_img, thres_img, 0,5); //5
  // cvErode(thres_img, thres_img, 0, 3);//erosion should be followed by dilation not vice versa 
   
  //  cvErode(thres_img, thres_img, 0, 3);
  //  cout<<"char is "<<url<<endl;
  // cvSaveImage("eye_brow_thres.jpeg",thres_img ); 

  // cout<<"name is "<<name<<endl;

  cvSaveImage(name,thres_img ); 
  //copy the image in another file 
  cvCopyImage(thres_img, contour_img);
   
  // cvSaveImage( "eye_brow_thres.jpeg",thres_img ); 

  //**************************************************************************//
  //find the eye brow contour================================================//
  CvSeq* contour = 0;
  CvSeq* largest_contour=NULL;
  CvMemStorage* storage = cvCreateMemStorage(0);
  // CvPoint brow_upper_mid, brow_lower_mid,  brow_left, brow_right;
  //*************this part is not in use***** some error*******************//

  // find_largest_contour(thres_img,  contour, largest_contour, storage);
  //   draw_contour(gray_img, largest_contour);
  //**********************************************************************//

  // // //************************************========================***************************//
  int count=0;
  int contour_count=0;

  count =  cvFindContours( thres_img, storage, &contour, sizeof(CvContour),
  			   CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );

  std::cout<<"number of contours detected is: "<<count<<std::endl;
 
 

   if( contour )//find the largest contour
     {

       if (count==1)
	 largest_contour=contour;
       else
	 {	 
      double area, max=0;
      for( ; contour != 0; contour = contour->h_next ){
      	area = cvContourArea( contour , CV_WHOLE_SEQ );

      	if (area > max ) 
      	  {max = area;
	  
      	    largest_contour=contour;

      	  }
   
    
      }

	 }
      //draw the largest contour into the gray scale image
      cvDrawContours(
		     gray_img,
		     largest_contour,
		     cvScalarAll(255),
		     cvScalarAll(255),
		     -1,

		     1,
		     8,
		     cvPoint(0,0)
		     );
 


      //**************************read the coordinate values of the sequence element for largest_contour**************************//
      CvPoint  pt; 
      CvSeqReader reader;
      CvPoint point_array[largest_contour->total];
      
      cvStartReadSeq( largest_contour, &reader, 0);

     
      for (int i=0; i<largest_contour->total;i++)
	{
	  CV_READ_SEQ_ELEM(pt, reader);
	  printf("x = %d y= %d\n", pt.x, pt.y);

	}
      //********************************************************************//
      //convert seq to array ********//
      // CvPoint* point_array;  
      //fprintf(stderr,"malloc\n");  
      // Alloc memory for contour point set.  
    


      // point_array = (CvPoint*)malloc( count*sizeof(CvPoint) );  

      // Get contour point set.  
      //fprintf(stderr,"cvCvtSeqToArray\n");  
      cvCvtSeqToArray(largest_contour, point_array, CV_WHOLE_SEQ);  

      //   cout<<"seq array element are :"<<endl;

      for(int j=0; j<largest_contour->total; j++)
		cout<<point_array[j].x<<"\t"<<point_array[j].y<<endl;

      // ****** call the function here to obtain all the contours of eyebrow the extract the largest contour from it. ****************//
      //  find_largest_contour(thres_img,  contour, largest_contour, storage);
      //   draw_contour(gray_img, largest_contour);
      //   cout<<contour<<endl;
      // if(contour){
      //======================================================================//
      //********************************************************************* **//
      //**********call the snake algo here to get the snake eyebrow contour****//
      // float alpha = 0.1;
      //  float beta = 0.01;
      //  float gamma =0.1;
  
      // snake_algo( gray_img, contour,  gray_img, alpha, beta, gamma ); 

      //  cout<<"save contour image in fine name as :"<<contour_name<<endl; 
  //***********After finding all the contour points obtain the two extreme corner points and two middle points of eyebrow. which is done as follows:
  /* For two extrme points find out the points for which x coordinates are largest and smallest. 
     For two middle points: find x such that x is the mid point of two extreme x coordinate and y are corresponding y values for that x. If that x does not exists find out the nearest x coordinate. The upper middle y is less y value and lower y for middle point is more y value  */


      //******find the bounding rectangle of the contour ************//
      CvRect boundbox;
      boundbox = cvBoundingRect(largest_contour);
      CvPoint center;
      center.x=(boundbox.x + boundbox.width)/2;
      center.y=(boundbox.y + boundbox.height)/2;
      float constraint_height=boundbox.height/5;
      bool flag=false;
      //************end of bounding rectangele***********************//

   contour_count=largest_contour->total;
     //call the function of eyebrow point location here:
   locate_pts( contour_count, point_array,center, constraint_height, brow_upper_mid,brow_lower_mid, brow_left, brow_right, flag);
  //  cout<<"eyebrow points located are :"<<"\t"<<brow_upper_mid.x<<"\t"<<brow_upper_mid.y<<"\t"<<brow_lower_mid.x<<"\t"<<brow_lower_mid.y<<"\t"<<brow_left.x<<brow_left.y<<brow_right.x<<brow_right.y<<endl; 

  draw_circle(gray_img,brow_upper_mid);
  draw_circle(gray_img,brow_lower_mid);
  draw_circle(gray_img,brow_left);
  draw_circle(gray_img,brow_right);
  // cvShowImage("window",gray_img);
  //     cvWaitKey(0);
 
  show_image(gray_img,wait_ms );

      // update the eyebrow contour points w.r.t the whole face image. *******************************//

      brow_left.x = eye_brow_r.x + brow_left.x;
      brow_left.y = eye_brow_r.y + brow_left.y;

      brow_right.x = eye_brow_r.x + brow_right.x;
      brow_right.y = eye_brow_r.y + brow_right.y;

      brow_upper_mid.x = eye_brow_r.x + brow_upper_mid.x;
      brow_upper_mid.y = eye_brow_r.y + brow_upper_mid.y;

      brow_lower_mid.x = eye_brow_r.x + brow_lower_mid.x;
      brow_lower_mid.y = eye_brow_r.y + brow_lower_mid.y;

      //***********updation of eyebrow points w.r.t whole face image is done here. 


 }// end of if contour loop

   
  //  cvSaveImage( "eye_brow_contour.jpeg",gray_img );
  cvSaveImage( contour_name, gray_img );
  //release all memory
  // Free memory.            
  //free ( point_array ); 
  std::cout<<"no prob in extract_eye_brow_contour file "<<std::endl;
  cvReleaseImage(&gray_img);
  // cvReleaseImage(&hsv);
  cvReleaseImage(&h_plane);
  cvReleaseImage(&thres_img);
  // cvDestroyWindow("window");

  cvClearMemStorage(storage);
  return;
}





