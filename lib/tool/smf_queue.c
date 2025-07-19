
void enqueue_smf_data(SmfDataStruct *data)
{   
   int8 next_tail = (smf_queue.smf_data_tail + 1) % SMF_DATA_SIZE;

   if(next_tail == smf_queue.smf_data_head)
      fprintf(PC, "SMF data list is full!!!\r\n");
      
   else
   {
      smf_queue.smf_data[smf_queue.smf_data_tail].mission_type = data->mission_type;
      smf_queue.smf_data[smf_queue.smf_data_tail].src         = data->src;
      smf_queue.smf_data[smf_queue.smf_data_tail].size        = data->size;

      smf_queue.smf_data_tail = next_tail;
   }
}


SmfDataStruct *dequeue_smf_data()
{
   if (smf_queue.smf_data_head == smf_queue.smf_data_tail)
   {
      fprintf(PC, "SMF data list is empty\r\n");
      return 0x00;
   }
   else
   {
      int8 current_head = smf_queue.smf_data_head;
      smf_queue.smf_data_head = (smf_queue.smf_data_head + 1) % SMF_DATA_SIZE;
      return &smf_queue.smf_data[current_head];
   }
}

int1 is_empty_smf_data(void)
{
   return smf_queue.smf_data_head == smf_queue.smf_data_tail;
}


MissionTypeStruct getMissionTypeStruct(MissionType mis_type)
{
   MissionTypeStruct mis_struct = {0};
   
   if (mis_type == MEAURE_DATA)
   {
      mis_struct.start_address   = 0x04EC0000;
      mis_struct.end_address     = 0x056BFFFF;
      mis_struct.mission_flag    = 0b10000000;
   }
   else if (mis_type == PICLOG_DATA)
   {
      mis_struct.start_address   = 0x04DC0000;
      mis_struct.end_address     = 0x04EBFFFF;
      mis_struct.mission_flag    = 0b10000001;  
   }
   else
   {
      // undefined mission type. output `ff` as default
      mis_struct.start_address   = 0xFFFFFFFF;
      mis_struct.end_address     = 0xFFFFFFFF;
      mis_struct.mission_flag    = 0b11111111; 
   }
   
   return mis_struct;
}

