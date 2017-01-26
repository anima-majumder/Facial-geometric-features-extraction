#include"locate_eye_brow.h"

//function to locate the eye brow. 
//if sign is -1 then right eye is detected first else left eye is detected first

void locate_eye_brow( CvRect *face_rect, CvRect* eye_rect, CvRect& eye_brow)
{

  //Obtain the rectangular region for eye brow from the given data: face rectangle, eye rectangle
  //estimated eye brow location is:  
  
  /* this part was working previously */
  /*
    int extra=eye_rect->width/10;//earlier extra was equal to 5
  

    eye_brow.x=eye_rect->x-2.0*extra;
    eye_brow.y=face_rect->y+face_rect->height/8;// earlier it was 5
    eye_brow.height=eye_rect->y-eye_brow.y+1.5*extra;//distance from the eye brow initial y location to the eye initial y location 
    eye_brow.width=eye_rect->width+3*extra; 
  
  */
  /* above part was working previously */


  //=======================================//
 
  int extra=eye_rect->width/10;//earlier extra was equal to 5
  eye_brow.x=eye_rect->x-1.5*extra;
  eye_brow.y=face_rect->y+face_rect->height/5;// earlier it was 5
  eye_brow.height=eye_rect->y-eye_brow.y+1.5*extra;//distance from the eye brow initial y location to the eye initial y location 
  eye_brow.width=eye_rect->width+3.0*extra;

 

  //=========================================//

  return;
}


