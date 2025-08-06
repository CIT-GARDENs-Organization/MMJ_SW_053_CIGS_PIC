#include "mmj_cigs_mission_manager.h"
#include "mmj_cigs_flash.h"
#include "../../../lib/tool/mmj_smf_memorymap.h"

// ミッション管理構造体のグローバルインスタンス
CigsMissionStruct cigs_meas_head
CigsMissionStruct cigs_meas = {0};
CigsMissionStruct cigs_piclog = {0};

// ミッション管理関数の実装
void init_cigs_mission_structs(void)
{
    // CIGS測定データミッション初期化
    cigs_meas.mission_id = ID_CIGS_MEASURE_DATA;
    cigs_meas.start_address = ADDRESS_MISF_MEASUREMENT_START;
    cigs_meas.end_address = ADDRESS_MISF_MEASUREMENT_END;
    cigs_meas.used_size = 0;
    cigs_meas.loop_counter = 0;
    cigs_meas.uncopyed_counter = 0;
    cigs_meas.write_counter = 0;
    
    // CIGSピクログミッション初期化
    cigs_piclog.mission_id = ID_CIGS_PICLOG;
    cigs_piclog.start_address = ADDRESS_MISF_PICLOG_DATA_START;
    cigs_piclog.end_address = ADDRESS_MISF_PICLOG_DATA_END;
    cigs_piclog.used_size = 0;
    cigs_piclog.loop_counter = 0;
    cigs_piclog.uncopyed_counter = 0;
    cigs_piclog.write_counter = 0;
    
    fprintf(PC, "CIGS Mission structures initialized\r\n");
    fprintf(PC, "  Measure: 0x%08LX - 0x%08LX\r\n", 
            cigs_meas.start_address, cigs_meas.end_address);
    fprintf(PC, "  Piclog:  0x%08LX - 0x%08LX\r\n", 
         cigs_piclog.start_address, cigs_piclog.end_address);
}

CigsMissionStruct* get_cigs_mission_by_id(int8 mission_id)
{
    switch(mission_id)
    {
        case ID_CIGS_MEASURE_DATA:
            return &cigs_meas;
        case ID_CIGS_PICLOG:
            return  cigs_piclog;
        default:
            fprintf(PC, "Error: Unknown CIGS mission_id %02X\r\n", mission_id);
            return 0x00; // NULL pointer
    }
}

void update_cigs_uncopyed_counter(int8 mission_id, int32 size_change)
{
    CigsMissionStruct* mission = get_cigs_mission_by_id(mission_id);
    if (mission != 0x00)
    {
        if (size_change > 0) {
            mission->uncopyed_counter += size_change;
            fprintf(PC, "CIGS Mission %02X: +%ld uncopied, Total: %ld\r\n", 
                    mission_id, size_change, mission->uncopyed_counter);
        } else if (size_change < 0) {
            int32 decrease = -size_change;
            if (mission->uncopyed_counter >= decrease) {
                mission->uncopyed_counter -= decrease;
                fprintf(PC, "CIGS Mission %02X: -%ld uncopied, Remaining: %ld\r\n", 
                        mission_id, decrease, mission->uncopyed_counter);
            } else {
                fprintf(PC, "Warning: CIGS Mission %02X cannot decrease below 0 (current: %ld, requested: %ld)\r\n", 
                        mission_id, mission->uncopyed_counter, decrease);
                mission->uncopyed_counter = 0;
            }
        }
    }
}

void reset_cigs_uncopyed_counter(int8 mission_id)
{
    CigsMissionStruct* mission = get_cigs_mission_by_id(mission_id);
    if (mission != 0x00)
    {
        mission->uncopyed_counter = 0;
        fprintf(PC, "CIGS Mission %02X: uncopied counter reset\r\n", mission_id);
    }
}

CigsMissionStruct get_cigs_mission_struct(int8 mission_id)
{
    CigsMissionStruct mis_struct = {0};
    
    switch(mission_id)
    {
        case ID_CIGS_MEASURE_DATA:
            mis_struct.mission_id = ID_CIGS_MEASURE_DATA;
            mis_struct.start_address = ADDRESS_MISF_MEASUREMENT_START;
            mis_struct.end_address = ADDRESS_MISF_MEASUREMENT_END;
            break;
            
        case ID_CIGS_PICLOG:
            mis_struct.mission_id = ID_CIGS_PICLOG;
            mis_struct.start_address = ADDRESS_MISF_PICLOG_DATA_START;
            mis_struct.end_address = ADDRESS_MISF_PICLOG_DATA_END;
            break;
            
        default:
            fprintf(PC, "Error: Unknown CIGS mission_id %02X\r\n", mission_id);
            break;
    }
    
    return mis_struct;
}
