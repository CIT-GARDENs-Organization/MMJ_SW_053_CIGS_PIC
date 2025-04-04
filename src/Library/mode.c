#include "mode.h"

//********* global variables *********/
void update_time(unsigned int8 raw_time[])
{  
   unsigned int32 time;
   memcpy(&time, raw_time, 4);
   
   day = (time >> 22) & 0x000003FF;
   sec = time & 0x0001FFFF;
   //unsigned int8  subsec = (time >> 18) & 0x0000000F;
   // int1 absolute_flag = time >> 17 & 0x00000001
}