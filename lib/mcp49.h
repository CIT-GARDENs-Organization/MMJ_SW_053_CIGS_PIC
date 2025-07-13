// ===================== mcp4911.h =====================
#ifndef _MCP4911_H_
#define _MCP4911_H_

// DACデバイスを識別するための構造体
typedef struct {
   int1 id;               // ユニット番号 (0 or 1)
   int8 spi_stream;       // 使用するSPIストリームID
   unsigned int16 cs_pin; // チップセレクトピン（ピン番号）
} mcp4911_device_t;

// ---------------------- Public API --------------------------
void mcp4911_init(const mcp4911_device_t* dev);
void mcp4911_write(const mcp4911_device_t* dev, unsigned int16 val, int1 gain_1x);
void mcp4911_shutdown(const mcp4911_device_t* dev, int1 shutdown);

#endif /* _MCP4911_H_ */
// ===================== mcp4911.h =====================
