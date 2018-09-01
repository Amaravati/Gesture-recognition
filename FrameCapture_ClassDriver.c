/*
 ****************************************************************************
 Copyright (c) 2015 - Analog Devices Inc. All Rights Reserved.
 This software is proprietary & confidential to Analog Devices, Inc.
 and its licensors.
 ******************************************************************************

 Title: Frame Capture

 Description:
 
 This file contains functions for capturing the video from the sensor. 
 They demonstrate the functionality of sensors using Video Class Driver.
   
 The example receives 'N' video frames and writes to a .dat file.
 The value 'N' is set by macro 'NUM_FRAME_NUM_TO_CAPTURE'. 
 The '.dat" filename is specified by macro 'FRAME_CAPTURE_FILENAME'.
 The file is created in the project folder.
  
 *****************************************************************************/

/*=============  I N C L U D E S   =============*/
//#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <adi_types.h>
#include <services/pwr/adi_pwr.h>
#include <drivers/twi/adi_twi.h>
#include <services/tmr/adi_tmr.h>
#include <services/gpio/adi_gpio.h>
#include <drivers/device_class/adi_dev_video_class.h>
#include "adi_initialize.h"
#include "FrameCapture_ClassDriver.h"
#include <cplbtab.h>
#include <sys/exception.h>
#include <time.h>
#include <math.h>

/*=============  D A T A  =============*/
#define Inf 10000
#pragma align 4
#pragma section("sdram0_bank1")
static uint8_t TimerMemory[ADI_TMR_MEMORY];

/* Timer handle */
static ADI_TMR_HANDLE ghTimer;

/* Capture buffers. */
#pragma align(32)
#pragma section ("sdram0_bank0")
static uint8_t gaVideoBufferA[VIDEO_BUF_SIZE];

#pragma align(32)
#pragma section ("sdram0_bank1")
static uint8_t gaVideoBufferB[VIDEO_BUF_SIZE];

#pragma optimize_for_space

struct DTWInfo{
    double dist;

};

struct mc{
	 int x;
	 int y;
};



struct Gesture
{
	char label;
	int len;
	double* x;
	double* y;
};


struct TrainingData
{
	struct Gesture* G;
	int N;
};

struct mcent{
	double x[buflen];
	double y[buflen];

};


struct node
{
    int value;
    struct node* next;

};


struct GestureClassifiers{
	double weights[3][3];
	double lin[1][3];
	double Biases[5];
	char Labels[5];
	int N;
};


struct smfilt{

	short int arr1[nmeas][1];

};


struct MCBuffer
{
	short int* Bufx;
	short int* Bufy;
	short int len;
};

int *pairmatch_s, *pairmatch_t, *pairmatch_s1, *pairmatch_t1;
/*=============  F U N C T I O N =============*/

/*
 **  Function Prototype section
 */


static ADI_DEV_VIDEO_HANDLE OpenAndConfigureSensor(ADI_DEV_VIDEO_FORMAT eInputVideoFormat,
                                                   int32_t              eInputUserDataFormat);
static int32_t ProcessVideoFrame(FILE *pOutFile,void *pCapturedFrame,
                                 uint32_t nFrameSizeInBytes);
static int32_t ProcessVideoFrame1(FILE *pOutFile,void *pCapturedFrame,
                                 uint32_t nFrameSizeInBytes);
static int32_t ResetSensor(void);
static int32_t InitTimer(uint32_t nTimerNum);
static uint32_t CloseTimer(void);
static void TimerHandler(void *pCBParam, uint32_t Event, void *pArg);
static int32_t ShutSensor(void);
static void Delay(uint32_t nCount);
static int32_t PowerInit(void);
static int32_t CloseSensor(ADI_DEV_VIDEO_HANDLE hSensorHandle);
int rand(void);
void printMatrix(short int *, int, int);
void matmul(short int*, short int*, short int*,int,int,int);
int max_array(int a[], int num_elements);


struct MCBuffer initMCBuffer(short int len);
void addtoMCBuffer(struct MCBuffer MCBuf, short int a, short int b);
#pragma section ("sdram0_bank0")
struct TrainingData getTrData(double** gx, double** gy, int* len, int N);
void clearTrData(struct TrainingData Gtr);
void printgval(double *g_x, double *g_y, double *a, double *b);
void cpy1(struct node* head, struct node* head1);
void pushBack(struct node** head, int value);
void swap(int *arr1, int *arr2);
//struct DTWInfo getDTW(double* s, double *s1, double* t, double *t1, int len_s, int len_t, int local_lim);
double getDTW(double* s, double *s1, double* t, double *t1, int len_s, int len_t, int local_lim);
void dtwScaling(double* src_x, double* src_y, double* tgt_x, double* tgt_y, double* dst_x, double* dst_y, int idx);
double * applyPCA(double * pca_basis, double * pca_mean, double* MCx, double* MCy, int len, int dim);
double * mulmatrix1(double *a, double *b, int rsize, int csize);
char recogGesture1(struct mcent m1, double** g_x, double** g_y, double * pca_basis, double * pca_mean, struct GestureClassifiers *GesC);
int getStackSize(struct node *head);
void pop(struct node *head);
void scp(int *a, int *b);
double * mpy(double *a, double *b, int rsize, int csize);

#pragma section ("sdram0_bank0")
uint8_t rmatrix[rws][ncs];

#pragma section ("sdram0_bank1")
uint8_t rmatrixf1[rws][ncs];

#pragma section ("sdram0_bank1")
uint8_t rmatrixd[rws][ncs];

#pragma section ("sdram0_bank1")
uint8_t rmatrix1[nobyt];

#pragma section ("sdram0_bank0")
 short int random[nmeas][nobyt];

#pragma section ("sdram0_bank0")
 int * mulmatrix(short int *, uint8_t *, int, int,int );

#pragma section ("sdram0_bank0")
 int * matnew;

//#pragma section ("sdram0_bank1")


#pragma section ("sdram0_bank1")
 short int cmatrix[nobyt][1];


// short int arr1[nmeas][1];

 #pragma section ("sdram0_bank1")
 struct mc mtc[3000];


#pragma section ("sdram0_bank1")

int arrm[1000];

#pragma section ("sdram0_bank1")
struct smfilt sm[2000];

#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))

int memlen=buflen;
int var, var1;
int var2[5];


/*
 **  Function Definition section
 */


/**
 * Main function to dump the frames captured by the sensor.
 * This function uses class driver calls to configure the sensor.
 * The function captures 'N' frames and these frames are dumped to the output
 * file.
 *
 * @return 0 - Success,1 - Failure
 */


