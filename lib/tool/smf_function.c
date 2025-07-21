#include "../device/mt25q.h"


#include <stdint.h>

#define MAX_READ_SIZE 64
#define SIZE_AREA_SIZE 0x1000
#define SUBSECTOR_SIZE 0x1000 // (4096bytes)

void data_copy()
{
    fprintf(PC, "\r\n____________________\r\n");
    fprintf(PC, "___Start smf_write__\r\n");

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
        fprintf(PC, "In smf mission data start   address: %08LX\r\n", mis_start_address);
        fprintf(PC, "In smf mission data end     address: %08LX\r\n", mis_end_address);
        fprintf(PC, "In MIS FM Write source data address: %08LX\r\n", write_src);
        fprintf(PC, "In MIS FM Write data size          : %lu (0x%lx)\r\n\r\n", write_size, write_size);
      
      
        // flash setting
        //flash_setting(mis_fm);
        //flash_setting(smf);
        if (is_connect(smf) == FALSE)
        {
            fprintf(PC, "Error: SMF is not connected\r\n");
            return;
        }


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

    fprintf(PC, "\r\n___End smf_write____\r\n");
    fprintf(PC, "____________________\r\n\r\n");
}


void smf_write()
{
    fprintf(PC, "\r\n____________________\r\n");
    fprintf(PC, "___Start smf_write__\r\n");

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
        fprintf(PC, "In smf mission data start   address: %08LX\r\n", mis_start_address);
        fprintf(PC, "In smf mission data end     address: %08LX\r\n", mis_end_address);
        fprintf(PC, "In MIS FM Write source data address: %08LX\r\n", write_src);
        fprintf(PC, "In MIS FM Write data size          : %lu (0x%lx)\r\n\r\n", write_size, write_size);
      
      
        // flash setting
        //flash_setting(mis_fm);
        //flash_setting(smf);
        if (is_connect(smf) == FALSE)
        {
            fprintf(PC, "Error: SMF is not connected\r\n");
            return;
        }


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

    fprintf(PC, "\r\n___End smf_write____\r\n");
    fprintf(PC, "____________________\r\n\r\n");
}


void smf_read(unsigned int32 address, unsigned int8 *data, unsigned int8 size)
{
    
}



void smf_erase(unsigned int32 address, unsigned int8 sector_size)
{

}

