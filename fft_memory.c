/* ========================================================================== */
/*                                                                            */
/*   FFT_memory.c                                                             */
/*   (c) 2014 Ondrej Nentvich                                                 */
/*                                                                            */
/*   Computing FFT with data stored in external SPI memory                    */
/*                                                                            */
/* ========================================================================== */

#include "fft_memory.h"
#include "spi_memory_23lcxx.h"
#include "config.h"
#include <math.h>
#include <avr/pgmspace.h>

const float sincos[] PROGMEM=			// 512 points
{
	//sin(w)	cos(w)
	0.000000,	1.000000,
	0.012272,	0.999925,
	0.024541,	0.999699,
	0.036807,	0.999322,
	0.049068,	0.998795,
	0.061321,	0.998118,
	0.073565,	0.997290,
	0.085797,	0.996313,
	0.098017,	0.995185,
	0.110222,	0.993907,
	0.122411,	0.992480,
	0.134581,	0.990903,
	0.146730,	0.989177,
	0.158858,	0.987301,
	0.170962,	0.985278,
	0.183040,	0.983105,
	0.195090,	0.980785,
	0.207111,	0.978317,
	0.219101,	0.975702,
	0.231058,	0.972940,
	0.242980,	0.970031,
	0.254866,	0.966976,
	0.266713,	0.963776,
	0.278520,	0.960431,
	0.290285,	0.956940,
	0.302006,	0.953306,
	0.313682,	0.949528,
	0.325310,	0.945607,
	0.336890,	0.941544,
	0.348419,	0.937339,
	0.359895,	0.932993,
	0.371317,	0.928506,
	0.382683,	0.923880,
	0.393992,	0.919114,
	0.405241,	0.914210,
	0.416430,	0.909168,
	0.427555,	0.903989,
	0.438616,	0.898674,
	0.449611,	0.893224,
	0.460539,	0.887640,
	0.471397,	0.881921,
	0.482184,	0.876070,
	0.492898,	0.870087,
	0.503538,	0.863973,
	0.514103,	0.857729,
	0.524590,	0.851355,
	0.534998,	0.844854,
	0.545325,	0.838225,
	0.555570,	0.831470,
	0.565732,	0.824589,
	0.575808,	0.817585,
	0.585798,	0.810457,
	0.595699,	0.803208,
	0.605511,	0.795837,
	0.615232,	0.788346,
	0.624859,	0.780737,
	0.634393,	0.773010,
	0.643832,	0.765167,
	0.653173,	0.757209,
	0.662416,	0.749136,
	0.671559,	0.740951,
	0.680601,	0.732654,
	0.689541,	0.724247,
	0.698376,	0.715731,
	0.707107,	0.707107,
	0.715731,	0.698376,
	0.724247,	0.689541,
	0.732654,	0.680601,
	0.740951,	0.671559,
	0.749136,	0.662416,
	0.757209,	0.653173,
	0.765167,	0.643832,
	0.773010,	0.634393,
	0.780737,	0.624859,
	0.788346,	0.615232,
	0.795837,	0.605511,
	0.803208,	0.595699,
	0.810457,	0.585798,
	0.817585,	0.575808,
	0.824589,	0.565732,
	0.831470,	0.555570,
	0.838225,	0.545325,
	0.844854,	0.534998,
	0.851355,	0.524590,
	0.857729,	0.514103,
	0.863973,	0.503538,
	0.870087,	0.492898,
	0.876070,	0.482184,
	0.881921,	0.471397,
	0.887640,	0.460539,
	0.893224,	0.449611,
	0.898674,	0.438616,
	0.903989,	0.427555,
	0.909168,	0.416430,
	0.914210,	0.405241,
	0.919114,	0.393992,
	0.923880,	0.382683,
	0.928506,	0.371317,
	0.932993,	0.359895,
	0.937339,	0.348419,
	0.941544,	0.336890,
	0.945607,	0.325310,
	0.949528,	0.313682,
	0.953306,	0.302006,
	0.956940,	0.290285,
	0.960431,	0.278520,
	0.963776,	0.266713,
	0.966976,	0.254866,
	0.970031,	0.242980,
	0.972940,	0.231058,
	0.975702,	0.219101,
	0.978317,	0.207111,
	0.980785,	0.195090,
	0.983105,	0.183040,
	0.985278,	0.170962,
	0.987301,	0.158858,
	0.989177,	0.146730,
	0.990903,	0.134581,
	0.992480,	0.122411,
	0.993907,	0.110222,
	0.995185,	0.098017,
	0.996313,	0.085797,
	0.997290,	0.073565,
	0.998118,	0.061321,
	0.998795,	0.049068,
	0.999322,	0.036807,
	0.999699,	0.024541,
	0.999925,	0.012272,
	1.000000,	0.000000,
	0.999925,	-0.012272,
	0.999699,	-0.024541,
	0.999322,	-0.036807,
	0.998795,	-0.049068,
	0.998118,	-0.061321,
	0.997290,	-0.073565,
	0.996313,	-0.085797,
	0.995185,	-0.098017,
	0.993907,	-0.110222,
	0.992480,	-0.122411,
	0.990903,	-0.134581,
	0.989177,	-0.146730,
	0.987301,	-0.158858,
	0.985278,	-0.170962,
	0.983105,	-0.183040,
	0.980785,	-0.195090,
	0.978317,	-0.207111,
	0.975702,	-0.219101,
	0.972940,	-0.231058,
	0.970031,	-0.242980,
	0.966976,	-0.254866,
	0.963776,	-0.266713,
	0.960431,	-0.278520,
	0.956940,	-0.290285,
	0.953306,	-0.302006,
	0.949528,	-0.313682,
	0.945607,	-0.325310,
	0.941544,	-0.336890,
	0.937339,	-0.348419,
	0.932993,	-0.359895,
	0.928506,	-0.371317,
	0.923880,	-0.382683,
	0.919114,	-0.393992,
	0.914210,	-0.405241,
	0.909168,	-0.416430,
	0.903989,	-0.427555,
	0.898674,	-0.438616,
	0.893224,	-0.449611,
	0.887640,	-0.460539,
	0.881921,	-0.471397,
	0.876070,	-0.482184,
	0.870087,	-0.492898,
	0.863973,	-0.503538,
	0.857729,	-0.514103,
	0.851355,	-0.524590,
	0.844854,	-0.534998,
	0.838225,	-0.545325,
	0.831470,	-0.555570,
	0.824589,	-0.565732,
	0.817585,	-0.575808,
	0.810457,	-0.585798,
	0.803208,	-0.595699,
	0.795837,	-0.605511,
	0.788346,	-0.615232,
	0.780737,	-0.624859,
	0.773010,	-0.634393,
	0.765167,	-0.643832,
	0.757209,	-0.653173,
	0.749136,	-0.662416,
	0.740951,	-0.671559,
	0.732654,	-0.680601,
	0.724247,	-0.689541,
	0.715731,	-0.698376,
	0.707107,	-0.707107,
	0.698376,	-0.715731,
	0.689541,	-0.724247,
	0.680601,	-0.732654,
	0.671559,	-0.740951,
	0.662416,	-0.749136,
	0.653173,	-0.757209,
	0.643832,	-0.765167,
	0.634393,	-0.773010,
	0.624859,	-0.780737,
	0.615232,	-0.788346,
	0.605511,	-0.795837,
	0.595699,	-0.803208,
	0.585798,	-0.810457,
	0.575808,	-0.817585,
	0.565732,	-0.824589,
	0.555570,	-0.831470,
	0.545325,	-0.838225,
	0.534998,	-0.844854,
	0.524590,	-0.851355,
	0.514103,	-0.857729,
	0.503538,	-0.863973,
	0.492898,	-0.870087,
	0.482184,	-0.876070,
	0.471397,	-0.881921,
	0.460539,	-0.887640,
	0.449611,	-0.893224,
	0.438616,	-0.898674,
	0.427555,	-0.903989,
	0.416430,	-0.909168,
	0.405241,	-0.914210,
	0.393992,	-0.919114,
	0.382683,	-0.923880,
	0.371317,	-0.928506,
	0.359895,	-0.932993,
	0.348419,	-0.937339,
	0.336890,	-0.941544,
	0.325310,	-0.945607,
	0.313682,	-0.949528,
	0.302006,	-0.953306,
	0.290285,	-0.956940,
	0.278520,	-0.960431,
	0.266713,	-0.963776,
	0.254866,	-0.966976,
	0.242980,	-0.970031,
	0.231058,	-0.972940,
	0.219101,	-0.975702,
	0.207111,	-0.978317,
	0.195090,	-0.980785,
	0.183040,	-0.983105,
	0.170962,	-0.985278,
	0.158858,	-0.987301,
	0.146730,	-0.989177,
	0.134581,	-0.990903,
	0.122411,	-0.992480,
	0.110222,	-0.993907,
	0.098017,	-0.995185,
	0.085797,	-0.996313,
	0.073565,	-0.997290,
	0.061321,	-0.998118,
	0.049068,	-0.998795,
	0.036807,	-0.999322,
	0.024541,	-0.999699,
	0.012272,	-0.999925,
	0.000000,	-1.000000

};

