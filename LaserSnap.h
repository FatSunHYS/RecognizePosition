/**
 ******************************************************************************
 * @project	
 *		RecognizePosition
 * @author  			
 *		
 * @creationdate   
 *		
 * @lastmodifydate
 *		May 6, 2016
 * @verify
 *		
 * @verifydate
 * 
 * @company
 *		
 ******************************************************************************
 * @brief   
 *		
 * @attention
 *		
 ******************************************************************************
 */


#ifndef LASERSNAP_H_
#define LASERSNAP_H_

/*=====================================
Include headers
=====================================*/
#include "cv.h"
#include "highgui.h"
#include "imgproc/imgproc.hpp"


/*=====================================
Global Constants
=====================================*/
#define RED_CHANNEL_OFFSET	 	2
#define GREEN_CHANNEL_OFFSET	1
#define BLUE_CHANNEL_OFFSET		0

#define LASERSNAP_PHOTO0_DIRECTORY ( char* )"./photo/lasersnap_0.jpg"


/*=====================================
Global Macro
=====================================*/



/*=====================================
Extern Variables
=====================================*/
extern CvPoint LaserSnap_CrossPoint[ 4 ];
extern double LaserSnap_Line1Slope[ 4 ];
extern double LaserSnap_Line2Slope[ 4 ];


/*=====================================
Extern Functions
=====================================*/
extern bool LaserSnap_Probe( char* Photo, CvPoint& CrossPoint, double& Line1slope, double& Line2slope );





#endif /* LASERSNAP_H_ */
