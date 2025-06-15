//#include "../../cigs/mmj_cigs_config.h"
#include "../mt25q.h"

#include <stdint.h>

#define MAX_READ_SIZE 64
#define SIZE_AREA_SIZE 0x1000
#define SUBSECTOR_SIZE 0x1000 // (4096bytes)

#Separate
#separate
void copy_data()
{
    fprintf(PC, "\r\n____________________\r\n");
    fprintf(PC, "___Start copy_data__\r\n");

    int8 buffer[MAX_READ_SIZE];

    for (int8 i = 0; !is_empty_smf_data(); i++)
    {
        fprintf(PC, "Start [%d th] data copy\r\n\r\n", i);

        smfDataStruct *smf_data = dequeue_smf_data();
        MissionTypeStruct mission_type;
        unsigned int32 mis_start_address, mis_end_address, write_src, write_size;
        uint8_t loop_count;
        unsigned int32 used_size;

        initialize_mission(smf_data, &mission_type, &mis_start_address, &mis_end_address, &write_src, &write_size, &loop_count, &used_size);
        status[i+1] = mission_type.mission_flag;

        unsigned int32 data_region_start = mis_start_address + SUBSECTOR_SIZE;
        unsigned int32 data_region_end = mis_end_address;
        unsigned int32 data_region_size = data_region_end - data_region_start;
        unsigned int32 data_write_addr = data_region_start + used_size;

        if ((used_size + write_size) > data_region_size)
        {
            check_and_erase_if_wrap(data_region_start, data_region_end);
            loop_count++;
            used_size = 0;
            data_write_addr = data_region_start;
        }

        erase_write_area(data_write_addr, write_size, mis_end_address);

        write_mission_data(write_src, write_size, data_write_addr, buffer, &used_size);

        update_size_area(mis_start_address, used_size, loop_count);
    }

    fprintf(PC, "\r\n___End copy_data____\r\n");
    fprintf(PC, "____________________\r\n\r\n");
}

#separate
void initialize_mission(smfDataStruct* smf_data, MissionTypeStruct* mission_type,
                        unsigned int32* mis_start_address, unsigned int32* mis_end_address,
                        unsigned int32* write_src, unsigned int32* write_size,
                        unsigned int8* loop_count, unsigned int32* used_size)
{
    *mission_type = getMissionTypeStruct(smf_data->mission_type);
    *mis_start_address = mission_type->start_address;
    *mis_end_address = mission_type->end_address;
    *write_src = smf_data->src;
    *write_size = smf_data->size;

    fprintf(PC, "In smf mission data start   address: %LX\r\n", *mis_start_address);
    fprintf(PC, "In smf mission data end     address: %LX\r\n", *mis_end_address);
    fprintf(PC, "In MIS FM Write source data address: %LX\r\n", *write_src);
    fprintf(PC, "In MIS FM Write data size          : %lu\r\n\r\n", *write_size);

    unsigned int8 used_size_bytes[4];
    read_data_bytes(smf, *mis_start_address, used_size_bytes, 4);
    *used_size = 
       ((unsigned int32)used_size_bytes[3] << 24) |
       ((unsigned int32)used_size_bytes[2] << 16) |
       ((unsigned int32)used_size_bytes[1] << 8)  |
       ((unsigned int32)used_size_bytes[0]);
    *loop_count = read_data_byte(smf, *mis_start_address + 4);

    fprintf(PC, "Size area read\r\n");
    fprintf(PC, "used_size = %lu (src 0x%08LX)\r\n", *used_size, *mis_start_address);
    fprintf(PC, "loop count= %d  (src 0x%08LX)\r\n\r\n", *loop_count, *mis_start_address + 4);
}

#separate
void check_and_erase_if_wrap(unsigned int32 start_addr, unsigned int32 end_addr)
{
    fprintf(PC, "Wrap triggered: Resetting data_write_addr to start\r\n");
    for (unsigned int32 addr = start_addr; addr < end_addr; addr += SUBSECTOR_SIZE)
        subsector_4kByte_erase(smf, addr);
}

#separate
void erase_write_area(unsigned int32 start_addr, unsigned int32 size, unsigned int32 max_addr)
{
    unsigned int32 erase_start = start_addr & ~0xFFF;
    unsigned int32 erase_end = (start_addr + size + 0xFFF) & ~0xFFF;

    for (unsigned int32 addr = erase_start; addr < erase_end && addr < max_addr; addr += SUBSECTOR_SIZE)
        subsector_4kByte_erase(smf, addr);
}

#separate
void write_mission_data(unsigned int32 src, unsigned int32 size, unsigned int32 dst,
                        int8* buffer, unsigned int32* used_size)
{
    unsigned int32 remaining = size;
    unsigned int32 src_addr = src;
    unsigned int32 data_write_addr = dst;

    while (remaining > 0)
    {
        unsigned int16 chunk = (remaining > MAX_READ_SIZE) ? MAX_READ_SIZE : remaining;

        read_data_bytes(mis_fm, src_addr, buffer, chunk);
        write_data_bytes(smf, data_write_addr, buffer, chunk);

        src_addr += chunk;
        data_write_addr += chunk;
        *used_size += chunk;
        remaining -= chunk;
    }
}

