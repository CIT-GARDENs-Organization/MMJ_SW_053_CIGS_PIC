#include "mis_muc_flash.h"
#include "GDNS_tbd_FlashOperation.h"

//********PICF FUCNTION ***************/
void write_data_picf(PICF_START_ADDR, PICF_END_ADDR, (int8 *)mission_datas)
{
   write_data_bytes_flash128m(PICF_START_ADDR, PICF_END_ADDR, (int8 *)mission_datas); 
}

void read_data_picf()
{
   read_data_bytes_flash128m(PICF_START_ADDR, PICF_END_ADDR, (int8 *)mission_datas); 
   for (int i = 0; i < PICF_END_ADDR; i++)
   {
      fprintf(PC, "mission_datas[%d]: %X\r\n", i, mission_datas[i]);
   }
}

void erase_picf()
{
   sector_erase_flash128m(PICF_START_ADDR); // erase 64kByte
   delay_ms(1000);
   fprintf(PC, "Erase PICF\r\n");
}





//********* SMF data queue *********/
void copy_data(void)
{
   fprintf(PC, "\r\n______________________________\r\n________Start copy_data________\r\n\r\n");
   while (!is_empty_smf_data())
   {
      SmfDataStruct smf_data = dequeue_smf_data();
      fprintf(PC, "Source address: \t0x%08LX\r\n", smf_data.src);
      fprintf(PC, "Destination address: \t0x%08LX\r\n", smf_data.dest);
      fprintf(PC, "Data size: \t\t  %08LX\r\n", smf_data.size);
      fprintf(PC, "Copying.");
      delay_ms(1000);
      fprintf(PC, ".");
      delay_ms(1000);
      fprintf(PC, ".");
      delay_ms(1000);
      fprintf(PC, ".");   
      delay_ms(1000);
      fprintf(PC, ".\r\n");   
      delay_ms(1000);
   }
   fprintf(PC, "_________End copy_data_________\r\n______________________________\r\n\r\n");
}

/**
 * @brief enqueue_smf_data
 * @param src: source address
 * @param dest: destination address
 * @param size: data size
 */
SmfDataStruct *dequeue_smf_data(void)
{
   if (smf_data_head == smf_data_tail)
   {
      fprintf(PC, "SMF data list is empty\r\n");
      return 0x00;
   }
   else
   {
      int8 current_head = smf_data_head;
      smf_data_head = (smf_data_head + 1) % SMF_DATA_SIZE;
      return &smf_data[current_head];
   }
}

/**
 * @brief is_empty_smf_data
 * @return 1: empty, 0: not empty
 */
int1 is_empty_smf_data(void)
{
   return smf_data_head == smf_data_tail;
}
