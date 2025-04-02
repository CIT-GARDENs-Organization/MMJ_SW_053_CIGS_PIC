#include "DataCopy.h"

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

int1 is_empty_smf_data(void)
{
   return smf_data_head == smf_data_tail;
}