int32_t main(void)
{
    int32_t nResult = 0;
    uint16_t nNumFramesCaptured = 0;
    ADI_DEV_VIDEO_HANDLE hSensorHandle = NULL;
    FILE *pOutFile = NULL;
    FILE *pOutFile1 = NULL;
    void *pCapturedFrame = NULL;
    void *pCapturedFrame1 = NULL;
    void *pCapFramediff = NULL;
    ADI_DEV_VIDEO_FRAME_PROPERTIES *poFrameProperties = NULL;
    uint32_t nFrameSizeInBytes = 0;
    ADI_DEV_VIDEO_FORMAT eInputVideoFormat = CAPTURE_VIDEO_FORMAT;
    int32_t   eInputUserDataFormat = CAPTURE_VIDEO_DATA_FORMAT;
    int rsize = 640, csize=480;
    long int i=0,j=0,k=0,l,nyc=0;
    short int  *matrix, *filter, *temp, m,n,filterDirection = 1, filtTop = 0, filtLeft = 0;
    short int filtRows[3]={10,11,10};
    short int filtCols[3]={10,6,13};
    int index;
    //struct TrainingData Gtr;

	extern double* g_x[];
	extern double* g_y[];
	extern int g_len[];
	extern char gc_Labels[];
	extern int g_N;

	struct mcent m1;

	extern double pca_basis[140][140];
    extern double  pca_mean[140];
    struct GestureClassifiers ges[5];
	 extern double  gc_Weights_Quad1[3][3];
	 extern double  gc_Weights_Quad2[3][3];
	 extern double  gc_Weights_Quad3[3][3];
	 extern double  gc_Weights_Quad4[3][3];
	 extern double  gc_Weights_Quad5[3][3];


	 extern double gc_Weights_Lin1[3][1];
	 extern double gc_Weights_Lin2[3][1];
	 extern double gc_Weights_Lin3[3][1];
	 extern double gc_Weights_Lin4[3][1];
	 extern double gc_Weights_Lin5[3][1];
	 extern double gc_Biases[5];
	 extern char gc_Labels[5];

	// int i,j,k;
	 for(k=0;k<5;k++){
    for(i=0;i<3;i++){
   	 for(j=0;j<3;j++){
   		 ges[0].weights[i][j]=gc_Weights_Quad1[i][j];
   		 ges[1].weights[i][j]=gc_Weights_Quad2[i][j];
   		 ges[2].weights[i][j]=gc_Weights_Quad3[i][j];
   		 ges[3].weights[i][j]=gc_Weights_Quad4[i][j];
   		 ges[4].weights[i][j]=gc_Weights_Quad5[i][j];
            //printf("%f ", ges[k].weights[i][j]);
   	 }

       }
    printf("\n");
    for(i=0;i<1;i++){
    for(j=0;j<3;j++){
    	ges[0].lin[i][j]=gc_Weights_Lin1[i][j];
    	ges[1].lin[i][j]=gc_Weights_Lin2[i][j];
    	ges[2].lin[i][j]=gc_Weights_Lin3[i][j];
    	ges[3].lin[i][j]=gc_Weights_Lin4[i][j];
    	ges[4].lin[i][j]=gc_Weights_Lin5[i][j];
    	//printf("%f ", ges[k].lin[i][j]);
    }

    }
    //printf("\n");
        ges[0].Biases[k]=gc_Biases[k];
        ges[0].Labels[k]=gc_Labels[k];
       printf("%c ", ges[0].Labels[k]);
	 }
	 ges[0].N=3;

    //struct TrainingData Gtr = getTrData(g_x, g_y, g_len, g_N);

		extern double pca_basis[140][140];
	    extern double  pca_mean[140];
    /**
     * Initialize managed drivers and/or services that have been added to 
     * the project.
     * @return zero on success 
     */
    adi_initComponents();

    /* Power initialization. */
    nResult = PowerInit();

    /* Open the output file. */


    pOutFile = fopen(CAPTURE_OUTPUT_FILENAME,"wb");
    pOutFile1 = fopen(CAPTURE_OUTPUT_FILENAME1,"wb");
    if (pOutFile == NULL)
    {
        printf("Error opening output file %s\n",CAPTURE_OUTPUT_FILENAME);
        return 1;
    }

    /* Open and configure the sensor. */
    hSensorHandle = OpenAndConfigureSensor(eInputVideoFormat,eInputUserDataFormat);
    if (hSensorHandle == NULL)
    {
        printf("Error in configuring the sensor.\n"); 
        return 1;
    }

    /* Get size of the frame. */
    nResult = adi_dev_Video_Control(hSensorHandle,
            ADI_DEV_VIDEO_CMD_GET_FRAME_PROPERTIES,(void*)&poFrameProperties);
    if (nResult != ADI_DEV_CLASS_RESULT_SUCCESS)
    {
        printf("adi_dev_Video_Control(Getting frame properties), failed\n");
        return 1;
    }

    if(poFrameProperties == NULL)
    {
        printf("adi_dev_Video_Control(Getting frame properties) returns NULL pointer \n");
        return 1;
    }

    nFrameSizeInBytes = poFrameProperties->nLinesPerFrame*
                        poFrameProperties->nPixelsPerLine*
                        poFrameProperties->nBytesPerPixel;

    /* Submit Video buffer A to sensor */
    nResult = adi_dev_Video_SubmitFrame(hSensorHandle,&gaVideoBufferA[0]);
    if (nResult != ADI_DEV_CLASS_RESULT_SUCCESS)
    {
        printf("adi_dev_Video_SubmitFrame (Frame A) failed, Error Code: 0x%08X\n",nResult);
        return 1;
    }

    /* Submit Video buffer B to sensor */
    nResult = adi_dev_Video_SubmitFrame(hSensorHandle,&gaVideoBufferB[0]);
    if (nResult != ADI_DEV_CLASS_RESULT_SUCCESS)
    {
        printf("adi_dev_Video_SubmitFrame (Frame B) failed, Error Code: 0x%08X\n",nResult);
        return 1;
    }

    /* Enable sensor flow. */
    nResult = adi_dev_Video_Control (hSensorHandle,ADI_DEV_VIDEO_CMD_ENABLE,(void*)true);
    if (nResult != ADI_DEV_CLASS_RESULT_SUCCESS)
    {
        printf("adi_dev_Video_Control (Enable) failed, Error Code: 0x%08X\n", nResult);
        return 1;
    }




    for(i = 0; i<nmeas; i++){
         for(j = 0; j<nobyt; j++){
      	   random[i][j] = rand()%10>5?-1:1;
         }
     }

    n=0;
    m=1;

    	matrix = (short int *) malloc(sizeof(short int) * matCols * matRows);
    	temp=matrix;
    	filterDirection = 1, filtTop = 0, filtLeft = 0;

        for (i = 0; i < matRows; i++){
            for (j = 0; j < matCols; j++){
            	matrix[(matCols * i) + j] = 0;
            }
        }


       int nr;
       nr=100/(filtRows[m]*filtCols[m]);
        // k to restrict infinite loop
        k = 0;
        while (k < 1000){
            // loop through and create g matrices
            for (i = 0; i < matRows; i++){
                for (j = 0; j < matCols; j++){

                    if (j >= filtLeft && j < (filtLeft + filtCols[m]) && i >= filtTop && i < (filtTop + filtRows[m]))
                        matrix[(matCols * i) + j] = nr;
                    else
                        matrix[(matCols * i) + j] = 0;
                }
            }

    l=0;
           //  printf("FiltTop and filtLeft are:%d & %d \n",filtLeft, filtTop);


             mtc[n].y=(filtRows[m]/2+filtTop);
             mtc[n].x=40-(filtCols[m]/2+filtLeft);
    		//mtc[n].y=30-(filtRows[m]/2+filtTop);
    		//mtc[n].x=(filtCols[m]/2+filtLeft);


           //  printf("%d & %d\n", mtc[n].x, mtc[n].y);

             for (i = 0; i < matRows; i++){
                 for (j = 0; j < matCols; j++){
                     cmatrix[l][0]=matrix[(matCols * i) + j] ;
                  // printf("%d",cmatrix[l][0]);

                      l++;
                 }
              }
           // printf("\n\n");

              matmul(&sm[k].arr1[0][0],&random[0][0],&cmatrix[0][0],nmeas,nobyt,1);
               for(i=0;i<nmeas;i++){

                     // printf("%d ", sm[k].arr1[i][0]);

               }

              // printf("\n\n");

            //printMatrix(matrix, matRows, matCols);

           // printf("\n\n");
//            // determine the direction filter has to shift next
            if (filtLeft + filtCols[m] >= matCols){
                // it cant go right anymore
                if (filterDirection == 1){
                    // previously it went right, so it has to go down now
                    if ((filtTop + filtRows[m]) >= matRows){
                        // filter is at the bottom right corner and cant move down or right
                        break;
                    } else {
                        // filter is at right and can move down
                        filterDirection = 0;
                        filtTop++;
                    }
                } else {
                    // previously it went down, so it has to go left now
                    if ((matCols - filtLeft) > 0){
                        // filter can move left
                        filterDirection = -1;
                        filtLeft--;
                    } else if ((filtTop + filtRows[m]) < matRows){
                        // filter cant go left either, case of single column matrix. it has move down again
                        filtTop++;
                    } else {
                        // it cant move left, right or down
                        break;
                    }
                }
            } else if (filtLeft == 0){

                // filter is full left
                if (filterDirection == 0){
                    // filter went down previously, move right now
                    filtLeft++;
                    filterDirection = 1;
                } else if (filterDirection == 1){
                    // initially starts from top left and moves right
                    filtLeft++;
                } else {
                    // filter went left previously, move down
                    if ((filtTop + filtRows[m]) >= matRows){
                        // filter is bottom left corner
                        break;
                    } else {
                        filterDirection = 0;
                        filtTop++;
                    }
                }
            } else {
                // filter is somewhere in middle, keep going in same direction
                if (filterDirection == -1)
                    filtLeft--;
                else
                    filtLeft++;
            }

            k++;n++;

        }

        printf("Final k and n values:%d & %d\n",k,n);
        free(temp);

        short int filtlen=k;


       FILE *fpn;
       fpn=fopen("motion_centerx.txt","w");
       FILE *fpn1;
       fpn1=fopen("motion_centery.txt","w");

    uint8_t mrr[buflen][2];

    while (nNumFramesCaptured != NUM_FRAME_NUM_TO_CAPTURE)
    {

      i=0;
      clock_t begin, end;
      double time_spent;
      begin = clock();
    	        /* Get video frame. */
        nResult = adi_dev_Video_GetFrame(hSensorHandle,&pCapturedFrame);

        if ((nResult != ADI_DEV_CLASS_RESULT_SUCCESS) || (pCapturedFrame == NULL))
        {
            printf("adi_dev_Video_GetFrame failed, Error Code: 0x%08X\n",nResult);
            return 1;
        }

        srand((unsigned int)time(NULL));

    /*find the difference matrix*/

        uint8_t *a=(uint8_t *) pCapturedFrame;

        if(nNumFramesCaptured%2==0){k=0,l=0;
        	for (i = 0; i < 480; i++){
        	 if(i%16==0){
        		 l=0;
        		 	 for(j=0;j<640;j++){
        		 if(j%16==0){
        			 rmatrix[k][l]= *(a +640*i+j) ;
                     l++;
        		 }

        	 }
             k++;
        	 }

         }
        }


              if(nNumFramesCaptured%2==1){k=0,l=0;
             	for (i = 0; i < 480; i++){

            	 if(i%16==0){
            	 l=0;
            		 	 for(j=0;j<640;j++){
            		 if(j%16==0){
            			 rmatrixf1[k][l]= *(a +640*i+j) ;
                        l++;
            		 }
            	 }
                 k++;
            	 }
              }
          }


              for(i=0;i<rws;i++)
                   	for(j=0;j<ncs;j++)
                   	rmatrixd[i][j]=abs(rmatrix[i][j]-rmatrixf1[i][j]);

         k=0;
         for(i=0;i<rws;i++){
        	 for(j=0;j<ncs;j++){
        		// if(j%2==1){
        		 rmatrix1[k]=rmatrixd[i][j];
        		 k++;
        		// }
        	 }
         }

         int z;
//          while(z<10000){
//        	  z++;
//          }

         matnew=mulmatrix(random, rmatrix1, nmeas, nobyt, 1);
         short int p;
//         arrm[p]=0;
         for(p=0;p<filtlen;p++){
       for(i=0;i<nmeas;i++){
             arrm[p]+=(sm[p].arr1[i][0]**(matnew+i))/2;
               }
       arrm[p]=arrm[p]/32;
      // printf("%d \n", arrm[p]);
         }



         //printf("Value of i, j & k :%d, %d & %d", i,j,k);

         int numbytes=rws*ncs;



        /* Process the frame. */

         index = max_array(arrm, filtlen);
         m1.x[nNumFramesCaptured]= (double)mtc[index].x;
         m1.y[nNumFramesCaptured]= (double)mtc[index].y;
        // printf("The motion centers are %d & %d\n", m1.x[nNumFramesCaptured],m1.y[nNumFramesCaptured]);

         //mrr[nNumFramesCaptured][0]=m1.x[nNumFramesCaptured];
         //mrr[nNumFramesCaptured][1]=m1.y[nNumFramesCaptured];


         for(i=0;i<1;i++){
              //fprintf(fpn,"%u",mrr[nNumFramesCaptured][i]);
              fprintf(fpn,"%f ",m1.x[nNumFramesCaptured]);
              fprintf(fpn1,"%f ",m1.y[nNumFramesCaptured]);
              fprintf(fpn,"\n");
                 }


        // nResult = ProcessVideoFrame(pOutFile1,&rmatrix,numbytes);
        // nResult = ProcessVideoFrame(pOutFile,&rmatrix1,numbytes);


        if (nResult != 0)
       {
           printf("Error processing output.\n");
           return 1;
       }


        /* Submit frame. */
        nResult = adi_dev_Video_SubmitFrame(hSensorHandle,pCapturedFrame);

        if (nResult != ADI_DEV_CLASS_RESULT_SUCCESS)
        {
            printf("adi_dev_Video_SubmitFrame failed, Error Code: 0x%08X\n",nResult);
            return 1;
        }



        /* Increment captured frame number */

        end = clock();
        time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        //printf("Time spent is: %f\n", time_spent);

        nNumFramesCaptured++;

//        printf("Finished capturing Frame %d\n",nNumFramesCaptured);
        free(matnew);
        //free(pairmatch_s);
        //free(pairmatch_t);
    }



	short int recog_thresh = 250;
	double test[5];
    //pairmatch_s = (int * ) malloc( 70 * sizeof(int));
    //pairmatch_t = (int * ) malloc( 70 * sizeof(int));
    //printgval(g_x[0], g_y[0], m1.x, m1.y);
    int q=0;

    /*
    for(q=0;q<5;q++){
	test[q]=getDTW(g_x[q], g_y[q], m1.x, m1.y, 70, 20, 0);
	printf("distance is %f ", test[q]);
    }
*/
    pairmatch_s = (int * ) malloc( 100 * sizeof(int));
    pairmatch_t = (int * ) malloc( 100 * sizeof(int));
    pairmatch_s1 = (int * ) malloc( 100 * sizeof(int));
    pairmatch_t1 = (int * ) malloc( 100 * sizeof(int));
    double time_spent;
    clock_t begin, end;
    begin = clock();

	//printf("distance is %f ", test);

    char g =  recogGesture1(m1, g_x, g_y, pca_basis, pca_mean, ges);
    printf("Gesture recognized:%c \n", g);
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Time spent is: %f\n", time_spent);

    free(pairmatch_s);
    free(pairmatch_t);
    free(pairmatch_s1);
    free(pairmatch_t1);


//    clearTrData(Gtr);

    /* Disable the data flow. */
    nResult = adi_dev_Video_Control(hSensorHandle,
            ADI_DEV_VIDEO_CMD_ENABLE,(void*) false
            );
    if (nResult != ADI_DEV_CLASS_RESULT_SUCCESS)
    {
        printf("adi_dev_Video_Control(Disabling device) failed \n"); 
        return 1;
    }

    /* Close the sensor. */
    nResult = CloseSensor(hSensorHandle);

    /* Close the file. */
    if (fclose(pOutFile) != 0)
    {
        printf("Error closing the output file.\n"); 
        return 1;
    }

    printf("Output file %s is generated.\n",CAPTURE_OUTPUT_FILENAME); 
    printf("All Done.\n"); 
    fclose(fpn);
    fclose(fpn1);
    return 0;
}

