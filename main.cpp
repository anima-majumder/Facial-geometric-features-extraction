// This program is the main program for detection of smile in a video. 
#include <cstdio>
#include <cv.h>
#include <highgui.h>
#include <cmath>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
#include <iostream>
#include "utils.h"
#include "global.h"
#include "face_alignment.h"

#include "my_lib.h"
using namespace std;


//declare the global variables here
CvHaarClassifierCascade *cascade_f;
CvHaarClassifierCascade *cascade_e;
CvHaarClassifierCascade *cascade_m;
CvHaarClassifierCascade *cascade_n;
CvMemStorage		*storage;

char filename1[]="eye1_projection_ratio.txt";
char filename2[]="eye2_projection_ratio.txt";

unsigned const wait_ms = 10; //external variable defined in global.h
float x_scale=0;
float y_scale=0;
float scale=0.0;
double const stand_eye_space=70; //x-distance between right and left eye

CvRect eye1r0_prev = cvRect (0, 0, 0, 0);
CvRect eye2r1_prev = cvRect (0, 0, 0, 0);
CvRect nose_prev = cvRect (0, 0, 0, 0);
CvPoint2D32f* lip_position_prev;
CvRect* lip_rect_prev;
CvRect* face_rect_prev;

// //declare the functions here
/******************************************************************************/

//using namespace std;
//Declare some global variables for the function automatic featues initialization 

