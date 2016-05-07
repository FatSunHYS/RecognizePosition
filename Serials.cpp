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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <pthread.h>

/*=====================================
 Variables definition
 =====================================*/
int serials_fd;
struct termios serials_opt;
pthread_t serials_recievedata_tid;
pthread_t serials_transmitdata_tid;

unsigned char Serials_RecieveBuffer[ SERIALS_RECIEVEDATA_BUFFERSLOT ][ SERIALS_RECIEVEDATA_BUFFERLENGTH ];
bool Serials_RecieveData_Valid[ SERIALS_RECIEVEDATA_BUFFERSLOT ];
int Serials_RecieveBuffer_SlotIndex_W;
int Serials_RecieveBuffer_SlotIndex_R;
unsigned char Serials_TransmitBuffer[ SERIALS_TRANSMITDATA_BUFFERSLOT ][ SERIALS_TRANSMITDATA_BUFFERLENGTH ];
bool Serials_TransmitData_Valid[ SERIALS_TRANSMITDATA_BUFFERSLOT ];
int Serials_TransmitBuffer_SlotIndex_W;
int Serials_TransmitBuffer_SlotIndex_R;

/*=====================================
 Functions definition
 =====================================*/
bool Serials_Initialization();
void* Serials_RecieveData_Pthread( void* arg );
void* Serials_TransmitData_Pthread( void* arg );
unsigned char* Serials_GetRecieveMessage();
unsigned char* Serials_GetTransmitBuffer();
void Serials_MessageRead();
void Serials_SendMessage();
bool Serials_IsNewMessageRecieved();

/*=====================================
 Implementation of functions
 =====================================*/

bool Serials_Initialization()
{
	int error_number;

	//Initialize some variables.
	for( int counter = 0; counter < SERIALS_RECIEVEDATA_BUFFERSLOT; ++counter )
	{
		Serials_RecieveData_Valid[ counter ] = false;
	}

	Serials_RecieveBuffer_SlotIndex_R = 0;

	for( int counter = 0; counter < SERIALS_TRANSMITDATA_BUFFERSLOT; ++counter )
	{
		Serials_TransmitData_Valid[ counter ] = false;
	}

	Serials_TransmitBuffer_SlotIndex_W = 0;

	//Open the serials.
	serials_fd = open( DEVICE_SERIALS, O_RDWR | O_NOCTTY | O_NDELAY );
	if( serials_fd == -1 )
	{
		printf( "Serials : Could not open serials %s.\n", DEVICE_SERIALS );
		return false;
	}

	printf( "Serials : Open serials successfully.\n" );

	//Read the attribute of the serials.
	tcgetattr( serials_fd, &serials_opt );

	//Set the baud rate of the serials.
	cfsetispeed( &serials_opt, B115200 );
	cfsetospeed( &serials_opt, B115200 );

	//Set 8 data bits to the serials.
	serials_opt.c_cflag &= ~CSIZE;
	serials_opt.c_cflag |= CS8;

	//Set no parity to the serials.
	serials_opt.c_cflag &= ~PARENB;
	serials_opt.c_iflag &= ~INPCK;

	//Set 1bit stop bit to the serials.
	serials_opt.c_cflag &= ~CSTOPB;

	//Set other general options.
	serials_opt.c_cflag |= CLOCAL | CREAD;
	serials_opt.c_lflag &= ~( ICANON | ECHO | ECHOE | ISIG );
	serials_opt.c_oflag &= ~OPOST;
	serials_opt.c_iflag &= ~( BRKINT | ICRNL | INPCK | ISTRIP | IXON );

	tcflush( serials_fd, TCIFLUSH );

	//set 5 seconds for time out.
	serials_opt.c_cc[ VTIME ] = 5;
	serials_opt.c_cc[ VMIN ] = 0;

	if( tcsetattr( serials_fd, TCSANOW, &serials_opt ) != 0 )
	{
		printf( "Serials : Set Serials options error!\n" );
		return false;
	}

	error_number = pthread_create( &serials_recievedata_tid, NULL, Serials_RecieveData_Pthread, NULL );
	if( error_number != 0 )
	{
		printf( "Serials : Serials_RecieveData_Pthread created failed.\n" );
		return false;
	}

	error_number = pthread_create( &serials_transmitdata_tid, NULL, Serials_TransmitData_Pthread, NULL );
	if( error_number != 0 )
	{
		printf( "Serials : Serials_TransmitData_Pthread created faild.\n" );
		return false;
	}

#if 0
	//just for testing.
	close( serials_fd );
#endif

	return true;
}

