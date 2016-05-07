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

#include "Serials.h"
#include "MessageController.h"
#include "LaserSnap.h"

/*=====================================
 Variables definition
 =====================================*/
unsigned char Task;
unsigned char LocationPoint;

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
		MessageController_TaskSelect( Task, LocationPoint );

		switch( Task )
		{
			case 0x11:
			{
				printf( "Main : Task = 0x11.\n" );
				break;
			}

			case 0x22:
			{
				printf( "Main : Task = 0x22.\n" );
				break;
			}

			case 0x33:
			{
				printf( "Main : Task = 0x33.\n" );
				break;
			}

			case 0x44:
			{
				printf( "Main : Task = 0x44.\n" );
				break;
			}

			case 0xA1:
			{
				printf( "Main : Task = 0xA1.\n" );

				//Take a laser snap.

				//Probe out the cross point and line slope.
				LaserSnap_Probe( LASERSNAP_PHOTO0_DIRECTORY, LaserSnap_CrossPoint[ 0 ], LaserSnap_Line1Slope[ 0 ], LaserSnap_Line2Slope[ 0 ] );

				break;
			}

			case 0xA2:
			{
				printf( "Main : Task = 0xA2.\n" );

				//Probe out the cross point and line slope.

				//Calculate the offset.

				break;
			}

			case 0xA3:
			{
				printf( "Main : Task = 0xA3.\n" );

				//Probe out the cross point and line slope.

				//Calculate the offset.

				break;
			}

			case 0xA4:
			{
				printf( "Main : Task = 0xA4.\n" );

				//Probe out the cross point and line slope.

				//Calculate the offset.

				break;
			}

			default:
			{
				printf( "Main : Task Selection Error.\n" );
				break;
			}
		}
	}

	return 0;
}
