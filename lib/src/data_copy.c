#include "smf_queue.h"
#include "flash.h"


void copy_data()
{   
   while (!is_empty_smf_data())
   {
      SmfDataStruct *smf_data = dequeue_smf_data();
      fprintf(PC, "Destination address: \t0x%08LX\r\n", smf_data->dest);
      fprintf(PC, "Source address: \t0x%08LX\r\n", smf_data->src);
      fprintf(PC, "Data size: \t\t  %08LX\r\n", smf_data->size);
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
