#include "smf_queue.h"
#include "mmj_smf_memorymap.h"


void enqueue_flash_operation(FlashOperationStruct *data)
{   
   int8 next_tail = (flash_queue.tail_index + 1) % SMF_QUEUE_SIZE;

   if(next_tail == flash_queue.head_index)
      printf("Flash queue is full!!!\r\n");
      
   else
   {
      flash_queue.entries[flash_queue.tail_index].mission_id = data->mission_id;
      flash_queue.entries[flash_queue.tail_index].func_type  = data->func_type;
      flash_queue.entries[flash_queue.tail_index].write_mode = data->write_mode;
      flash_queue.entries[flash_queue.tail_index].source_type = data->source_type;
      flash_queue.entries[flash_queue.tail_index].misf_start_addr = data->misf_start_addr;
      flash_queue.entries[flash_queue.tail_index].misf_size = data->misf_size;
      flash_queue.tail_index = next_tail;
   }
}


FlashOperationStruct *dequeue_flash_operation()
{
   if (flash_queue.head_index == flash_queue.tail_index)
   {
      printf("Flash queue is empty\r\n");
      return 0x00;
   }
   else
   {
      int8 current_head = flash_queue.head_index;
      flash_queue.head_index = (flash_queue.head_index + 1) % SMF_QUEUE_SIZE;
      return &flash_queue.entries[current_head];
   }
}

int1 is_empty_flash_queue(void)
{
   return flash_queue.head_index == flash_queue.tail_index;
}


SmfAddressStruct get_smf_address_struct(MissionID mission_id)
{
   SmfAddressStruct mis_struct = {0};

   if (mission_id == CIGS_DATA_TABLE)
   {
      mis_struct.start_address = CIGS_DATA_TABLE_START_ADDRESS;
      mis_struct.end_address   = CIGS_DATA_TABLE_END_ADDRESS;
   }
   else if (mission_id == CIGS_PICLOG_DATA)
   {
      mis_struct.start_address = CIGS_PICLOG_START_ADDRESS;
      mis_struct.end_address   = CIGS_PICLOG_END_ADDRESS;
   }
   else if (mission_id == CIGS_ENVIRO_DATA)
   {
      mis_struct.start_address = CIGS_PICLOG_START_ADDRESS;
      mis_struct.end_address   = CIGS_PICLOG_END_ADDRESS;
   }
   else if (mission_id == CIGS_IV_HEADER)
   {
      mis_struct.start_address = CIGS_PICLOG_END_ADDRESS;
      mis_struct.end_address   = CIGS_PICLOG_END_ADDRESS;
   }
   else if (mission_id == CIGS_IV_DATA)
   {
      mis_struct.start_address = CIGS_PICLOG_END_ADDRESS;
      mis_struct.end_address   = CIGS_PICLOG_END_ADDRESS;
   }
   return mis_struct;
}

