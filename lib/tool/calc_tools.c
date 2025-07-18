unsigned int8 calc_crc8(unsigned int8 frame[], int8 payload_size)
{
   unsigned int8 crc = frame[0], i = 1;
   while(i < payload_size)
      crc ^= frame[i++];
   return crc;
}


void int32_to_be_array(int32 value, int8 buf[4])
{
    buf[0] = (int8)(value >> 24);   // MSB
    buf[1] = (int8)(value >> 16);
    buf[2] = (int8)(value >>  8);
    buf[3] = (int8)(value      );   // LSB
}


int32 be_array_to_int32(int8 buf[4])
{
    return ((int32)buf[0] << 24) |
           ((int32)buf[1] << 16) |
           ((int32)buf[2] <<  8) |
           ((int32)buf[3]);
}

unsigned int32 int32_lsb_to_msb(unsigned int32 value)
{
    return ((value & 0x000000FF) << 24) |
           ((value & 0x0000FF00) << 8)  |
           ((value & 0x00FF0000) >> 8)  |
           ((value & 0xFF000000) >> 24);
}