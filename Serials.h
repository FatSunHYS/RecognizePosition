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





/*=====================================
Global Constants
=====================================*/
#define SERIALS_RECIEVEDATA_BUFFERSLOT  	16
#define SERIALS_RECIEVEDATA_BUFFERLENGTH	6
#define SERIALS_TRANSMITDATA_BUFFERSLOT		16
#define SERIALS_TRANSMITDATA_BUFFERLENGTH	6



/*=====================================
Global Macro
=====================================*/
#if 1
#define DEVICE_SERIALS (char*)"/dev/ttyUSB1"
#else
#define DEVICE_SERIALS (char*)"/dev/ttyAMA2"
#endif


/*=====================================
 Class definition
 =====================================*/


/*=====================================
Extern Variables
=====================================*/



/*=====================================
Extern Functions
=====================================*/
extern bool Serials_Initialization();






#endif /* SERIALS_H_ */
