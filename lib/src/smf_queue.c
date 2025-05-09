void enqueue_smf_data(SmfDataStruct *data)
{   
   int8 next_tail = (smf_queue.smf_data_tail + 1) % SMF_DATA_SIZE;

   if(next_tail == smf_queue.smf_data_head)
      fprintf(PC, "SMF data list is full!!!\r\n");
      
   else
   {
      smf_queue.smf_data[smf_queue.smf_data_tail].dest = data->dest;
      smf_queue.smf_data[smf_queue.smf_data_tail].src = data->src;
      smf_queue.smf_data[smf_queue.smf_data_tail].size = data->size;

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
