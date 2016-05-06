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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

#include "Serials.h"


/*=====================================
 Variables definition
 =====================================*/


/*=====================================
 Functions definition
 =====================================*/


/*=====================================
 Implementation of functions
 =====================================*/


int main()
{

	if( Serials_Initialization() != true )
	{
		printf( "Main : Serials_Initialization() Error!\n" );
		return 0;
	}

	printf( "Main : Serials Initialization Done!\n" );




	while( true )
	{

	}

	return 0;
}