/**
 * This function opens the sensor and configures it to capture 
 * in desired video format.
 *
 * @return Video Handle - Success,NULL - Failure
 */
static ADI_DEV_VIDEO_HANDLE OpenAndConfigureSensor(ADI_DEV_VIDEO_FORMAT eInputVideoFormat,
                                                   int32_t              eInputUserDataFormat)
{
    uint32_t nResult = 0;
    const ADI_DEV_VIDEO_CLASS_ENTRY_POINT *poSensorEntryPoint = SENSOR_ENTRY_POINT;
    uint32_t nFrameRate = CAPTURE_VIDEO_FRAME_RATE;
    uint32_t nCapFrameRate = 0;
    ADI_DEV_VIDEO_HANDLE hSensorHandle = NULL;
    ADI_DEV_VIDEO_SIGNAL eVidSignal = ADI_DEV_VIDEO_SIGNAL_NONE;

    /* Shutdown the sensor */
    /* If multiple sensors are connected to the same port,
     * The sensor with highest priority may block the sensor at a lower 
     * priority. Hence, the sensors are shut down before initialization. */
    nResult = ShutSensor();
    if (nResult != 0)
    {
        printf("Error in shutting down the sensor.\n"); 
        return NULL;
    }

    /* Initialize timer. */
    nResult = InitTimer(SENSOR_TIMER);
    if (nResult != 0)
    {
        printf("Error in initializing the timer.\n"); 
        return NULL;
    }

    /* Reset the sensor. */
    nResult = ResetSensor();
    if (nResult != 0)
    {
        printf("Error in resetting the sensor.\n"); 
        return NULL;
    }

    /* Open the appropriate device using class drivers */
    nResult = adi_dev_Video_Open(poSensorEntryPoint,0,&hSensorHandle);
    if (nResult != ADI_DEV_CLASS_RESULT_SUCCESS)
    {
        printf("adi_dev_Video_Open failed \n"); 
        return NULL;
    }

    /* Set the video data format */
    switch(eInputUserDataFormat)
    {
        case ADI_VIDEO_IN_RAW422_PROG:
            nResult = adi_dev_Video_Control(
                                            hSensorHandle,
                                            ADI_DEV_VIDEO_CMD_SET_VIDEO_DATA_FORMAT,
                                            (void*)ADI_DEV_VIDEO_DATA_UYVY422
                                           );
        break;
        case ADI_VIDEO_IN_RGB565:
            nResult = adi_dev_Video_Control(
                                            hSensorHandle,
                                            ADI_DEV_VIDEO_CMD_SET_VIDEO_DATA_FORMAT,
                                            (void*)ADI_DEV_VIDEO_DATA_RGB565
                                           );
        break;
        case ADI_VIDEO_IN_Y:
            nResult = adi_dev_Video_Control(
                                            hSensorHandle,
                                            ADI_DEV_VIDEO_CMD_SET_VIDEO_DATA_FORMAT,
                                            (void*)ADI_DEV_VIDEO_DATA_Y
                                           );
        break;
    }
    if (nResult != ADI_DEV_CLASS_RESULT_SUCCESS)
    {
        printf("ERROR: Video Capture: adi_dev_Video_Control(ADI_DEV_VIDEO_CMD_SET_VIDEO_DATA_FORMAT) failed \n");
        return NULL;
    }

    /* Set the video format */
    if( (eInputVideoFormat == ADI_DEV_VIDEO_FORMAT_720X576I) ||
        (eInputVideoFormat == ADI_DEV_VIDEO_FORMAT_640X480P) ||
        (eInputVideoFormat == ADI_DEV_VIDEO_FORMAT_720X480I))
    {
        nResult = adi_dev_Video_Control(hSensorHandle,
                ADI_DEV_VIDEO_CMD_SET_VIDEO_FORMAT,(void*)eInputVideoFormat);
        if (nResult != ADI_DEV_CLASS_RESULT_SUCCESS)
        {
            printf("adi_dev_Video_Control(Setting video format), failed\n");
            return NULL;
        }
    }
    else
    {
        printf("This Video format is not supported, failed\n");
        return NULL;
    }

    /* Set frame rate. */
    nResult = adi_dev_Video_Control(hSensorHandle,
            ADI_DEV_VIDEO_CMD_SET_VIDEO_FRAME_RATE,(void*)nFrameRate);
    if (nResult != ADI_DEV_CLASS_RESULT_SUCCESS)
    {
        printf("adi_dev_Video_Control(Setting video frame rate), failed\n"); 
        return NULL;
    }

    /* Get sensor configuration */
    /* Video format. */
    nResult = adi_dev_Video_Control(hSensorHandle,
            ADI_DEV_VIDEO_CMD_GET_VIDEO_SIGNAL_FORMAT,&eVidSignal);
    if (nResult != ADI_DEV_CLASS_RESULT_SUCCESS)
    {
        printf("adi_dev_Video_Control(Getting video format), failed\n"); 
        return NULL;
    }
    if ( (eVidSignal != ADI_DEV_VIDEO_SIGNAL_NTSC_M_J) &&
         (eVidSignal != ADI_DEV_VIDEO_SIGNAL_PAL_BGHID) &&
         (eVidSignal != ADI_DEV_VIDEO_SIGNAL_VGA_640X480P) &&
         (eVidSignal != ADI_DEV_VIDEO_SIGNAL_NTSC_443))
    {
        printf("Error:Video Input Signal Not Found \n");
        return NULL;
    }

    /* Frame rate. */
    nResult = adi_dev_Video_Control(hSensorHandle,
                                    ADI_DEV_VIDEO_CMD_GET_VIDEO_FRAME_RATE,
                                    &nCapFrameRate);
    if (nResult != ADI_DEV_CLASS_RESULT_SUCCESS)
    {
        printf("adi_dev_Video_Control(Getting video frame rate), failed\n"); 
        return NULL;
    }

    /* Print sensor configuration info */
    switch (eVidSignal)
    {
        case ADI_DEV_VIDEO_SIGNAL_NTSC_M_J:
            printf("NTSC M/J @ %dFPS\n",nCapFrameRate);
            break;

        case ADI_DEV_VIDEO_SIGNAL_NTSC_443:
            printf("NTSC 443 @ %dFPS\n",nCapFrameRate);
            break;

        case ADI_DEV_VIDEO_SIGNAL_PAL_BGHID:
            printf("PAL B/G/H/I/D @ %dFPS\n",nCapFrameRate);
            break;

        case ADI_DEV_VIDEO_SIGNAL_VGA_640X480P:
            printf("VGA 640x480 @ %dFPS\n",nCapFrameRate);
            break;

        default:
            printf("Unable to find valid input signal\n"); 
            return NULL;
    }

    return hSensorHandle;

}


