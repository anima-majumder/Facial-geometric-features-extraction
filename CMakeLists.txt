cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(lbp)
set(MY_PROJECT_DIRECTORY /home/ani/work/code/main/facial_emotion_detection/geometric_features_generalized_code)
set(PROJECT_LINK_DIRS /usr/local/lib) 
set(OPENCV_LIB opencv_imgproc opencv_objdetect opencv_features2d opencv_flann opencv_gpu opencv_highgui opencv_legacy opencv_ml opencv_nonfree)
set(PROJECT_INCLUDE_DIRS /usr/local/include; usr/include; ${MY_PROJECT_DIRECTORY}/include)

set(CV1_LIBS cv cxcore highgui cvaux) ##boost_thread gsl gslcblas)
set(ANI_LIBS constants utils  face_features LBP GABOR )
find_package(OpenCV REQUIRED)
 
include_directories(${PROJECT_INCLUDE_DIRS})
link_directories(${PROJECT_LINK_DIRS} ${MY_PROJECT_DIRECTORY}/build)
#add_definitions(${PCL_DEFINITIONS})
 
#set(PCL_BUILD_TYPE Release)
add_library(constants ${MY_PROJECT_DIRECTORY}/source/constants.cpp) 
add_library(utils ${MY_PROJECT_DIRECTORY}/source/utils.cpp) 
#add_library(gabor ${MY_PROJECT_DIRECTORY}/src/gabor.cpp) 
add_library(LBP ${MY_PROJECT_DIRECTORY}/source/LBP.cpp) 
add_library(GABOR ${MY_PROJECT_DIRECTORY}/source/GABOR.cpp) 
add_library(face_features ${MY_PROJECT_DIRECTORY}/source/face_features.cpp) 
target_link_libraries(utils ${OPENCV_LIB} ${CV1_LIBS})
target_link_libraries(face_features ${OPENCV_LIB} ${CV1_LIBS} constants utils LBP GABOR)
function(add_programs)
	foreach (prog ${ARGV})
		add_executable(${prog} ${MY_PROJECT_DIRECTORY}/main/${prog}.cpp)
		target_link_libraries(${prog} ${OpenCV_LIBS} ${CV1_LIBS} ${ANI_LIBS})
	endforeach()
endfunction()

add_programs(

main
  
)




#add_executable(gabor src/main.cpp)
 
#target_link_libraries (gabor ${OpenCV_LIBS})