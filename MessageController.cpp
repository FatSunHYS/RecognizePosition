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

/*=====================================
 Include headers
 =====================================*/
#include <stdlib.h>
#include <stdio.h>

#include "MessageController.h"
#include "Serials.h"

/*=====================================
 Variables definition
 =====================================*/

/*=====================================
 Functions definition
 =====================================*/
void MessageController_TaskSelect( unsigned char& TaskSelection, unsigned char& LocationPoint );
bool MessageController_VerifyMessage( unsigned char* Data, int DataLength );
unsigned char MessageController_CalculateCheckSum( unsigned char* Data, int DataLength );

/*=====================================
 Implementation of functions
 =====================================*/

void MessageController_TaskSelect( unsigned char& TaskSelection, unsigned char& LocationPoint )
{
	unsigned char* TemporaryRecieveMessage = NULL;
	unsigned char* TemporaryTransmitMessage = NULL;
	unsigned short ReplyType;

	while( true )
	{
		if( Serials_IsNewMessageRecieved() == false )
		{
			continue;
		}

		TemporaryTransmitMessage = Serials_GetTransmitBuffer();
		if( TemporaryTransmitMessage == NULL )
		{
			continue;
		}

		TaskSelection = TASK_NONE;
		TemporaryRecieveMessage = Serials_GetRecieveMessage();

		switch( TemporaryRecieveMessage[ 0 ] )
		{
			//Reply frame.
			case 0xFD:
			{
				if( MessageController_VerifyMessage( TemporaryRecieveMessage, 8 ) == false )
				{
					Serials_MessageRead();
					printf( "MessageController : Verify Message failed.\n" );
					continue;
				}

				ReplyType = TemporaryRecieveMessage[ 3 ];
				ReplyType <<= 8;
				ReplyType += TemporaryRecieveMessage[ 4 ];

				if( ReplyType == 0xAAAA )
				{
					//Successfully
					printf( "MessageController : ReplyType is 0xAAAA, Reply Successfully.\n" );

				}
				else if( ReplyType == 0x5555 )
				{
					//Failed.
					printf( "MessageController : ReplyType is 0x5555, Reply Failed.\n" );
				}
				else
				{
					//Error.
					printf( "MessageController : ReplyType Error!" );
				}

				Serials_MessageRead();
				break;
			}

				//Control frame.
			case 0xCA:
			{
				if( MessageController_VerifyMessage( TemporaryRecieveMessage, 6 ) == false )
				{
					Serials_MessageRead();
					printf( "MessageController : Verify Message failed.\n" );

					TemporaryTransmitMessage[ 0 ] = 0xC1;
					TemporaryTransmitMessage[ 1 ] = 0xC1;
					TemporaryTransmitMessage[ 2 ] = 0x55;
					TemporaryTransmitMessage[ 3 ] = 0x00;
					TemporaryTransmitMessage[ 4 ] = 0x00;
					TemporaryTransmitMessage[ 5 ] = 0x55;
					Serials_SendMessage();

					continue;
				}

				//Set location point.
				LocationPoint = TemporaryRecieveMessage[ 2 ];

				//Set task.
				TaskSelection = TemporaryRecieveMessage[ 4 ];

				TemporaryTransmitMessage[ 0 ] = 0xC1;
				TemporaryTransmitMessage[ 1 ] = 0xC1;
				TemporaryTransmitMessage[ 2 ] = 0xAA;
				TemporaryTransmitMessage[ 3 ] = 0x00;
				TemporaryTransmitMessage[ 4 ] = 0x00;
				TemporaryTransmitMessage[ 5 ] = 0xAA;
				Serials_SendMessage();

				Serials_MessageRead();
				break;
			}

			default:
			{
				Serials_MessageRead();
				printf( "MessageController : Message type incorrect.\n" );
				break;
			}
		}

		if( TaskSelection == TASK_NONE )
		{
			continue;
		}
		else
		{
			break;
		}

	}

	return;
}

bool MessageController_VerifyMessage( unsigned char* Data, int DataLength )
{
	unsigned char result = MessageController_CalculateCheckSum( Data, DataLength - 1 );

	if( result == Data[ DataLength - 1 ] )
	{
		return true;
	}
	else
	{
		return false;
	}
}

unsigned char MessageController_CalculateCheckSum( unsigned char* Data, int DataLength )
{
	unsigned char result = 0x00;

	for( int i = 0; i < DataLength; ++i )
	{
		result ^= Data[ i ];
	}

	return result;

}