/**
 * This function processes the captured video frame.
 * The frame is dumped to the output file.
 *
 * @param pOutFile - Output file pointer.
 * @param pCapturedFrame - Pointer to captured frame buffer.
 * @param nFrameSizeInBytes - Frame size in bytes.
 *
 * @return 0 - Success,1 - Failure
 */
static int32_t ProcessVideoFrame(FILE *pOutFile,void *pCapturedFrame,
                                 uint32_t nFrameSizeInBytes)
{
    uint32_t nResult = 0;
    uint32_t nNumBytesWritten;

    /* Write bytes to output file. */
    nNumBytesWritten = fwrite(pCapturedFrame,sizeof(uint8_t),
            nFrameSizeInBytes,pOutFile);
    if (nNumBytesWritten != nFrameSizeInBytes)
    {
        printf("Error writing bytes to output file.\n");
        return 1;
    }

    return 0;
}


static int32_t ProcessVideoFrame1(FILE *pOutFile,void *pCapturedFrame,
                                 uint32_t nFrameSizeInBytes)
{
    uint32_t nResult = 0;
    uint32_t nNumBytesWritten;

    /* Write bytes to output file. */
    nNumBytesWritten = fwrite(pCapturedFrame,sizeof(short int),
            nFrameSizeInBytes,pOutFile);
    if (nNumBytesWritten != nFrameSizeInBytes)
    {
        printf("Error writing bytes to output file.\n");
        return 1;
    }

    return 0;
}