string num2str(const unsigned& num)
{
  stringstream ss;
  ss << num;
  return ss.str();
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

inline static double square(int a)
{
  return a * a;
}

inline static void allocateOnDemand( IplImage **img, CvSize size, int depth, int channels)
{
  if ( *img != NULL ) return;
  *img = cvCreateImage( size, depth, channels );
  if ( *img == NULL )
    {
      fprintf(stderr, "Error: Couldn't allocate image. Out of memory?\n");
      exit(-1);
    }
}

double EuclideanDistance(CvPoint2D32f features1, CvPoint2D32f features2){
  return sqrt(square(features1.x - features2.x) + square(features1.y - features2.y));
}// End of the function ==========================================================================================//
//****************************************************************************************************************//


//***********************************************************************************************************//
void BinarizeROI(IplImage* temp, int threshold, CvPoint2D32f feature, int window_h_half, int window_w_half){
  CvScalar s;
  for(int i=-window_h_half; i < window_h_half; i++){
    for(int j=-window_w_half; j < window_w_half; j++){
      //printf("GourangaBIN %d  %d\n", (int)feature.y + i,(int)feature.x + j);
      s = cvGet2D(temp,(int)feature.y + i,(int)feature.x + j); // get i,j pixel value
      
      if(s.val[0]>threshold) s.val[0] = 254;
      else s.val[0] = 0;
      cvSet2D(temp,(int)feature.y + i,(int)feature.x + j,s);
    }
  }
  
  //	cvShowImage("Krishna", temp);
  return;
  
}// End of the function ***********************************************************************************************//
//*********************************************************************************************************************//

void Binarize(IplImage* temp, int threshold){
  CvScalar s;
  for(int i=0; i < temp->height; i++){
    for(int j=0; j < temp->width; j++){
      s = cvGet2D(temp,i,j); // get i,j pixel value
      if(s.val[0]>threshold) s.val[0] = 154;
      else s.val[0] = 0;
      cvSet2D(temp,i,j,s);
    }
  }
  
  return;
  
}//End of the function ***********************************************************************************************//
//********************************************************************************************************************//
//Function to get image data
// //=====================================================================================================================//
//Array must be passed by pointer type .. 
void get_img_data(IplImage* img,uchar * data, int number_of_features,  CvPoint2D32f* frame1_features, float* frame1_features_data)
{
  data = (uchar*)img->imageData;
  //to get image data 
  //img_data =(float)data[j*step + i*channels+ 0 ]; x coordinate is i and y coordinate is j
  //for a gray scale image--// img->imageData + y * img->widthStep
  int y=0, x=0;
  for (int i=0; i<number_of_features;i++)
    {
      x= frame1_features[i].x;// cout<<"x="<<x<<endl;
      y= frame1_features[i].y;//cout<<"y="<<y<<endl;
      frame1_features_data[i]= (float)data[y*img->widthStep + x*img->nChannels+ 0 ]; //img->imageData + y * img->widthStep;
      // cout<<"frame1 features data="<<frame1_features_data[i]<<endl;
    }
  return;
}


void auto_features_init(IplImage* img,  CvPoint &left_lip_corner,  CvPoint &right_lip_corner,  CvPoint &lip_upper_mid, CvPoint &lip_lower_mid, CvRect &lip_r_out, CvRect* eye1_r, CvRect* eye2_r, CvRect* r_face, bool &is_first_frame,  CvPoint2D32f* left_eyebrow_ntl_dist_array,  CvPoint2D32f* right_eyebrow_ntl_dist_array)

{
 
  //======================================================================================//
  int extra_eye_width=10;// The extra width of eyes is used to avoid eye corner cutting. Can be changed later. 
  int extra_eye_height=5;// The extra height of eyes is removed . Can be changed later.
  int lip_width_extra=5;//use it if the person is smiling.
 
  //==============================================================================================//

  char file1[] = "./xml_files/haarcascade_frontalface_alt.xml";
  char file2[] = "./xml_files/haarcascade_eye.xml";
  char file3[] = "./xml_files/haarcascade_mcs_mouth.xml";
  char file4[] = "./xml_files/haarcascade_mcs_nose.xml";

  //initial points for eye_detection function
  CvPoint2D32f lip_point[2];
  lip_point[0].x = 0;
  lip_point[0].y = 0;
  lip_point[1].x = 0;
  lip_point[1].y = 0;

  //initialize the lip points to zero
 
  CvRect  *r_lip_estm;
  r_lip_estm=new CvRect();

 
  CvRect r_i1, r_i0, nose;


  /* load the face classifier */
  cascade_f = (CvHaarClassifierCascade*)cvLoad(file1, 0, 0, 0);

  /* load the eye classifier */
  cascade_e = (CvHaarClassifierCascade*)cvLoad(file2, 0, 0, 0);

  /* load the mouth classifier */
  cascade_m=(CvHaarClassifierCascade*)cvLoad(file3, 0, 0, 0);
  /*load the nouse classifier */
  cascade_n=(CvHaarClassifierCascade*)cvLoad(file4, 0, 0, 0);
  /* setup memory storage, needed by the object detector */
  storage = cvCreateMemStorage(0);
  /* detect eyes and display image */
  /***************************************************************/
  int chk_sign;
  //========Function to dectect eyes is called here==========//
  
  detectEyes(img,lip_point,r_i0,r_i1, r_face, r_lip_estm, extra_eye_width, extra_eye_height, lip_width_extra, nose, chk_sign, is_first_frame);
  save_eyes_images(img, r_i0, r_i1, r_face, eye1_r, eye2_r);
  
  
  //here eye1_r is the left eye rect location w.r.t whole image and eye2_r is the right eye rect location
  //*****************calculate the left and right eye center ********************************//
  CvPoint left_eye_cnt, right_eye_cnt;
  left_eye_cnt= cvPoint (eye1_r->x + (eye1_r->width)/2,eye1_r->y + (eye1_r->height)/2);
  right_eye_cnt= cvPoint (eye2_r->x + (eye2_r->width)/2,eye2_r->y + (eye2_r->height)/2);
  //*************************************************************************************//
  /*
  //===========FACE REGISTRATION =================//
  IplImage *img_r= cvCreateImage( cvGetSize(img), 8, 3 );
  CvPoint2D32f eye1, eye2;
  eye1.x =left_eye_cnt.x;
  eye1.y =left_eye_cnt.y;
  eye2.x = right_eye_cnt.x;
  eye2.y = right_eye_cnt.y;
  face_alignment( img, eye1, eye2, img_r, stand_eye_space );


  cvCopy (img_r, img);

  
  cvReleaseImage(&img_r);
  //=========end of face registration===========//
  */
//call the eye pupil detection function for detecting pupil of both the eyes. 
  CvPoint eye_pupl_cnt;
  //  CvRect *cnt=new CvRect();
  for(int j=0; j<2;j++)
    {
        
      if (j==0)
	{
	  cvSetImageROI(img,*eye1_r);
	}
      else 
	{
	  cvSetImageROI(img,*eye2_r);
	  //the thres_img size should be same as the size of the image reset. 
	}
      IplImage *thres_img= cvCreateImage( cvGetSize(img), 8, 1 );
      //the function to detect the eye pupil
      detectEyes_pupil(img, eye_pupl_cnt, thres_img);
 
   
      cvReleaseImage (&thres_img);
      // call the function to detect eye corner
    // CornerDetection(img,  size_window,  left_lip_corner, right_lip_corner,  eye_pupl_cnt);//doubt : it should be left_eye_corner and right_eye_corner
     

      cvResetImageROI(img);
     
    }
  // call the function to detect nose. After that call the function to detect nostril. 
  CvRect nose_r= cvRect(0, 0, 0, 0);
  //nose_r is the position of the nose w.r.t the whole image. Detect the nose ROI given the face ROI. We can later give the 
  //estimated nose postion which will further give more precision. 
  //  std::cout<<nose.x<<"\t"<<nose.y<<"\t"<<nose.height<<"\t"<<nose.width<<std::endl;
  detect_nose(img, r_face, nose_r,nose);
  //   std::cout<<" fine  here "<<std::endl;
  // Detect the nostrils 
  CvPoint l_nostril, r_nostril, nose_tip;

  locate_nostile(img,nose_r, l_nostril, r_nostril);
 
  nose_tip.x=(l_nostril.x+r_nostril.x)/2;
  nose_tip.y=(l_nostril.y+r_nostril.y)/2;

  //*****obtain the ratio to scale the face distances accordingly**********//
  x_scale=right_eye_cnt.x - left_eye_cnt.x;
  x_scale=STAND_X_SCALE/x_scale;
  y_scale=nose_tip.y - (right_eye_cnt.y + left_eye_cnt.y)/2;
  y_scale=STAND_Y_SCALE/y_scale;
  scale=y_scale/x_scale;//eye to nose height/ inter eye width
  
 
  //***********************************************************************//
  //==============ESTIMATED LIP REGION IS GIVEN BY =======================//

  /*
  //previous lip estimation method  
  lip_r_out.x=r_lip_estm->x;
  // lip_r_out.y=nose_r.y+nose_r.height;
  lip_r_out.y=nose_tip.y+nose_r.height/10;// +nose_r.height/10 is done to eliminate the problem due to nose tip
  lip_r_out.width=r_lip_estm->width;
  lip_r_out.height=1.5*nose_r.height;//twice the nose height
   */
  //===============new lip location ============================//


   lip_r_out.x=r_lip_estm->x;
   lip_r_out.y=nose_r.y + nose_r.height - nose_r.height/10; // +nose_r.height/10 is done to eliminate the problem due to nose tip
   lip_r_out.width=r_lip_estm->width;
   lip_r_out.height=2.0*nose_r.height;//twice the nose height //earlier
				      //it was 1.5 times the nose height

   std::cout<<"lip_rect= "<<nose_tip.y <<"\t"<<lip_r_out.y<<"\t"<<lip_r_out.height <<"\t"<<lip_r_out.width<<std::endl;

  save_image_roi (img, lip_r_out);
  //************************************************


  //*******detect eyelid and corner points **********//
  double proj_ratio_eye1=0.0;
  double proj_ratio_eye2=0.0;
 
  eye_pts_detection ( img, eye1_r, proj_ratio_eye1 );
  eye_pts_detection ( img, eye2_r, proj_ratio_eye2 );

  //==========================================//

  
  detect_eyebrow_features(img,
			   r_face,  
			   eye1_r,  
			   eye2_r, 
			   is_first_frame,  
			   left_eyebrow_ntl_dist_array, 
			   right_eyebrow_ntl_dist_array);
  

  //******************************End of eyebrow_detection_function****************************//    
  /* set the image ROI */
  
  cvSetImageROI(img, cvRect( lip_r_out.x, lip_r_out.y ,  lip_r_out.width,  lip_r_out.height));
 
  
   cvNamedWindow("for_lip", CV_WINDOW_AUTOSIZE);
   cvShowImage("for_lip", img);
   cvWaitKey(0);

   IplImage *t_img=cvCreateImage( cvGetSize(img), 8, 1 );

   CvSeq* lip_contour=0;


   // check the code detect lip in cvcopy function 
  detect_lip(img,
	     t_img, lip_r_out,
	     lip_contour,
	     lip_upper_mid,
	     lip_lower_mid,
	     left_lip_corner,
	     right_lip_corner );

  
  // std::cout<<"ok upto this"<<std::endl;
  cvResetImageROI(img);

  //*********************************//
  draw_circle( img, left_eye_cnt);
  draw_circle( img, right_eye_cnt);
  draw_circle( img, l_nostril);
  draw_circle( img, r_nostril);
  draw_circle( img, nose_tip);
  //draw the corner points on the image//
  draw_circle( img, left_lip_corner);
  draw_circle( img, right_lip_corner);
  draw_circle( img, lip_upper_mid);
  draw_circle( img, lip_lower_mid);
  if (is_first_frame==true)
    {
      cvSaveImage("neutral_image.jpg", img);
    }

  is_first_frame=false;
  
  //*********************************//
  //===========================================================================//
  // cvDestroyWindow("eye");
  cvDestroyWindow("img");
  // cvReleaseImage(&thres_img);
  //  cvReleaseImage(&img);
  //delete all the allocated memory 
  // delete r_face;
  delete r_lip_estm;
  
  //  delete cnt;
  cvReleaseImage(&t_img);
  cvClearMemStorage(storage);
  return;
}

//**************************************************************************************************************************//
//Automatic tracking of features points in a video . Main starts here. *****************************************************//
//**************************************************************************************************************************//


int main()
{

  //global variable
  lip_position_prev= new CvPoint2D32f [4];
  lip_rect_prev= new CvRect ();
  face_rect_prev= new CvRect();
  //  const unsigned numSecToWait = 10;
  CvCapture *input_video = cvCaptureFromFile("../videos/emotion/fear/S001-106.avi");//S034-004.avi");//S001-043.avi");//S002-026.avi ;//"brow_riser_3.avi"//ritu1.avi//S002-032.avi//happy_0008_2.mpg//
  if(!input_video)
    {
      /* Either the video didn't exist OR it uses a codec OpenCV
       * doesn't support.
       */
      fprintf(stderr, "Error: Can't open video.\n");
      exit(EXIT_FAILURE);
    }
  
  IplImage* first_frame;
  cvNamedWindow("first_frame", CV_WINDOW_AUTOSIZE);
  first_frame=cvQueryFrame(input_video);

  //******this is a special case .. i.e when the image is not frontal .. say for 90 deg colockwise and anticlockwise***//
  bool rotate= false;
  bool is_first_frame= true;
  IplImage *rotated_img= cvCreateImage(cvGetSize(first_frame),8,3);
  double angle=-90.0;// negative value gives clockwise rotation
  if(rotate==true){ 
 
    rotate_at_theta_ang (first_frame, rotated_img, angle);
    first_frame= cvCloneImage(rotated_img);
    //******************************************************************************************************************//
  }

  cvShowImage("first_frame", first_frame);
  cvWaitKey(wait_ms);


  /* Read the video's frame size out of the AVI. */
  //   CvSize frame_size;
  // frame_size.height =
  //   (int) cvGetCaptureProperty( input_video, CV_CAP_PROP_FRAME_HEIGHT );
  // frame_size.width =
  //   (int) cvGetCaptureProperty( input_video, CV_CAP_PROP_FRAME_WIDTH );


  double fps = cvGetCaptureProperty(input_video,CV_CAP_PROP_FPS);
  printf("\n Frame speed is = %d\n",(int)fps);


  // //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
  CvPoint left_lip_corner,  right_lip_corner,lip_upper_mid, lip_lower_mid ;
  CvRect lip_rect_roi;
  CvRect *eye1_r = new CvRect();
  CvRect *eye2_r = new CvRect();
  CvRect *r_face = new CvRect();
  CvPoint2D32f* left_eyebrow_ntl_dist_array=new CvPoint2D32f[ 4 ];
  CvPoint2D32f* right_eyebrow_ntl_dist_array=new CvPoint2D32f[ 4 ];

  auto_features_init( first_frame, 
		      left_lip_corner, 
		      right_lip_corner,
		      lip_upper_mid, 
		      lip_lower_mid,
		      lip_rect_roi, 
		      eye1_r,
		      eye2_r, 
		      r_face, 
		      is_first_frame,  left_eyebrow_ntl_dist_array,  right_eyebrow_ntl_dist_array);


  
  cvShowImage("first_frame", first_frame);
  cvWaitKey(wait_ms);

  cout<<"features are successfully initialized automatically!"<<endl;
  cvDestroyWindow("first_frame");
  // lip data is considered as neutral lip data when frame is the first frame
  CvPoint ntl_left_lip_corner=left_lip_corner;
  CvPoint ntl_right_lip_corner=right_lip_corner;
  CvPoint ntl_upper_lip_mid_pt=lip_upper_mid;
  CvPoint ntl_lower_lip_mid_pt=lip_lower_mid;

  double proj_ratio_eye1=0.0;
  double proj_ratio_eye2=0.0;
  //*************************************************//
  int frame_no = 0;
  int key;
 
  static IplImage *frame = NULL; 

  while(cvGrabFrame(input_video)) 
    // while(true)
    {
      frame_no++;

      // if(2 == frame_no) break;

      printf("Frame no. %d----------\n" , frame_no);
    
      frame = cvRetrieveFrame( input_video );  
      if(frame == NULL)
	{
	  /* Why did we get a NULL frame? We shouldn't be at the end. */
	  fprintf(stderr, "Error: Hmm. The end came sooner than we thought.\n");
	  return -1;
	}
   
      //************************rotate the frame by the angle ********************//
      if(rotate==true){     
	angle=-90.0;
	rotate_at_theta_ang (frame, rotated_img, angle);
	frame= cvCloneImage(rotated_img);

      }

     
      //******************************************************************//



      //  lip_rect_roi.height=lip_rect_roi.height+0.020*lip_rect_roi.height; // This part is a spaecial case.. to be used only in image S001-043.avi  
    
      /*******************************************************************/
      //This part of the code is used for smile detection 

      // display text on the image 

      /* initialize font and add text */
             
           
      auto_features_init( frame, 
			  left_lip_corner, 
			  right_lip_corner,
			  lip_upper_mid, 
			  lip_lower_mid,
			  lip_rect_roi, 
			  eye1_r,
			  eye2_r, 
			  r_face, 
			  is_first_frame,  left_eyebrow_ntl_dist_array,  right_eyebrow_ntl_dist_array);

      
     

      //***********don't use this part of the code when auto features init function is called in the while loop************//
      /*	
		CvFont font;
		cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 1, CV_AA);
		bool smile=false;
		cout<<"checking if smiling"<<endl;
		detect_smile( frame,
		lip_rect_roi, 
		ntl_left_lip_corner,
		ntl_right_lip_corner,
		left_lip_corner, 
		right_lip_corner, 
		lip_upper_mid, lip_lower_mid,  smile);

		if(smile==true)
		{
		cout<<"smiling"<<endl;
		cvPutText(frame, "Smiling!", cvPoint(10, 130), &font, cvScalar(255, 255, 0, 0));
		}
		else
	 
		cout<<"not smiling"<<endl;
       

		//----this part of the code is to detect eyebrow region and extract eyebrow features from that-----//

		detect_eyebrow_features( frame,
		r_face, 
		eye1_r, 
		eye2_r,
		is_first_frame,
		left_eyebrow_ntl_dist_array,
		right_eyebrow_ntl_dist_array);
     

      */
	
      //---don't use above part of the code when auto features init function is called in the while loop------//
      
      lip_features(ntl_left_lip_corner,
		   ntl_right_lip_corner,
		   ntl_upper_lip_mid_pt,
		   ntl_lower_lip_mid_pt,
		   left_lip_corner, 
		   right_lip_corner, 
		   lip_upper_mid, // 
		   lip_lower_mid);



      //--------------detect eyelid and corner points --------------//
     
     
	  
      eye_pts_detection ( frame, eye1_r, proj_ratio_eye1 );
      eye_pts_detection ( frame, eye2_r, proj_ratio_eye2 );


     
      write_scalar_data_to_file(filename1,  proj_ratio_eye1 );
      write_scalar_data_to_file(filename2,  proj_ratio_eye2 );

      
      //===============================================================//

      cvNamedWindow("curr_frame", CV_WINDOW_AUTOSIZE);
      cvShowImage("curr_frame",frame);

      key= cvWaitKey(wait_ms);// press esc to exit from the while loop
      //  cout<<"key is ="<<key<<endl;
      if (key==1048603)break;//if 'esc' key is pressed then go out of while loop.1048603

      cvDestroyWindow("curr_frame");
     
      string fileName = "output_img"+num2str(frame_no)+".jpeg";
      cvSaveImage(fileName.c_str(), frame);
    

      
    }// end of while loop************************************//



  //==============================================================//
  //call the function here to read all the movement data file and write
  //them into a single file 

  read_data_write_back();   

  //===========================================================//
  rotate=false;//optional
 
  cvDestroyWindow("Optical Flow");
  cvReleaseImage(&first_frame);
  cvReleaseImage(&frame);
  cvReleaseImage(&rotated_img);
 
  delete eye1_r;
  delete eye2_r;
  delete r_face;
  delete lip_rect_prev;
  delete face_rect_prev;
  //cvReleaseCapture(&input_video); //removes the segmentation fault error
  delete []left_eyebrow_ntl_dist_array;
  delete []right_eyebrow_ntl_dist_array;

  //  delete []lip_position_prev; //giving memory leakage problem
  cvReleaseHaarClassifierCascade( &cascade_f );
  cvReleaseHaarClassifierCascade( &cascade_e );
  cvReleaseHaarClassifierCascade( &cascade_m );
  cvReleaseHaarClassifierCascade( &cascade_n );
 

  cvDestroyAllWindows();


  return 0;
}// end of main function for automatic lip features tracking =========================================================//
//********************************************************************************************************************//





