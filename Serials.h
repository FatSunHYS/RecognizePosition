/**
 ******************************************************************************
 * @project	
 *		RecognizePosition
 * @author  			
 *		
 * @creationdate   
 *		
 * @lastmodifydate
 *		May 1, 2016
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


#ifndef SERIALS_H_
#define SERIALS_H_

/*=====================================
Include headers
=====================================*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>




/*=====================================
Global Constants
=====================================*/
#if 1
#define DEVICE_SERIALS (char*)"/dev/ttyUSB1"
#else
#define DEVICE_SERIALS (char*)"/dev/ttyAMA2"
#endif


/*=====================================
Global Macro
=====================================*/



/*=====================================
Extern Variables
=====================================*/



/*=====================================
Extern Functions
=====================================*/
extern bool Serials_Initialization();






#endif /* SERIALS_H_ */
