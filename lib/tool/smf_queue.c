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
      flash_queue.entries[flash_queue.tail_index].src        = data->src;
      flash_queue.entries[flash_queue.tail_index].size       = data->size;
      flash_queue.entries[flash_queue.tail_index].write_mode = data->write_mode;
      flash_queue.entries[flash_queue.tail_index].source_type = data->source_type;
      flash_queue.entries[flash_queue.tail_index].misf_start_addr = data->misf_start_addr;
      flash_queue.entries[flash_queue.tail_index].misf_size = data->misf_size;
      flash_queue.entries[flash_queue.tail_index].manager = data->manager;

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


SmfMissionStruct get_smf_mission_struct(FunctionType func_type)
{
   SmfMissionStruct mis_struct = {0};
   
   if (func_type == SMF_WRITE)
   {
      mis_struct.start_address = CIGS_DATA_TABLE_START_ADDRESS;
      mis_struct.end_address   = CIGS_DATA_TABLE_END_ADDRESS;
   }
   else if (func_type == SMF_READ)
   {
      mis_struct.start_address = CIGS_MEASURE_DATA_START_ADDRESS;
      mis_struct.end_address   = CIGS_MEASURE_DATA_END_ADDRESS;
   }
   else if (func_type == SMF_ERASE)
   {
      mis_struct.start_address = CIGS_PICLOG_START_ADDRESS;
      mis_struct.end_address   = CIGS_PICLOG_END_ADDRESS;
   }
   
   return mis_struct;
}

