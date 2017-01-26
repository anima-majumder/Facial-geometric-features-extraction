# ifndef _GLOBAL_H_
# define _GLOBAL_H_

/* # define wait_ms 10 */

extern unsigned const wait_ms; /* = 10; */
extern float x_scale;	//pixel distance between two eyes.. x dist.
extern float y_scale;	//pixel distnace between two eyes and nose tip. y distance 
extern float scale; 	// ratio of y_scale /x_scale


extern CvRect eye1r0_prev; //previous frame's left eye location
extern CvRect eye2r1_prev; //previous frame's right eye location

extern CvRect nose_prev; //previous nose rect
extern CvPoint2D32f* lip_position_prev; // previous lip position
extern CvRect * lip_rect_prev;//previous frame's lip's rectangular region
extern CvRect* face_rect_prev; //previous rectangular region containing face
# endif
