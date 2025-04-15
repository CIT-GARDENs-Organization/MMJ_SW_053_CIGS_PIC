#ifndef COMMUNICATION_H
#define COMMUNICATION_H

// fundamental function. It's format resieve signal to Command struct
Command make_receive_command(unsigned int8 receive_signal[], int8 receive_signal_size);
int8 make_frame(unsigned int8 receive_signal[], int8 receive_signal_size);
int8 get_content_size(unsigned int8 frame_id);
int1 check_crc(unsigned int8 frame[], int8 receive_frame_size);
unsigned int8 calc_crc(unsigned int8 frame[], int8 payload_size);
int1 check_device_id(unsigned int8 device_id);

// clear receive buffer
void clear_receive_signal(unsigned int8 receive_signal[], int8 *receive_signal_size);

// convet Command to byte and transmit this data
void transmit_command(int8 frame_id, unsigned int8 content[], int8 size);
void transmit(unsigned int8 data[], int8 data_size);

#endif // COMMUNICATION_H
