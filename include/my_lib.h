#ifndef MY_LIB_H

#define MY_LIB_H

#define FACE_HEIGHT ()
#define FACE_WIDTH ()
#define STAND_X_SCALE (72)
#define STAND_Y_SCALE (46)

int read_data_write_back();
int eye_pts_detection (IplImage* img, CvRect* eye_r , double &proj_ratio);
int detectEyes(IplImage *img, CvPoint2D32f *lip_point,  CvRect& r_i0, CvRect &r_i1, CvRect *face_rect, CvRect *r2, int extra_eye_width, int extra_eye_height, int lip_width_extra, CvRect& nose, int &chk_sign, bool &is_first_frame);
void save_eyes_images(IplImage *img, CvRect& r_i0, CvRect& r_i1, CvRect *face_rect, CvRect *eye1_rect, CvRect *eye2_rect);
void  detectEyes_pupil(IplImage *img, CvPoint& eye_cnt, IplImage *t_img );
int CornerDetection(IplImage* src, int size_window, CvPoint& left_corner_feature,CvPoint& right_corner_feature,  CvPoint& origin);
void detect_nose(IplImage *img, CvRect *face_rect, CvRect& nose_rect, CvRect& nose_estm);
void locate_nostile(IplImage *img,CvRect& nose_rect, CvPoint& l_nostril, CvPoint& r_nostril);
void save_image_roi(IplImage* input, CvRect& roi);
void detect_eyebrow_features(IplImage* img,CvRect* r_face, CvRect* eye1_r, CvRect* eye2_r, bool &is_first_frame,  CvPoint2D32f* left_eyebrow_ntl_dist_array,  CvPoint2D32f* right_eyebrow_ntl_dist_array);
void  detect_lip(IplImage *img, IplImage *t_img,CvRect &lip_rect, CvSeq* lip_contour, CvPoint& upper_corner_feature, CvPoint& lower_corner_feature, CvPoint& left_corner_feature, CvPoint &right_corner_feature );
void draw_circle(IplImage* img, CvPoint eyecentroid);
//void detect_smile(IplImage* frame, CvRect &lip_roi, CvPoint &ntl_left_lip_corner, CvPoint &ntl_right_lip_corner, bool &smile);

void detect_smile(IplImage* frame, CvRect &lip_roi,const CvPoint &ntl_left_lip_corner, const CvPoint &ntl_right_lip_corner, CvPoint &left_lip_corner, CvPoint &right_lip_corner,  CvPoint &lip_upper_mid, CvPoint &lip_lower_mid,  bool &smile);

void rotate_at_theta_ang (IplImage* src, IplImage* dst, double &angle);

void lip_features( CvPoint &ntl_left_lip_corner, CvPoint &ntl_right_lip_corner,  CvPoint &ntl_upper_lip_mid_pt, CvPoint &ntl_lower_lip_mid_pt,CvPoint &left_lip_corner,  CvPoint &right_lip_corner,   CvPoint &lip_upper_mid,   CvPoint &lip_lower_mid);
//==========================================================================================================================//


static const double pi = 3.14159265358979323846;


#endif
