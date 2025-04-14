#include "function.h"

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

unsigned int8 make_checksum()
{
   unsigned int8 checksum = 0;
   for (int i = 0; i < PACKET_SIZE; i++)
      checksum += PACKET_DATA[i];
   PACKET_DATA[PACKET_SIZE - 1] = checksum;
   return checksum;
}