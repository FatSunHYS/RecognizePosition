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


/*=====================================
Include headers
=====================================*/
#include "Serials.h"




/*=====================================
Variables definition
=====================================*/
int serials_fd;


/*=====================================
Functions definition
=====================================*/
bool Serials_Initialization();


/*=====================================
Implementation of functions
=====================================*/

bool Serials_Initialization()
{
	//Open the serials.
	serials_fd = open( DEVICE_SERIALS, O_RDWR );
	if( serials_fd == -1 )
	{
		printf( "Could not open serials %s.\n", DEVICE_SERIALS );
		return false;
	}

	printf( "Open serials successfully.\n" );

	//Setup the serials.


	close( serials_fd );

	return true;
}
