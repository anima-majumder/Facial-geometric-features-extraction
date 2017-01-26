#include <fstream>
#include"eye_brow_features_detection.h"


void detect_eyebrow_features(IplImage* img,CvRect* r_face, CvRect* eye1_r, CvRect* eye2_r, bool &is_first_frame,  CvPoint2D32f* left_eyebrow_ntl_dist_array,  CvPoint2D32f* right_eyebrow_ntl_dist_array)

{
  /* Locate the eye brow positions. Call the function here. Function is in the file "locate_eye_brow.cpp"*/
  CvRect eye_brow1_r;
  CvRect eye_brow2_r;
 
  locate_eye_brow( r_face, eye1_r, eye_brow1_r);
  //  cout<<"eye brow1 postion is: "<<eye_brow1_r.x<<","<<eye_brow1_r.y<<","<<eye_brow1_r.width<<","<<eye_brow1_r.height<<endl;
  locate_eye_brow( r_face, eye2_r, eye_brow2_r);
  //================================================================================================================//
  //save eye brow images to be used for obtaining contours
  save_eye_brow_images(img, eye_brow1_r, eye_brow2_r);
  //========================================================================//

  
  //Extract eye brow contour
  IplImage *eye_brow_contour1, *eye_brow_contour2;
  // string image1="thres1.jpeg";
  // string image2="thres2.jpeg";
  //eye brow h planes are saved as :
   char  eye_brow1_h_plane[]="eye_brow_h_plane1.jpeg";
   char  eye_brow2_h_plane[]="eye_brow_h_plane2.jpeg";

  //threshold are saved as 
  char image1[]="thres1.jpeg";
  char  image2[]="thres2.jpeg";
  //contour image to be saved as 
  char contour_image1[]="eye_brow_contour1.jpeg";
  char  contour_image2[]="eye_brow_contour2.jpeg";
  int k=5;//k is used for threshold extra weight k=-20

  //for first eyebrow*****************************************//
  // CvPoint eye_brow_center1;
  CvPoint  eye_brow_left1, eye_brow_upper_mid1, eye_brow_lower_mid1, eye_brow_right1;
  //for 2nd eyebrow******************************************// 
  //CvPoint eye_brow_center2;
  CvPoint  eye_brow_left2, eye_brow_upper_mid2, eye_brow_lower_mid2,  eye_brow_right2;


  IplImage *eye_brow_1=cvLoadImage("./eye_brow1.jpeg", 1 );
  eye_brow_contour1=cvCreateImage( cvGetSize(eye_brow_1), 8, 1 );
  //extract the 1st  eye brow contour
  extract_eye_brow_contour(eye_brow_1, 
			   eye_brow_contour1,
			   k,
			   &image1[0],
			   &contour_image1[0], 
			   &eye_brow1_h_plane[0],
			   eye_brow1_r,
			   eye_brow_left1,
			   eye_brow_upper_mid1,
			   eye_brow_lower_mid1, 
			   eye_brow_right1);


  //**************************************************//
  k=10;
 //extract the 2nd eye brow contour
  IplImage *eye_brow_2=cvLoadImage("./eye_brow2.jpeg", 1 );
  eye_brow_contour2=cvCreateImage(cvGetSize(eye_brow_2),8,1);
  
  extract_eye_brow_contour(eye_brow_2,
			   eye_brow_contour2, 
			   k, 
			   &image2[0],
			   &contour_image2[0], 
			   &eye_brow2_h_plane[0], 
			   eye_brow2_r, 
			   eye_brow_left2, 
			   eye_brow_upper_mid2, 
			   eye_brow_lower_mid2,  
			   eye_brow_right2);


  // cout<<"eye brow points after updation are :"<<eye_brow_left1.x<<","<<eye_brow_left1.y<<" "<<eye_brow_right1.x<<" "<<eye_brow_right1.y<<" "<<endl;


  //draw circle at the obtained points
 draw_circle(img,eye_brow_upper_mid1);
 draw_circle(img,eye_brow_lower_mid1);
 //draw left and right eye brow points

 draw_circle(img,eye_brow_left1);
 draw_circle(img,eye_brow_right1);

  //draw circle at the obtained points
 draw_circle(img,eye_brow_upper_mid2);
 draw_circle(img,eye_brow_lower_mid2);
 //draw left and right eye brow points

 draw_circle(img,eye_brow_left2);
 draw_circle(img,eye_brow_right2);
 char filename1[]="left_eyebrow_feature_points.txt";
 char filename2[]="right_eyebrow_feature_points.txt";
 char filename3[]="left_eyebrow_movement_array.txt";
 char filename4[]="right_eyebrow_movement_array.txt";
 char filename5[]="eyebrow_movement_array.txt";
 CvPoint2D32f left_eyebrow_points_array[4];
 CvPoint2D32f right_eyebrow_points_array[4];
 CvPoint2D32f* left_eyebrow_movement_array=new CvPoint2D32f[ 4 ];
 CvPoint2D32f* right_eyebrow_movement_array=new CvPoint2D32f[ 4 ];
 CvPoint2D32f *eyebrow_movement_array=new CvPoint2D32f[ 8 ];

 // // //**************for test ***************************//
 //  CvPoint2D32f *left_eyebrow_ntl_pt_array=new CvPoint2D32f[ 4 ];
 //  CvPoint2D32f *right_eyebrow_ntl_pt_array=new CvPoint2D32f[ 4 ];

  //allocate memory
 // //******************************************//

 // CvPoint eyebrow_movement=cvPoint(0, 0);
 
 //bool  left_eyebrow_pt=true;
 CvPoint left_eye_cnt=cvPoint (0,0), right_eye_cnt= cvPoint(0,0);
 // CvPoint eyebrow_pt=cvPoint(0,0), ntl_eye_brow_dist=cvPoint(0,0);
 left_eye_cnt= cvPoint (eye1_r->x + (eye1_r->width)/2,eye1_r->y + (eye1_r->height)/2);
 right_eye_cnt= cvPoint (eye2_r->x + (eye2_r->width)/2,eye2_r->y + (eye2_r->height)/2);

 //************************************************//
 left_eyebrow_points_array[0].x = eye_brow_left1.x;
 left_eyebrow_points_array[0].y = eye_brow_left1.y;


 left_eyebrow_points_array[1].x = eye_brow_upper_mid1.x;
 left_eyebrow_points_array[1].y = eye_brow_upper_mid1.y;

 left_eyebrow_points_array[2].x = eye_brow_lower_mid1.x;
 left_eyebrow_points_array[2].y = eye_brow_lower_mid1.y;

 left_eyebrow_points_array[3].x = eye_brow_right1.x;
 left_eyebrow_points_array[3].y = eye_brow_right1.y;

 //**************************************************//

 right_eyebrow_points_array[0].x = eye_brow_left2.x;
 right_eyebrow_points_array[0].y = eye_brow_left2.y;


 right_eyebrow_points_array[1].x = eye_brow_upper_mid2.x;
 right_eyebrow_points_array[1].y = eye_brow_upper_mid2.y;

 right_eyebrow_points_array[2].x = eye_brow_lower_mid2.x;
 right_eyebrow_points_array[2].y = eye_brow_lower_mid2.y;

 right_eyebrow_points_array[3].x = eye_brow_right2.x;
 right_eyebrow_points_array[3].y = eye_brow_right2.y;
 //******************************************************//



 // CvPoint left_eyebrow_ntl_dist=cvPoint(0,0);

 if(is_first_frame)
     
   {

 calculate_neutral_eyebrow_movement_pts (  left_eyebrow_points_array,
					   right_eyebrow_points_array, 
					   left_eyebrow_ntl_dist_array,
					   right_eyebrow_ntl_dist_array, 
					   left_eye_cnt,
					   right_eye_cnt );


 // //************test******************//
 // for(int i=0; i<4; i++)
 //   {
 //     left_eyebrow_ntl_pt_array[i].x=left_eyebrow_points_array[i].x;
 //     left_eyebrow_ntl_pt_array[i].y=left_eyebrow_points_array[i].y;
 //     right_eyebrow_ntl_pt_array[i].x=right_eyebrow_points_array[i].x;
 //     right_eyebrow_ntl_pt_array[i].y=right_eyebrow_points_array[i].y;

 //   cout<<"first frame test"<<left_eyebrow_ntl_pt_array[i].x<<"\t"<<left_eyebrow_ntl_pt_array[i].y<<"\t"<<left_eyebrow_points_array[i].x<<"\t"<<left_eyebrow_points_array[i].y<<endl;
 // }

 ////*****************************************//

 //cout<<"left_eyebrow_ntl_dist_array[0].x"<<"\t"<<left_eyebrow_ntl_dist_array[0].x<<endl;
 
   }
   //cout<<"chk"<<left_eyebrow_ntl_dist_array[0].x<<"\t"<<left_eyebrow_ntl_dist_array[0].y<<endl;
   
 //**************************calculate for right eyebrow***********************//


else
   {

 eyebrow_movement_fn (left_eyebrow_points_array, 
		      right_eyebrow_points_array,
		      left_eye_cnt, 
		      right_eye_cnt,
		      left_eyebrow_movement_array,
		      right_eyebrow_movement_array, 
		      left_eyebrow_ntl_dist_array,
		      right_eyebrow_ntl_dist_array );


  //write all the left eyebrow features movements data into a text file.

 for (int i=0; i<4;i++)
   {
     eyebrow_movement_array[i].x=left_eyebrow_movement_array[i].x;
     eyebrow_movement_array[i].y=left_eyebrow_movement_array[i].y;
     eyebrow_movement_array[4+i].x=right_eyebrow_movement_array[i].x;
     eyebrow_movement_array[4+i].y=right_eyebrow_movement_array[i].y;
   }



 // //*********test*****************//

 // for(int i=0; i<4; i++)  
 //       {
 // 	 left_eyebrow_movement_array[i].x = left_eyebrow_ntl_pt_array[i].x -left_eyebrow_points_array[i].x;//negative if moves inside
 // 	 left_eyebrow_movement_array[i].y = left_eyebrow_ntl_pt_array[i].y -left_eyebrow_points_array[i].y;//negative if moves down

 // 	 right_eyebrow_movement_array[i].x = right_eyebrow_ntl_pt_array[i].x -right_eyebrow_points_array[i].x;//negative if moves inside
 // 	 left_eyebrow_movement_array[i].y = right_eyebrow_ntl_pt_array[i].y -right_eyebrow_points_array[i].y;//negative if moves down

 // 	 cout<<"test"<<left_eyebrow_ntl_pt_array[i].x<<"\t"<<left_eyebrow_ntl_pt_array[i].y<<"\t"<<left_eyebrow_points_array[i].x<<"\t"<<left_eyebrow_points_array[i].y<<endl;
 //       }

 //  //*******************************//
  
   
 //************************************************************************//

 write_movement_data_to_file( filename3,left_eyebrow_movement_array, 4);
 
 write_movement_data_to_file( filename4,right_eyebrow_movement_array, 4);

 write_movement_data_to_file( filename5,eyebrow_movement_array, 8);


}

 //write all the left and right eyebrow features data into a text file.

 
 write_to_text_file( filename1, eye_brow_left1, eye_brow_upper_mid1, eye_brow_lower_mid1, eye_brow_right1);


 
 write_to_text_file( filename2, eye_brow_left2, eye_brow_upper_mid2, eye_brow_lower_mid2, eye_brow_right2);




  //88888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888//
  //********************************************************************************************************************//
 
 delete [] right_eyebrow_movement_array;
 delete [] left_eyebrow_movement_array;
 delete [] eyebrow_movement_array;
 // delete [] right_eyebrow_ntl_pt_array;
 // delete [] left_eyebrow_ntl_pt_array;

 cvReleaseImage( &eye_brow_1);
 cvReleaseImage( &eye_brow_2);
 cvReleaseImage( &eye_brow_contour1);
 cvReleaseImage( &eye_brow_contour2);

 return;

}