/**
 * This function shut downs the sensor. 
 * @return 0 - Success,1 - Failure
 */
static int32_t ShutSensor(void)
{

    ADI_GPIO_RESULT nResult = ADI_GPIO_SUCCESS;

#ifdef __ADSPBF707_BLIP2__
    /* Blip2 has two sensors, shut them down. */
    /* Both the sensors have to be shutdown as they use 
     * the same EPPI port. */

    /* Aptina ASX340. */
    /* set GPIO output PWDN of sensor*/
    if((nResult = adi_gpio_SetDirection(ADI_GPIO_PORT_C,
                    ADI_GPIO_PIN_13,
                    ADI_GPIO_DIRECTION_OUTPUT)) != ADI_GPIO_SUCCESS)
    {
        printf("Error Setting GPIO Direction(ASX340) \n");
        return 1;
    }

    if((nResult = adi_gpio_Clear(ADI_GPIO_PORT_C,
                    ADI_GPIO_PIN_13) )!= ADI_GPIO_SUCCESS)
    {
        printf("Error Setting GPIO pin(ASX340) \n");
        return 1;
    }
    Delay(0x1FF);

    /* Omnivision OV7692. */
    /* set GPIO output PWDN of sensor*/
    if((nResult = adi_gpio_SetDirection(ADI_GPIO_PORT_A,
                    ADI_GPIO_PIN_5,
                    ADI_GPIO_DIRECTION_OUTPUT)) != ADI_GPIO_SUCCESS)
    {
        printf("Error Setting GPIO Direction(OV7692) \n");
        return 1;
    }

    if((nResult = adi_gpio_Set(ADI_GPIO_PORT_A,
                    ADI_GPIO_PIN_5) )!= ADI_GPIO_SUCCESS)
    {
        printf("Error Setting GPIO pin(OV7692) \n");
        return 1;
    }
    Delay(0x1FF);
#endif /* __ADSPBF707_BLIP2__ */


    return 0;
}


/**
 * Timer event handler 
 *
 * @param pCBParam - Pointer to Callback parameter 
 * @param Event - Callback Event
 * @param pArg - Pointer to the event specific argument
 */
static void TimerHandler(void *pCBParam, uint32_t Event, void *pArg)
{
    switch(Event)
    {
        case ADI_TMR_EVENT_DATA_INT:
            /* Handle the timer event here */
            break;

        default:
            break;
    }

    return;
}

/**
 * Closes the timer.
 *
 * @return 0 - Success,1 - Failure
 */
static uint32_t CloseTimer(void)
{
    /* Timer handle */
    ADI_TMR_RESULT eTmrResult;

    /* Disable the timer */
    eTmrResult = adi_tmr_Enable(ghTimer, false);
    if( (eTmrResult) != ADI_TMR_SUCCESS)
    {
        printf("\nTIMER: Failed to Disable \n");
        return 1;
    }

    /* Close the timer */
    eTmrResult = adi_tmr_Close(ghTimer);
    if( (eTmrResult) != ADI_TMR_SUCCESS)
    {
        printf("\nTIMER: Failed to Close \n");
        return 1;
    }

    return 0;
}


/**
 * Initializes timer.
 *
 * @param nTimerNum - Id of the timer to be initialized.
 *
 * @return 0 - Success,1 - Failure
 */
static int32_t InitTimer(uint32_t nTimerNum)
{
    ADI_TMR_RESULT eTmrResult;

    /* Open the timer */
    if( (eTmrResult = adi_tmr_Open (nTimerNum,
                    TimerMemory,
                    ADI_TMR_MEMORY,
                    TimerHandler,
                    NULL,
                    &ghTimer)) != ADI_TMR_SUCCESS)
    {
        printf("\nFailed to open Timer \n");
        return 1;
    }

    /*
     * Use the GP timer's API's to configure and enable the timer
     *
     */

    /* Set the mode to PWM OUT */
    eTmrResult = adi_tmr_SetMode(ghTimer, ADI_TMR_MODE_CONTINUOUS_PWMOUT);
    if( (eTmrResult) != ADI_TMR_SUCCESS)
    {
        printf("\nTIMER: Failed to Set mode  \n");
        return 1;
    }
    /* Set the Period */
    eTmrResult = adi_tmr_SetPeriod(ghTimer, 4);
    if( (eTmrResult) != ADI_TMR_SUCCESS)
    {
        printf("\nTIMER: Failed to Set period \n");
        return 1;
    }
    /* Set the timer width */
    eTmrResult = adi_tmr_SetWidth(ghTimer, 2);
    if( (eTmrResult) != ADI_TMR_SUCCESS)
    {
        printf("\nTIMER: Failed to Set Width  \n");
        return 1;
    }
    /* Set the timer Delay */
    eTmrResult = adi_tmr_SetDelay(ghTimer, 0);
    if( (eTmrResult) != ADI_TMR_SUCCESS)
    {
        printf("\nTIMER: Failed to Set Delay  \n");
        return 1;
    }
    /* Enable the timer */
    eTmrResult = adi_tmr_Enable(ghTimer, true);
    if( (eTmrResult) != ADI_TMR_SUCCESS)
    {
        printf("\nTIMER: Failed to Enable \n");
        return 1;
    }

    return 0;
}