void* Serials_RecieveData_Pthread( void* arg )
{
	int readcounter;
	unsigned char TemporaryBuffer[ 2 ];
	int TemporaryIndex = 0;
	int StopIndex;

	printf( "Serials : Serials_RecieveData_Pthread Start.\n" );

	Serials_RecieveBuffer_SlotIndex_W = 0;

	while( true )
	{

		while( ( readcounter = read( serials_fd, TemporaryBuffer, 1 ) ) > 0 )
		{
			if( Serials_RecieveData_Valid[ Serials_RecieveBuffer_SlotIndex_W ] == true )
			{
				continue;
			}

			if( TemporaryIndex == 0 )
			{
				if( TemporaryBuffer[ 0 ] == 0xFD )
				{
					StopIndex = 8;
				}
				else if( TemporaryBuffer[ 0 ] == 0xCA )
				{
					StopIndex = 6;
				}
				else
				{
					break;
				}
			}

			if( TemporaryIndex == 1 )
			{
				if( TemporaryBuffer[ 0 ] != Serials_RecieveBuffer[ Serials_RecieveBuffer_SlotIndex_W ][ 0 ] )
				{
					TemporaryIndex = 0;
					continue;
				}
			}

			Serials_RecieveBuffer[ Serials_RecieveBuffer_SlotIndex_W ][ TemporaryIndex ] = TemporaryBuffer[ 0 ];
			++TemporaryIndex;

			if( TemporaryIndex == StopIndex )
			{
#if 1
				printf( "Serials : Recieve a new message on slot %d.\n", Serials_RecieveBuffer_SlotIndex_W );
				if( StopIndex == 6 )
				{
					printf( "Serials : Messages - %02X %02X %02X %02X %02X %02X\n", Serials_RecieveBuffer[ Serials_RecieveBuffer_SlotIndex_W ][ 0 ],
					        Serials_RecieveBuffer[ Serials_RecieveBuffer_SlotIndex_W ][ 1 ], Serials_RecieveBuffer[ Serials_RecieveBuffer_SlotIndex_W ][ 2 ],
					        Serials_RecieveBuffer[ Serials_RecieveBuffer_SlotIndex_W ][ 3 ], Serials_RecieveBuffer[ Serials_RecieveBuffer_SlotIndex_W ][ 4 ],
					        Serials_RecieveBuffer[ Serials_RecieveBuffer_SlotIndex_W ][ 5 ] );
				}
				else
				{
					printf( "Serials : Messages - %02X %02X %02X %02X %02X %02X %02X %02X\n", Serials_RecieveBuffer[ Serials_RecieveBuffer_SlotIndex_W ][ 0 ],
					        Serials_RecieveBuffer[ Serials_RecieveBuffer_SlotIndex_W ][ 1 ], Serials_RecieveBuffer[ Serials_RecieveBuffer_SlotIndex_W ][ 2 ],
					        Serials_RecieveBuffer[ Serials_RecieveBuffer_SlotIndex_W ][ 3 ], Serials_RecieveBuffer[ Serials_RecieveBuffer_SlotIndex_W ][ 4 ],
					        Serials_RecieveBuffer[ Serials_RecieveBuffer_SlotIndex_W ][ 5 ], Serials_RecieveBuffer[ Serials_RecieveBuffer_SlotIndex_W ][ 6 ],
					        Serials_RecieveBuffer[ Serials_RecieveBuffer_SlotIndex_W ][ 7 ] );
				}

#endif

				Serials_RecieveData_Valid[ Serials_RecieveBuffer_SlotIndex_W ] = true;
				Serials_RecieveBuffer_SlotIndex_W = ( Serials_RecieveBuffer_SlotIndex_W + 1 ) % SERIALS_RECIEVEDATA_BUFFERSLOT;
				TemporaryIndex = 0;

			}

		}

	}

	//This should never return.
	return ( void* )0;
}