const unsigned int bitrev[] PROGMEM=		// 512 points bit reversing
{
	0,256,128,384,64,320,192,448,
	32,288,160,416,96,352,224,480,
	16,272,144,400,80,336,208,464,
	48,304,176,432,112,368,240,496,
	8,264,136,392,72,328,200,456,
	40,296,168,424,104,360,232,488,
	24,280,152,408,88,344,216,472,
	56,312,184,440,120,376,248,504,
	4,260,132,388,68,324,196,452,
	36,292,164,420,100,356,228,484,
	20,276,148,404,84,340,212,468,
	52,308,180,436,116,372,244,500,
	12,268,140,396,76,332,204,460,
	44,300,172,428,108,364,236,492,
	28,284,156,412,92,348,220,476,
	60,316,188,444,124,380,252,508,
	2,258,130,386,66,322,194,450,
	34,290,162,418,98,354,226,482,
	18,274,146,402,82,338,210,466,
	50,306,178,434,114,370,242,498,
	10,266,138,394,74,330,202,458,
	42,298,170,426,106,362,234,490,
	26,282,154,410,90,346,218,474,
	58,314,186,442,122,378,250,506,
	6,262,134,390,70,326,198,454,
	38,294,166,422,102,358,230,486,
	22,278,150,406,86,342,214,470,
	54,310,182,438,118,374,246,502,
	14,270,142,398,78,334,206,462,
	46,302,174,430,110,366,238,494,
	30,286,158,414,94,350,222,478,
	62,318,190,446,126,382,254,510,
	1,257,129,385,65,321,193,449,
	33,289,161,417,97,353,225,481,
	17,273,145,401,81,337,209,465,
	49,305,177,433,113,369,241,497,
	9,265,137,393,73,329,201,457,
	41,297,169,425,105,361,233,489,
	25,281,153,409,89,345,217,473,
	57,313,185,441,121,377,249,505,
	5,261,133,389,69,325,197,453,
	37,293,165,421,101,357,229,485,
	21,277,149,405,85,341,213,469,
	53,309,181,437,117,373,245,501,
	13,269,141,397,77,333,205,461,
	45,301,173,429,109,365,237,493,
	29,285,157,413,93,349,221,477,
	61,317,189,445,125,381,253,509,
	3,259,131,387,67,323,195,451,
	35,291,163,419,99,355,227,483,
	19,275,147,403,83,339,211,467,
	51,307,179,435,115,371,243,499,
	11,267,139,395,75,331,203,459,
	43,299,171,427,107,363,235,491,
	27,283,155,411,91,347,219,475,
	59,315,187,443,123,379,251,507,
	7,263,135,391,71,327,199,455,
	39,295,167,423,103,359,231,487,
	23,279,151,407,87,343,215,471,
	55,311,183,439,119,375,247,503,
	15,271,143,399,79,335,207,463,
	47,303,175,431,111,367,239,495,
	31,287,159,415,95,351,223,479,
	63,319,191,447,127,383,255,511
};

