#ifndef DATA_COPY_H
#define DATA_COPY_H

#include "smf_queue.h"
//#include "flash.h"

#Separate
void copy_data(void);
#Separate
void initialize_mission(smfDataStruct* smf_data, MissionTypeStruct* mission_type,
                        unsigned int32* mis_start_address, unsigned int32* mis_end_address,
                        unsigned int32* write_src, unsigned int32* write_size,
                        unsigned int8* loop_count, unsigned int32* used_size);
#Separate
void check_and_erase_if_wrap(unsigned int32 data_region_start, unsigned int32 data_region_end);
#Separate
void erase_write_area(unsigned int32 data_write_addr, unsigned int32 write_size, unsigned int32 mis_end_address);
#Separate
void write_mission_data(unsigned int32 write_src, unsigned int32 write_size, unsigned int32 data_write_addr, int8* buffer, unsigned int32* used_size);
#Separate
void update_size_area(unsigned int32 mis_start_address, unsigned int32 used_size, unsigned int8 loop_count);

#include "./src/data_copy.c"

#endif


