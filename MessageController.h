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


#ifndef MESSAGECONTROLLER_H_
#define MESSAGECONTROLLER_H_

/*=====================================
Include headers
=====================================*/



/*=====================================
Global Constants
=====================================*/
#define TASK_NONE		0x00
#define TASK_PHOTOSNAP1 0x11
#define TASK_PHOTOSNAP2 0x22
#define TASK_PHOTOSNAP3 0x33
#define TASK_PHOTOSNAP4 0x44
#define TASK_LASERSNAP1 0xA1
#define TASK_LASERSNAP2	0xA2
#define TASK_LASERSNAP3	0xA3
#define TASK_LASERSNAP4 0xA4


/*=====================================
Global Macro
=====================================*/



/*=====================================
Extern Variables
=====================================*/



/*=====================================
Extern Functions
=====================================*/
extern void MessageController_TaskSelect( unsigned char& TaskSelection, unsigned char& LocationPoint );





#endif /* MESSAGECONTROLLER_H_ */
