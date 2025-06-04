#ifndef     MMJ_CIGS_FLASH_H
#define     MMJ_CIGS_FLASH_H

void misf_init(void);
void misf_read_id(void);




// Flash Area

// PICLOG
#define ADDRESS_PICLOG_START 0x000000
#define ADDRESS_PICLOG_END   0x000FFF










#include "../mmj_cigs_flash.c"

#endif // MMJ_CIGS_FLASH_H
//------------------End of File------------------