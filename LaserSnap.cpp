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
#include "LaserSnap.h"

#include <stdlib.h>
#include <stdio.h>
#include <cmath>

/*=====================================
 Variables definition
 =====================================*/
CvPoint LaserSnap_CrossPoint[ 4 ];
double LaserSnap_Line1Slope[ 4 ];
double LaserSnap_Line2Slope[ 4 ];

/*=====================================
 Functions definition
 =====================================*/
bool LaserSnap_Probe( char* Photo, CvPoint& CrossPoint, double& Line1slope, double& Line2slope );

/*=====================================
 Implementation of functions
 =====================================*/

bool LaserSnap_Probe( char* Photo, CvPoint& CrossPoint, double& Line1slope, double& Line2slope )
{
	IplImage * testimg = NULL;
	int counter;

	int TemporaryDegree;
	double TemporarySlope;
	int TemporaryData;
	int centerpoint_x;
	int centerpoint_y;

	int FirstSlope_Index = 0;
	int SecondSlope_Index = 0;
	double FirstSlope;
	double SecondSlope;
	int FirstOffset = 0;
	int SecondOffset = 0;
	int *FirstClusterOffset = NULL;
	int *SecondClusterOffset = NULL;
	int *FirstOffsetPointer = NULL;
	int *SecondOffsetPointer = NULL;
	CvPoint CrossCenter;

	testimg = cvLoadImage( Photo, CV_LOAD_IMAGE_GRAYSCALE );

	cvThreshold( testimg, testimg, 200, 255, CV_THRESH_BINARY );

	//Find the lines.
	CvMemStorage *storage = cvCreateMemStorage( 0 );
	cvClearMemStorage( storage );
	cvCanny( testimg, testimg, 50, 100 );

	CvSeq* lines;

	lines = cvHoughLines2( testimg, storage, CV_HOUGH_PROBABILISTIC, 1,
	CV_PI / 180, 50, 50, 10 );

	//printf( "total lines: %d\n", lines->total );

	//Draw the Histogram.
	int Histogram_bins[ 180 ];	// index 0 represent -90 degree, index 90 represent 0 degree, index 179 represent 89 degree

	for( counter = 0; counter < 180; ++counter )
	{
		Histogram_bins[ counter ] = 0;
	}

	CvPoint *endpoint = ( CvPoint* )cvGetSeqElem( lines, 0 );

	for( counter = 0; counter < lines->total; counter += 2 )
	{
		if( endpoint[ counter ].x == endpoint[ counter + 1 ].x )
		{
			++Histogram_bins[ 0 ];
			continue;
		}

		TemporarySlope = ( double )( endpoint[ counter + 1 ].y - endpoint[ counter ].y ) / ( endpoint[ counter + 1 ].x - endpoint[ counter ].x );
		TemporaryDegree = ( int )( atan( TemporarySlope ) * 180 / CV_PI );
		++Histogram_bins[ TemporaryDegree + 90 ];
	}

	//select the first slope.
	FirstSlope_Index = 0;
	for( counter = 1; counter < 180; ++counter )
	{
		if( Histogram_bins[ counter ] > Histogram_bins[ FirstSlope_Index ] )
		{
			FirstSlope_Index = counter;
		}
	}

	if( Histogram_bins[ FirstSlope_Index ] == 0 )
	{
		printf( "LaserSnap : There is no line here!\n" );
		return false;
	}

	//select the second slope.
	SecondSlope_Index = ( FirstSlope_Index + 85 ) % 180;
	for( counter = 1; counter < 10; ++counter )
	{
		TemporaryData = ( FirstSlope_Index + 85 + counter ) % 180;
		if( Histogram_bins[ TemporaryData ] > Histogram_bins[ SecondSlope_Index ] )
		{
			SecondSlope_Index = TemporaryData;
		}
	}

	if( Histogram_bins[ SecondSlope_Index ] == 0 )
	{
		printf( "LaserSnap : There is only one line here!\n" );
		return false;
	}

	//Cluster the offset.
	FirstClusterOffset = new int[ Histogram_bins[ FirstSlope_Index ] ];
	FirstOffsetPointer = FirstClusterOffset;
	SecondClusterOffset = new int[ Histogram_bins[ SecondSlope_Index ] ];
	SecondOffsetPointer = SecondClusterOffset;

	if( FirstSlope_Index )
	{
		FirstSlope = tan( ( double )( FirstSlope_Index - 90 ) / 180 * CV_PI );
	}
	if( SecondSlope_Index )
	{
		SecondSlope = tan( ( double )( SecondSlope_Index - 90 ) / 180 * CV_PI );
	}
	for( counter = 0; counter < lines->total; counter += 2 )
	{
		if( FirstSlope_Index == 0 )
		{
			//Slope does not exist.
			if( endpoint[ counter ].x == endpoint[ counter + 1 ].x )
			{
				*FirstOffsetPointer = endpoint[ counter ].x;
				++FirstOffsetPointer;
			}
			continue;
		}

		if( SecondSlope_Index == 0 )
		{
			//Slope does not exist.
			if( endpoint[ counter ].x == endpoint[ counter + 1 ].x )
			{
				*SecondOffsetPointer = endpoint[ counter ].x;
				++SecondOffsetPointer;
			}
			continue;
		}

		//Slope does exist;
		TemporarySlope = ( double )( endpoint[ counter + 1 ].y - endpoint[ counter ].y ) / ( endpoint[ counter + 1 ].x - endpoint[ counter ].x );
		TemporaryDegree = ( int )( atan( TemporarySlope ) * 180 / CV_PI );

		if( TemporaryDegree == FirstSlope_Index - 90 )
		{
			centerpoint_x = ( endpoint[ counter ].x + endpoint[ counter + 1 ].x ) / 2;
			centerpoint_y = ( endpoint[ counter ].y + endpoint[ counter + 1 ].y ) / 2;

			*FirstOffsetPointer = ( int )( centerpoint_y - FirstSlope * centerpoint_x );
			++FirstOffsetPointer;
			continue;
		}

		if( TemporaryDegree == SecondSlope_Index - 90 )
		{
			centerpoint_x = ( endpoint[ counter ].x + endpoint[ counter + 1 ].x ) / 2;
			centerpoint_y = ( endpoint[ counter ].y + endpoint[ counter + 1 ].y ) / 2;

			*SecondOffsetPointer = ( int )( centerpoint_y - SecondSlope * centerpoint_x );
			++SecondOffsetPointer;
			continue;
		}

	}

	//temporary set the FirstClusterOffset to the average value.
	FirstOffset = 0;
	for( counter = 0; counter < Histogram_bins[ FirstSlope_Index ]; ++counter )
	{
		FirstOffset += FirstClusterOffset[ counter ];
	}
	FirstOffset /= Histogram_bins[ FirstSlope_Index ];

	//temporary set the SecondClusterOffset to the average value.
	SecondOffset = 0;
	for( counter = 0; counter < Histogram_bins[ SecondSlope_Index ]; ++counter )
	{
		SecondOffset += SecondClusterOffset[ counter ];
	}
	SecondOffset /= Histogram_bins[ SecondSlope_Index ];

	//Print Out the formular.
	if( FirstSlope_Index )
	{
		printf( "LaserSnap : Formular 1 : %lf  * x - y + %d = 0 \n", FirstSlope, FirstOffset );
	}
	else
	{
		printf( "LaserSnap : Formular 1 : x = %d\n", -FirstOffset );
	}

	if( SecondSlope_Index )
	{
		printf( "LaserSnap : Formular 2 : %lf  * x - y + %d = 0\n", SecondSlope, SecondOffset );
	}
	else
	{
		printf( "LaserSnap : Formular 2 : x = %d\n", -SecondOffset );
	}

	//calculate the cross center point.
	if( FirstSlope_Index )
	{
		if( SecondSlope_Index )
		{
			CrossCenter.x = ( SecondOffset - FirstOffset ) / ( FirstSlope - SecondSlope );
			CrossCenter.y = ( FirstSlope * SecondOffset - SecondSlope * FirstOffset ) / ( FirstSlope - SecondSlope );
		}
		else
		{
			//the second slope does not exist.
			CrossCenter.x = SecondOffset;
			CrossCenter.y = FirstSlope * CrossCenter.x + FirstOffset;
		}
	}
	else
	{
		//the first slope does not exist.
		CrossCenter.x = FirstOffset;
		CrossCenter.y = SecondSlope * CrossCenter.x + SecondOffset;
	}

	//print out the cross center point.
	printf( "LaserSnap : CrossCenterPoint = ( %d, %d ).\n", CrossCenter.x, CrossCenter.y );

	delete FirstClusterOffset;
	delete SecondClusterOffset;
	FirstClusterOffset = NULL;
	SecondClusterOffset = NULL;

	cvReleaseImage( &testimg );
	testimg = NULL;

	return true;
}