/**
 * This function resets the sensor.
 *
 * @return 0 - Success,1 - Failure
 */
static int32_t ResetSensor(void)
{

    ADI_GPIO_RESULT nResult;

    /* set GPIO output PWDN of sensor*/
    if((nResult = adi_gpio_SetDirection(SENSOR_GPIO_PORT,
                    SENSOR_GPIO_PIN,
                    ADI_GPIO_DIRECTION_OUTPUT)) != ADI_GPIO_SUCCESS)
    {
        printf("Error Setting GPIO Direction \n");
        return 1;
    }
    Delay(0x1FF);

#if defined ENA_OV7692_SENSOR
    if((nResult = adi_gpio_Set(SENSOR_GPIO_PORT,
                    SENSOR_GPIO_PIN) )!= ADI_GPIO_SUCCESS)
#elif defined (ENA_ASX340_SENSOR)
    if((nResult = adi_gpio_Clear(SENSOR_GPIO_PORT,
                    SENSOR_GPIO_PIN) )!= ADI_GPIO_SUCCESS)
#endif /* ENA_ASX340_SENSOR */
    {
        printf("Error Setting GPIO pin \n");
        return 1;
    }
    Delay(0x1FF);

#if defined ENA_OV7692_SENSOR
    if((nResult = adi_gpio_Clear(SENSOR_GPIO_PORT,
                    SENSOR_GPIO_PIN) )!= ADI_GPIO_SUCCESS)
#elif defined (ENA_ASX340_SENSOR)
    if((nResult = adi_gpio_Set(SENSOR_GPIO_PORT,
                    SENSOR_GPIO_PIN) )!= ADI_GPIO_SUCCESS)
#endif /* ENA_ASX340_SENSOR */
    {
        printf("Error Clearing GPIO pin \n");
        return 1;
    }
    Delay(0x1FF);

    return 0;

}


/**
 * This function implements delay.
 *
 * @param nCount - Counter for the delay.
 */
static void Delay(uint32_t nCount)
{
    int32_t i;
    for(i = 0;i < nCount;i++)
    {
        asm("nop;");
    }
}

/**
 * This function initializes the processor clock settings.
 *
 * @return 0 - Success,1 - Failure
 */
static int32_t PowerInit(void)
{
    int32_t nResult = 0;
    ADI_PWR_RESULT eResult;
    uint32_t nCCLK,nSysClk,nSCLK0,nSCLK1;

    /* Set the core frequency */
    if(adi_pwr_Init(CGU_DEV_NUM,PROC_CLOCK_IN)!= 0)
    {
        printf(("Failed to initialize power service \n"));
        return 1;
    }

    /* Set the maximum core and system frequency */
    eResult = adi_pwr_SetFreq(CGU_DEV_NUM,
                              PROC_MAX_CORE_CLOCK,
                              PROC_MAX_SYS_CLOCK);
    if(eResult != ADI_PWR_SUCCESS)
    {
        printf("adi_pwr_SetFreq failed \n");
        return 1;

    }

    /*
     * The following call will directly set the CGU_DIV register
     *    CSEL = 1
     *    S0SEL = 2
     *    SYSSEL = 2
     *    S1SEL = 1
     *    DSEL = 2
     *    OSEL = 4
     *    UPDT = 0
     *
     *    For a 384 MHz Core Clock this results in  system clock being set to 192 MHz
     */

    eResult =  adi_pwr_SetClkDivideRegister(
        CGU_DEV_NUM,
        (ADI_PWR_CLK_DIV)(ADI_PWR_CLK_DIV_CSEL |
        ADI_PWR_CLK_DIV_S0SEL |
        ADI_PWR_CLK_DIV_SYSSEL |
        ADI_PWR_CLK_DIV_S1SEL  |
        ADI_PWR_CLK_DIV_DSEL |
        ADI_PWR_CLK_DIV_OSEL |
        ADI_PWR_CLK_DIV_UPDT),
        0x01022241);
    if(eResult != ADI_PWR_SUCCESS)
    {
        printf("adi_pwr_SetClkDivideRegister failed \n");
        return 1;

    }

    adi_pwr_GetCoreFreq(CGU_DEV_NUM,&nCCLK);
    adi_pwr_GetSystemFreq(CGU_DEV_NUM,&nSysClk,&nSCLK0,&nSCLK1);

    printf("\n");
    printf("Core clock:     %10u Hz\n",nCCLK);
    printf("System clock:   %10u Hz\n",nSysClk);

    printf("System clock 0: %10u Hz\n",nSCLK0);
    printf("System clock 1: %10u Hz\n",nSCLK1);

    return nResult;
}


/**
 * This function closes the sensor.
 *
 * @param hSensorHandle - Sensor handle.
 *
 * @return 0 - Success,1 - Failure
 */
static int32_t CloseSensor(ADI_DEV_VIDEO_HANDLE hSensorHandle)
{
    uint32_t nResult = 0;

    nResult = adi_dev_Video_Close(hSensorHandle);
    if (nResult != ADI_DEV_CLASS_RESULT_SUCCESS)
    {
        printf("adi_dev_Video_Close failed \n"); 
        return 1;
    }

    /* Close the timer. */
    nResult = CloseTimer();
    if (nResult != 0)
    {
        printf("Error closing the timer.\n"); 
        return 1;
    }

    /* Shutdown the sensor. */
    nResult = ShutSensor();
    if (nResult != 0)
    {
        printf("Error in shutting down the sensor.\n"); 
        return 1;
    }

    return 0;
}

/**
 *
 * This function captures frame1 video
 *
 */

void printgval(double *g_x, double *g_y, double *a, double *b)
{

	int i;
	for(i=0;i<70;i++){
	            printf("%f %f ", g_x[i], g_y[i]);


	}
 printf("\n\n\n\n");
	for(i=0;i<70;i++){
	            printf("%f %f ", a[i], b[i]);


	}

}


struct MCBuffer initMCBuffer(short int len)
{
	struct MCBuffer MCBuf;
	MCBuf.Bufx = (short int*)malloc(len*sizeof(short int));
	MCBuf.Bufy = (short int*)malloc(len*sizeof(short int));
	MCBuf.len = len;
	short int P0x;
	short int P0y;
	P0x = 1000;
	P0y = 1000;
	for (int l = 0; l < len; l++)
		MCBuf.Bufx = P0x;
	    MCBuf.Bufy = P0y;
	return MCBuf;
}