const char DECIMATION_FACTOR =		2;//SAMPLING_RATE/DECIMATE_TO_FREQUENCY

//**************************************************************************
// Storing and bitreversing for FFT
//**************************************************************************
	
void decimate_and_store(void)
{
	unsigned long decimation_position;
	unsigned int i;
	complex point;
	int signal_offset2;
	int read_data;
	int position;
	
	signal_offset2 = spi_mem_read_word(MEM_SIGNAL_OFFSET);

	point.imag = 0;
	
	for (i=0;i<NO_POINTS;++i)		// storing and bitreversing raw signal into spi memory
	{
		position = pgm_read_dword_far(&bitrev[i]);
		decimation_position = (long) position*DECIMATION_FACTOR;
		if (decimation_position<MEM_SIGNAL_LENGTH)
		{
			read_data = spi_mem_read_word(decimation_position*2 + MEM_SIGNAL_BEGIN) - signal_offset2;
			point.real = read_data;
		}
		else
			point.real = 0;
					
		
		spi_mem_write_complex((i*8)+MEM_FFT_BEGIN,point);	

	}
}

//**************************************************************************
// FFT algorithm
//**************************************************************************
	
void FFT_calculate(void)
{
	complex A, B, C;            // A, B - input points, C - calculated point
	
    float dsin,dcos;
    unsigned int s,k;
    unsigned int n=1;
    unsigned int angf=NO_POINTS/2;
	unsigned int Fnk;
	
	unsigned long pointer_A,pointer_B;
	Fnk=0;
    
    while(n<NO_POINTS)		// number of iterations log2(NO_POINTS)
    {
		pointer_A = MEM_FFT_BEGIN;
		pointer_B = MEM_FFT_BEGIN + n*8;
	    Fnk = 0;
	    
	    for(k=0;k<n;k++)	// distance between butterflies
	    {
		    dsin = pgm_read_float(&sincos[Fnk++]);
		    dcos = pgm_read_float(&sincos[Fnk++]);
		    
		    for(s=0;s<angf;s++)		// number of groups
		    {
				A = spi_mem_read_complex(pointer_A);
				B = spi_mem_read_complex(pointer_B);
				
				C.real=(B.real*dcos - B.imag*dsin);
				C.imag=(B.real*dsin + B.imag*dcos);
				
				B.real=A.real - C.real;
				B.imag=A.imag - C.imag;
				A.real=A.real + C.real;
				A.imag=A.imag + C.imag;
				
				spi_mem_write_complex(pointer_A,A);
				spi_mem_write_complex(pointer_B,B);
				
				pointer_A += 16*n;
				pointer_B += 16*n;
		    }
		    pointer_A-=((NO_POINTS-1)*8);
		    pointer_B-=((NO_POINTS-1)*8);
		    Fnk+=(angf-1)*2;
	    }
	    n<<=1;
	    angf>>=1;
    }
}


