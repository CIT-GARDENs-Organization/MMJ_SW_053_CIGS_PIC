Command make_command(unsigned int8 receive_signal[], int8 receive_signal_size)
{
   volatile Command command = make_receive_command(receive_signal, receive_signal_size);
   //fprintf(PC, "--Communication drier--\r\n");
   //fprintf(PC, "command.size: %d\r\n", command.size);
   //fprintf(PC, "command.frame_id: %X\r\n", command.frame_id);
   //fprintf(PC, "command.content: ");
   for (int8 i = 0; i < command.size; i++)
      //fprintf(PC, "%X ", command.content[i]);
   //fprintf(PC, "\r\n");
   //fprintf(PC, "command.is_exist: %X\r\n", command.is_exist);
   return command;
}

void transmit_ack()
{
   transmit_command(ACK, 0, 0);
}

void transmit_status()
{
   transmit_command(MIS_MCU_STATUS, &status, 1);
}

// EOF
