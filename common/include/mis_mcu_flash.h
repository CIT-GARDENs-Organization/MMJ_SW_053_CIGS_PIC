#ifndef MIS_MCU_FLASH_H
#define MIS_MCU_FLASH_H


#define EXAMPLE_APRS_HEADER   0x00000000
#define EXAMPLE_APRS          0x00001000
#define EXAMPLE_SENSOR_HEADR  0x0000A000
#define EXAMPLE_SENSOR        0x00010000




void copy_data(void);

SmfDataStruct *dequeue_smf_data(void);

int1 is_empty_smf_data(void);

#endif  // MIS_MCU_FLASH_H