void addtoMCBuffer(struct MCBuffer MCBuf, short int a, short int b)
{
	int L = MCBuf.len;
	for (int l = 0; l < L-1; l++){
		MCBuf.Bufx[l] = MCBuf.Bufx[l+1];
	MCBuf.Bufy[l] = MCBuf.Bufy[l+1];
	}
	MCBuf.Bufx[L-1] = a;
	MCBuf.Bufy[L-1] = b;
}


double * mpy(double *a, double *b, int rsize, int csize){
    int i;
    double * result;
    result = (double * ) malloc(sizeof(double));


    *(result)=0;


    for(i=0;i<rsize;i++)
    {

    		*(result)+=*(a+i)**(b+i);

    }

return result;
}

void cleanMCBuffer(struct MCBuffer MCBuf)
{
	int L = MCBuf.len;
	short int P0x;
	short int P0y;
	P0x = 1000;
	P0y = 1000;
	for (int l = 0; l < L; l++){
		MCBuf.Bufx[l] = P0x;
		MCBuf.Bufy[l] = P0y;
	}
}




int * mulmatrix(short int *a, uint8_t *b, int rsize, int csize, int c2){
    int i, j,k,  * result;
    result = (int * ) malloc( rsize* c2 * sizeof(int));

    for(i=0;i<rsize;i++)
    {
    	for(k=0;k<c2;k++){



    		*(result+(c2*i+k))=0;

    	for(j=0;j<csize;j++)

    	{
    		*(result+(c2*i+k))+=(*(a+(csize*i+j))**(b+(j*c2+k)))/8;

    	}

    }
    }

return result;
}

void printMatrix(short int *matrix, int rows, int cols){
    int i,j;

    for (i = 0; i < rows; i++){
        for ( j = 0; j < cols; j++){
            printf("%d ", *(matrix + cols * i + j));
        }
        printf("\n");
    }
}


void matmul(short int *c, short int *a, short int *b,int r,int c1, int c2)
{

int i,k, j;

for(i=0;i<r;i++)
{
	for(k=0;k<c2;k++){



		*(c+(c2*i+k))=0;

	for(j=0;j<c1;j++)

	{

		*(c+(c2*i+k))+=(*(a+(c1*i+j))**(b+(j*c2+k)));



	}

}


}

}


int max_array(int a[], int num_elements)
{
   int i, max=-32767;
   int index;
   for (i=0; i<num_elements; i++)
   {
	 if (a[i]>max)
	 {
	    max=a[i];
	    index=i;
	 }
   }
   return(index);
}


struct TrainingData getTrData(double** gx, double** gy, int* len, int N)
{
	struct TrainingData Gtr;
	struct Gesture* G = (struct Gesture *)malloc(70*sizeof(struct Gesture));
	for (int n = 0; n < N; n++)
	{
		int L = len[n];
		double *x=(double *)malloc(L*sizeof(double));
		double *y=(double *)malloc(L*sizeof(double));

		for (int l = 0; l < L; l++)
		{
			x[l] = gx[n][l];
			y[l] = gy[n][l];

		}

		G[n].x = x;
		G[n].y=y;
		G[n].len = L;
   }
	Gtr.G = G;
	Gtr.N = N;
	return Gtr;
}

void pushBack(struct node** head, int value)
{
    while (*head)
        head = &(*head)->next;

    *head = (struct node *)malloc(sizeof(**head));
    (*head)->value = value;
    (*head)->next = NULL;
}


int getStackSize(struct node *head){
    /* Input Validation */
    if (head == NULL) {
       printf("Error : Invalid stack pointer !!!\n");
       return 0;
    }

    int length = 0;
    while(head != NULL){
        head = head->next;
        length++;
    }
    return length;
}

void pop(struct node *head) {
    struct node *temp;
    while(head != NULL){
        temp = head;
        head = head->next;
        //printf("Removed  Element : %d\n", temp->value);
        free(temp);
    }
}


void cpy1(struct node* head, struct node* head1)
{
    struct node* iterator = head;
    struct node* iterator1 = head1;
    int n=0;

    while (iterator != NULL)
    {

        iterator = iterator->next;
        n=n+1;
    }

int i;
struct node* iter = head;

    for(i=0;i<n;i++){
     pairmatch_s[i]=iter->value;
     iter = iter->next;
     pairmatch_t[i]=iterator1->value;
     iterator1 = iterator1->next;
     //printf("%d %d\n", pairmatch_s[i], pairmatch_t[i]);
     //printf("%d\n", pairmatch_s[n]);
    }


 }

void swap(int *arr1, int *arr2)
{
int temp;
temp = *arr1;
*arr1 = *arr2;
*arr2 = temp;
}


double getDTW(double* s, double *s1, double* t, double *t1, int len_s, int len_t, int local_lim)
{
	double dist;
	double D[len_s+1][len_t+1];
	int i,j;
	int idx_s, idx_t;
	struct DTWInfo dtw_info;

	struct node *PairMatching_s= NULL;
	struct node* PairMatching_t=NULL;

	for (i = 0; i < len_s+1; i++)
	{
		for (j = 0; j < len_t+1; j++)
			D[i][j] = Inf;
	}
	int w = max(local_lim, abs(len_t-len_s));
	for (j = 0; j < min(len_t, w+1); j++)
		D[1][j+1]=sqrt(pow((s[0]-t[j]),2)+pow((s1[0]-t1[j]),2));
	for (i = 1; i < len_s; i++)
	{
		for (j = max(0, i-w); j < min(len_t, i+w+1); j++)
		{
			double c = sqrt(pow((s[i]-t[j]),2)+pow((s1[i]-t1[j]),2));
			D[i+1][j+1] = c + min(min(D[i][j+1],D[i+1][j]),D[i][j]);
		}
	}
	dist = D[len_s][0];
	for (j = max(1, len_t-w); j < len_t+1; j++)
	{
		if (D[len_s][j] < dist)
		{
			dist = D[len_s][j];
			idx_t = j-1;
		}
	}
idx_s = len_s - 1;

//printf("idx_t value is: %d \n",idx_t);

pushBack(&PairMatching_s,idx_s);
pushBack(&PairMatching_t,idx_t);


while (idx_s != 0 || idx_t != 0)
	{
		if (D[idx_s][idx_t] < min(D[idx_s+1][idx_t],D[idx_s][idx_t+1]))
		{
			idx_s -= 1;
			idx_t -= 1;
		}
		else if (D[idx_s+1][idx_t] < min(D[idx_s][idx_t],D[idx_s][idx_t+1]))
			idx_t -= 1;
		else
			idx_s -= 1;
		pushBack(&PairMatching_s,idx_s);
		pushBack(&PairMatching_t,idx_t);
		//printf("%d %d ", idx_s,idx_t);

	}
	//dtw_info.dist = dist;
//printf("distance is: %f\n", dist);
var=getStackSize(PairMatching_t);
var1=getStackSize(PairMatching_s);
//printf("var value %d  & %d \n", var, var1);


cpy1(PairMatching_s,PairMatching_t);
pop(PairMatching_s);
pop(PairMatching_t);

	return dist;
}