void* Serials_TransmitData_Pthread( void* arg )
{
	int ErrorCounter = 0;

	printf( "Serials : Serials_TransmitData_Pthread Start.\n" );

	Serials_TransmitBuffer_SlotIndex_R = 0;

	while( true )
	{
		if( Serials_TransmitData_Valid[ Serials_TransmitBuffer_SlotIndex_R ] == true )
		{
			if( write( serials_fd, Serials_TransmitBuffer[ Serials_TransmitBuffer_SlotIndex_R ], SERIALS_TRANSMITDATA_BUFFERLENGTH ) == SERIALS_TRANSMITDATA_BUFFERLENGTH )
			{
				printf( "Serials : Transmit a message on slot %d successfully.\n", Serials_TransmitBuffer_SlotIndex_R );

				Serials_TransmitData_Valid[ Serials_TransmitBuffer_SlotIndex_R ] = false;
				Serials_TransmitBuffer_SlotIndex_R = ( Serials_TransmitBuffer_SlotIndex_R + 1 ) % SERIALS_TRANSMITDATA_BUFFERSLOT;
				ErrorCounter = 0;
			}
			else
			{
				printf( "Serials : Transmit a message on slot %d failed. Retry again.\n", Serials_TransmitBuffer_SlotIndex_R );
				++ErrorCounter;

				if( ErrorCounter == 3 )
				{
					printf( "Serials : Transmit failed for 3 times. Ignore this message.\n" );
					ErrorCounter = 0;
					Serials_TransmitData_Valid[ Serials_TransmitBuffer_SlotIndex_R ] = false;
					Serials_TransmitBuffer_SlotIndex_R = ( Serials_TransmitBuffer_SlotIndex_R + 1 ) % SERIALS_TRANSMITDATA_BUFFERSLOT;
				}
			}
		}
	}

	//This should never return.
	return ( void* )0;
}

unsigned char* Serials_GetRecieveMessage()
{
	if( Serials_RecieveData_Valid[ Serials_RecieveBuffer_SlotIndex_R ] == true )
	{
		return Serials_RecieveBuffer[ Serials_RecieveBuffer_SlotIndex_R ];
	}
	else
	{
		return NULL;
	}
}

unsigned char* Serials_GetTransmitBuffer()
{
	if( Serials_TransmitData_Valid[ Serials_TransmitBuffer_SlotIndex_W ] == false )
	{
		return Serials_TransmitBuffer[ Serials_TransmitBuffer_SlotIndex_W ];
	}
	else
	{
		return NULL;
	}
}

void Serials_MessageRead()
{
	if( Serials_RecieveData_Valid[ Serials_RecieveBuffer_SlotIndex_R ] == true )
	{
		Serials_RecieveData_Valid[ Serials_RecieveBuffer_SlotIndex_R ] = false;
		Serials_RecieveBuffer_SlotIndex_R = ( Serials_RecieveBuffer_SlotIndex_R + 1 ) % SERIALS_RECIEVEDATA_BUFFERSLOT;
	}
}

void Serials_SendMessage()
{
	if( Serials_TransmitData_Valid[ Serials_TransmitBuffer_SlotIndex_W ] == false )
	{
		Serials_TransmitData_Valid[ Serials_TransmitBuffer_SlotIndex_W ] = true;
		Serials_TransmitBuffer_SlotIndex_W = ( Serials_TransmitBuffer_SlotIndex_W + 1 ) % SERIALS_TRANSMITDATA_BUFFERSLOT;
	}
}

bool Serials_IsNewMessageRecieved()
{
	return Serials_RecieveData_Valid[ Serials_RecieveBuffer_SlotIndex_R ];
}