#separate
void update_size_area(unsigned int32 base_addr, unsigned int32 used_size, unsigned int8 loop_count)
{
    fprintf(PC, "Update size area\r\n");
    subsector_4kByte_erase(smf, base_addr);

    write_data_byte(smf, base_addr + 0, (used_size >> 0)  & 0xFF);
    write_data_byte(smf, base_addr + 1, (used_size >> 8)  & 0xFF);
    write_data_byte(smf, base_addr + 2, (used_size >> 16) & 0xFF);
    write_data_byte(smf, base_addr + 3, (used_size >> 24) & 0xFF);
    write_data_byte(smf, base_addr + 4, loop_count);

    fprintf(PC, "used_size = %ld\r\n", used_size);
    fprintf(PC, "loop_count = %u\r\n\r\n", loop_count);
}












/*
void copy_data()
{
    fprintf(PC, "\r\n____________________\r\n");
    fprintf(PC, "___Start copy_data__\r\n");

    int8 buffer[MAX_READ_SIZE];

    for (int8 i = 0; !is_empty_smf_data(); i++)
    {
        fprintf(PC, "Start [%d th] data copy\r\n\r\n", i);
        
        // value initialize
        smfDataStruct *smf_data = dequeue_smf_data();
        MissionTypeStruct mission_type = getMissionTypeStruct(smf_data->mission_type);
        status[i+1] = mission_type.mission_flag; // `i` is assigned mis mcu status flag. so mission_flag start `i+1`
        unsigned int32 mis_start_address = mission_type.start_address;
        unsigned int32 mis_end_address = mission_type.end_address;
        unsigned int32 write_src = smf_data->src;
        unsigned int32 write_size = smf_data->size;
        fprintf(PC, "In smf mission data start   address: %LX\r\n", mis_start_address);
        fprintf(PC, "In smf mission data end     address: %LX\r\n", mis_end_address);
        fprintf(PC, "In MIS FM Write source data address: %LX\r\n", write_src);
        fprintf(PC, "In MIS FM Write data size          : %lu (0x%lx)\r\n\r\n", write_size, write_size);
      
      
        // flash setting
        //flash_setting(mis_fm);
        //flash_setting(smf);


        // read size area
        uint8_t used_size_bytes[4];
        read_data_bytes(smf, mis_start_address, used_size_bytes, 4);
        unsigned int32 used_size = 
           ((unsigned int32)used_size_bytes[3] << 24) |
           ((unsigned int32)used_size_bytes[2] << 16) |
           ((unsigned int32)used_size_bytes[1] << 8)  |
           ((unsigned int32)used_size_bytes[0]);
        int8 loop_count = read_data_byte(smf, mis_start_address + 4);
        fprintf(PC, "Size area read\r\n");
        fprintf(PC, "used_size = %lu (src 0x%08LX)\r\n", used_size, mis_start_address);
        fprintf(PC, "loop count= %d  (src 0x%08LX)\r\n\r\n", loop_count, mis_start_address + 4);
        

        // Calculate data write address and check for wrap-around
        unsigned int32 data_region_start = mis_start_address + SUBSECTOR_SIZE;
        unsigned int32 data_region_end = mis_end_address;
        unsigned int32 data_region_size = data_region_end - data_region_start;
        unsigned int32 data_write_addr = data_region_start + used_size;


        // Delete the first part in advance in case of looping
        if ((used_size + write_size) > data_region_size)
        {
            fprintf(PC, "Wrap triggered: Resetting data_write_addr to start\r\n");
            loop_count++;
            used_size = 0;
            data_write_addr = data_region_start;
            unsigned int32 erase_ptr = data_region_start;
            while (erase_ptr < data_region_end)
            {
                subsector_4kByte_erase(smf, erase_ptr);
                erase_ptr += SUBSECTOR_SIZE;     // tips: `value += 0x1000` means add up 4KB (0x1000 = 0d4096)
            }
        }


        // Erase the space you will be writing in now
        unsigned int32 erase_start = data_write_addr & ~0xFFF;       // tips: `value & ~0xFFF` means alignment 4KB.
        unsigned int32 erase_end = (data_write_addr + write_size + 0xFFF) & ~0xFFF;
        for (unsigned int32 addr = erase_start; addr < erase_end && addr < mis_end_address; addr += SUBSECTOR_SIZE)
            subsector_4kByte_erase(smf, addr);

        unsigned int32 remaining = write_size;
        unsigned int32 src_addr = write_src;
        while (remaining > 0)
        {
            unsigned int16 chunk = (remaining > MAX_READ_SIZE) ? MAX_READ_SIZE : remaining; //  = max(MAX_READ_SIZE, rest write_size)

            read_data_bytes(mis_fm, src_addr, buffer, chunk);
            write_data_bytes(smf, data_write_addr, buffer, chunk);

            src_addr += chunk;
            data_write_addr += chunk;
            used_size += chunk;
            remaining -= chunk;
        }


       // write size area
        fprintf(PC, "Update size area\r\n");
        subsector_4kByte_erase(smf, mis_start_address);
        write_data_byte(smf, mis_start_address + 0, (used_size >> 0)  & 0xFF);
        write_data_byte(smf, mis_start_address + 1, (used_size >> 8)  & 0xFF);
        write_data_byte(smf, mis_start_address + 2, (used_size >> 16) & 0xFF);
        write_data_byte(smf, mis_start_address + 3, (used_size >> 24) & 0xFF);
        write_data_byte(smf, mis_start_address + 4, loop_count);
        fprintf(PC, "used_size = %ld\r\n", used_size);
        fprintf(PC, "loop_count = %u\r\n\r\n", loop_count);
    }

    fprintf(PC, "\r\n___End copy_data____\r\n");
    fprintf(PC, "____________________\r\n\r\n");
}
*/


