#INT_RDA
static void RDA_isr(void)
{
   if (!(status == EXECUTING_MISSION || status == COPYING) || is_use_smf_req_in_mission)
      if (boss_receive_buffer_size < RECEIVE_BUFFER_MAX)
         boss_receive_buffer[boss_receive_buffer_size++ % RECEIVE_BUFFER_MAX] = fgetc(BOSS);
}

void initialize_uart()
{
   enable_interrupts(INT_RDA);
   enable_interrupts(global);
   fprintf(PC, "\tUART initialize complete\r\n");
}

void clear_receive_signal(unsigned int8 receive_signal[], int8 *receive_signal_size)
{
   memset(receive_signal, 0x00, *receive_signal_size);
   *receive_signal_size = 0;
}

// EOF
