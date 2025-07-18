Command make_command(unsigned int8 receive_signal[], int8 receive_signal_size)
{
   Command command = make_receive_command(receive_signal, receive_signal_size);
   fprintf(PC, "\t-> Frame ID: %X\r\n", command.frame_id);
   fprintf(PC, "\t-> Content size: %d\r\n", command.size);
   return command;
}

void transmit_ack()
{
   transmit_command(ACK, 0, 0);
}

void transmit_status()
{
   transmit_command(MIS_MCU_STATUS, &status, 4);
}

