// ===================== mcp4911.h =====================
#ifndef _MCP4901_H_
#define _MCP4901_H_

// DACデバイスを識別するための構造体
typedef struct {
   void (*write_spi)(unsigned int8);       // 使用するSPIストリームID
   unsigned int16 cs_pin; // チップセレクトピン（ピン番号）
} mcp4911_device_t;

// ---------------------- Public API --------------------------
void mcp4911_init(mcp4911_device_t* dev);
void mcp4911_write(mcp4911_device_t* dev, unsigned int16 val, int1 gain_1x);
void mcp4911_shutdown(mcp4911_device_t* dev, int1 shutdown);

#include "mcp4901.c"

#endif /* _MCP4911_H_ */
// ===================== mcp4911.h =====================
