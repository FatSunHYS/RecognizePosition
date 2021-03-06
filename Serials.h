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
#define SERIALS_RECIEVEDATA_BUFFERLENGTH	8
#define SERIALS_TRANSMITDATA_BUFFERSLOT		16
#define SERIALS_TRANSMITDATA_BUFFERLENGTH	8



/*=====================================
Global Macro
=====================================*/
#if 0
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
extern unsigned char* Serials_GetRecieveMessage();
extern unsigned char* Serials_GetTransmitBuffer();
extern void Serials_MessageRead();
extern void Serials_SendMessage();
extern bool Serials_IsNewMessageRecieved();




#endif /* SERIALS_H_ */