double * mulmatrix1(double *a, double *b, int rsize, int csize){
    int i, j, k;
    double * result;
    result = (double * ) malloc( rsize* csize * sizeof(double));

    for(i=0;i<rsize;i++)
    {
    	for(k=0;k<csize;k++){

    		*(result+(csize*i+k))=0;

    	for(j=0;j<csize;j++)

    	{
    		*(result+(csize*i+k))+=*(a+(csize*i+j))**(b+(j*csize+k));

    	}

    }
    }

return result;
}


void dtwScaling(double* src_x, double* src_y, double* tgt_x, double* tgt_y, double* dst_x, double* dst_y, int idx)
{
	int len_t;
	//len_t= memlen-1;
	len_t= var2[idx];
	//printf("Length value is:%d ",len_t);
	double src_px, src_py, tgt_px, tgt_py;
	double min_dist, dist;
	int best_s_idx;
	int i = 0;

	while (i < len_t)
	{
		tgt_px = tgt_x[pairmatch_t1[i]];
		tgt_py = tgt_y[pairmatch_t1[i]];
		//printf("%f %f \n",tgt_px, tgt_py);
		src_px = src_x[pairmatch_s1[i]];
		src_py = src_y[pairmatch_s1[i]];
		//printf("%f %f \n",src_px, src_py);
		min_dist = sqrt(pow(src_px - tgt_px, 2) + pow(src_py - tgt_py, 2));
		//printf("%f ", min_dist);
		best_s_idx = pairmatch_s1[i];
		if (i+1 < len_t)
		{
			while (pairmatch_t1[i+1] == pairmatch_t1[i])
			{
				src_px = src_x[pairmatch_s1[i+1]];
				dist = sqrt(pow(src_px - tgt_px, 2) + pow(src_py - tgt_py, 2));
				if (min_dist > dist)
				{
					min_dist = dist;
					best_s_idx = pairmatch_s1[i+1];
				}
				i += 1;
				if (i+1 == len_t)
					break;
			}
		}
		dst_x[pairmatch_t1[i]] = src_x[best_s_idx];
		dst_y[pairmatch_t1[i]] = src_y[best_s_idx];
		//printf("%d  %d  %f  %f  %f  %f\n", pairmatch_t1[i], best_s_idx, src_x[best_s_idx], src_y[best_s_idx],dst_x[pairmatch_t1[i]],dst_y[pairmatch_t1[i]]);

		i += 1;
	}
}


double * applyPCA(double * pca_basis, double * pca_mean, double* MCx, double* MCy, int len, int dim)
{
    int i,j;
    double v[140];

    for(i=0;i<140;i=i+1){
    	v[i]=0;
    }


    double p[3][140];
     for (i=0;i<140;i=i+1){
    	 for(j=0;j<3;j=j+1){
            p[j][i]=*(pca_basis + 140 * i + j);
           //printf("%f ", p[j][i]);
    	 }
    	 //printf("\n");
     }


	double *k, w[140];
	int rsize=140, csize=3;
	for ( i = 0; i < len; i=i+1)
	{
		v[i]     = MCx[i];
		v[len+i] = MCy[i];

	}


	for (i=0;i<140;i=i+1){
		w[i]=v[i]-pca_mean[i];
	}

	double * it;
	it = (double * ) malloc( 3 * sizeof(double));
	for(i=0;i<3;i++){
		it[i]=0;
		for(j=0;j<140;j++){
          *(it+i)+=p[i][j]*w[j];
		}
		//printf("%f ", it[i]);
	}

	return it;

}


void clearTrData(struct TrainingData Gtr)
{   int n;
	for (n = 0; n < Gtr.N; n++){
		free(Gtr.G[n].x);
		free(Gtr.G[n].y);
	}

	free(Gtr.G);

}

void scp(int *a, int *b){
int i;

for(i=0;i<var;i++){

	*(b+i)=*(a+i);
}


}

char recogGesture1(struct mcent m1, double** g_x, double** g_y, double * pca_basis, double * pca_mean, struct GestureClassifiers *GesC)
{
	char g;
	int local_lim = 0;
	int len = 70;
	double v1[3][1];
	double *v;
	double dist, temp;
	int idx;
	int mcs=memlen;
	int pca_dim = 3;
	double dtw_thresh = 670;
	double score;
	double best_score = -Inf;
	//double match_thresh = -11;
    double match_thresh = -35;
	dist = Inf;

    	int n;
	for (n = 0; n < 5; n++)
	{
		temp = getDTW(g_x[n], g_y[n], m1.x, m1.y, 70, mcs, local_lim);
		printf("distance is %f \n",temp);
      		var2[n]=var;
		if (dist > temp)
		{
			dist = temp;
            	scp(pairmatch_s,pairmatch_s1);
           		scp(pairmatch_t,pairmatch_t1);
			idx = n;
		}

	}
	printf("index is %d \n",idx);
	printf("dist & temp:%f & %f\n", dist, temp);
	int i;
	for(i=0;i<5;i++){
	printf("%d \n",var2[i]);
	}


	if (dist > dtw_thresh)
			return 'P';



	for(i=0;i<var2[idx];i++){
	swap(&pairmatch_s1[i],&pairmatch_t1[i]);
	}
/*
        for(i=0;i<var2[idx];i++){
	printf("%d %d \n",pairmatch_s1[i],pairmatch_t1[i]);
	}
*/
	double *dst_x=(double *)malloc(2*var*sizeof(double));
    double *dst_y=(double *)malloc(2*var*sizeof(double));
	dtwScaling(m1.x, m1.y, g_x[idx], g_y[idx], dst_x, dst_y, idx);
	/*
	for(i=0;i<70;i++){
	//dst_x[i]=0;
	//dst_y[i]=0;
	printf("%f %f \n",dst_x[i], dst_y[i]);
	}
	*/
	v = applyPCA(pca_basis, pca_mean, dst_x, dst_y , len, pca_dim);
    	free(dst_x);
	free(dst_y);

	printf("checkpoint 2\n");


	double	y[1][3];
        for(i=0;i<3;i++){
        	v1[i][0]=*(v+i);
	      y[0][i]=v1[i][0];
	      printf("values are: %f\n", y[0][i]);
        }
	  free(v);
      int rsize=3,csize=1;
      double *k, *temp1, *temp2, temp3;
      int j;


      for (n = 0; n < 5; n++)
  	{
    	  k = (double * ) malloc( 3 * sizeof(double));
    	  	for(i=0;i<3;i++){
    	  		k[i]=0;
    	  		for(j=0;j<3;j++){
    	            *(k+i)+=y[0][j]*GesC[n].weights[i][j];
    	  		}
    	  		//printf("%f ", k[i]);
    	  	}

  	temp1=mpy(k,v1, 3,3);
  	temp2=mpy(GesC[n].lin,v1, 3,3);
  	temp3=*temp1+*temp2+GesC[0].Biases[n];
  	score = temp3;
    printf("Score is: %f\n", score);
    free(k);
    free(temp1);
    free(temp2);
		if (score > best_score)
		{
			best_score = score;

			g = GesC[0].Labels[n];
		}
	}

        printf("Best score is: %f\n", best_score);
        printf("character is %c\n", g);
	if (best_score < match_thresh)
		return  'P';
	else
		return g;



}












