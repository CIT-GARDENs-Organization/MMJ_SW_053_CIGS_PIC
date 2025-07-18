#include "mcp4901.h"

void mcp4901_init();
{
   output_high(MCP4911_CS_PIN);
}


static void _mcp4901_select(mcp4901_device_t* dev)
{
   output_low(dev->cs_pin);
}

static void _mcp4911_release(mcp4911_device_t* dev)
{
   output_high(dev->cs_pin);
}

static void _mcp4901_write16(mcp4901_device_t* dev, unsigned int16 cmd)
{
   _mcp4901_select(dev);
   dev->write_spi(cmd >> 8);
   dev->write_spi(cmd & 0xFF);
   _mcp4901_release(dev);
}

void mcp4901_init(mcp4901_device_t* dev)
{
   output_high(dev->cs_pin);
   /*
   if (MCP4911_LDAC_PIN != 0xFFFF) {
      output_high(MCP4911_LDAC_PIN);
      set_tris_c(get_tris_c() & ~(1 << (MCP4911_LDAC_PIN & 0x0F)));
   }
   */
}

void mcp4901_write(mcp4901_device_t* dev, unsigned int16 val, int1 gain_1x)
{
   if(val > 1023) val = 1023;

   unsigned int16 cmd = 0;
   cmd |= (1U << 14);
   cmd |= ((gain_1x & 1U) << 13);
   cmd |= (1U << 12);
   cmd |= (val << 2);

   _mcp4901_write16(dev, cmd);
   /*
   if (MCP4911_LDAC_PIN != 0xFFFF) {
      output_low(MCP4911_LDAC_PIN);
      delay_cycles(1);
      output_high(MCP4911_LDAC_PIN);
   }
   */
}

void mcp4901_shutdown(mcp4901_device_t* dev, int1 shutdown)
{
   unsigned int16 cmd = 0;
   cmd |= (1U << 14);
   cmd |= (1U << 13);
   cmd |= ((shutdown ? 0U : 1U) << 12);
   _mcp4901_write16(dev, cmd);
}
