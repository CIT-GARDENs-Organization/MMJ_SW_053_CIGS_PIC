# SPI Flash Memory Operation Library - Deep Wiki Documentation

## 🔍 Overview

**File:** `flash.c`  
**Version:** 2.2  
**Author:** T. Kawai  
**Purpose:** SPI Flash Memory Operation Library for embedded systems

This library provides comprehensive SPI flash memory operations supporting two Micron flash memory models with different addressing schemes. The library implements a robust, multi-stream SPI communication interface with timeout handling and error detection.

---

## 📋 Table of Contents

1. [Supported Hardware](#supported-hardware)
2. [Command Reference](#command-reference)
3. [Data Structures](#data-structures)
4. [Core Functions](#core-functions)
5. [SPI Communication Layer](#spi-communication-layer)
6. [Flash Operations](#flash-operations)
7. [Error Handling](#error-handling)
8. [Usage Examples](#usage-examples)
9. [Memory Organization](#memory-organization)
10. [Best Practices](#best-practices)
11. [Troubleshooting](#troubleshooting)

---

## 🔧 Supported Hardware

### Flash Memory Models

| Model | Addressing | Capacity | Notes |
|-------|------------|----------|-------|
| **MT25QL128ABA** | 3-byte (24-bit) | 128 Mbit (16 MB) | Standard commands |
| **MT25QL01GBBB** | 4-byte (32-bit) | 1 Gbit (128 MB) | Extended commands |

### SPI Stream Configuration

The library supports 4 independent SPI streams:
- `SPI_0` → `FLASH_STREAM0`
- `SPI_1` → `FLASH_STREAM1`
- `SPI_2` → `FLASH_STREAM2`
- `SPI_3` → `FLASH_STREAM3`

---

## 📖 Command Reference

### Universal Commands
```c
#define CMD_READ_ID                     0x9F    // Read JEDEC ID
#define CMD_READ_STATUS_REGISTER        0x05    // Read status register
#define CMD_WRITE_ENABLE                0x06    // Enable write operations
```

### 3-Byte Addressing Commands (MT25QL128ABA)
```c
#define CMD_READ                        0x03    // Read data
#define CMD_PAGE_PROGRAM                0x02    // Program page
#define CMD_SUBSECTOR_4KB_ERASE         0x20    // Erase 4KB subsector
#define CMD_SUBSECTOR_32KB_ERASE        0x52    // Erase 32KB subsector
#define CMD_SECTOR_ERASE                0xD8    // Erase 64KB sector
```

### 4-Byte Addressing Commands (MT25QL01GBBB)
```c
#define CMD_4BYTE_READ                  0x13    // Read data (4-byte addr)
#define CMD_4BYTE_PAGE_PROGRAM          0x12    // Program page (4-byte addr)
#define CMD_4BYTE_SUBSECTOR_4KB_ERASE   0x21    // Erase 4KB subsector (4-byte addr)
#define CMD_4BYTE_SUBSECTOR_32KB_ERASE  0x5C    // Erase 32KB subsector (4-byte addr)
#define CMD_4BYTE_SECTOR_ERASE          0xDC    // Erase 64KB sector (4-byte addr)
```

---

## 🏗️ Data Structures

### Flash Structure
```c
typedef struct {
    int spi_stream_id;      // SPI stream identifier (SPI_0 to SPI_3)
    int cs_pin;             // Chip select pin
    int flash_model;        // Flash model (MT25QL128ABA or MT25QL01GBBB)
} Flash;
```

### Flash Model Constants
```c
enum FlashModels {
    MT25QL128ABA = 1,       // 128 Mbit flash with 3-byte addressing
    MT25QL01GBBB = 2        // 1 Gbit flash with 4-byte addressing
};
```

---

## 🔧 Core Functions

### Initialization & Configuration

#### `flash_setting(Flash flash_stream)`
**Purpose:** Initialize flash communication interface  
**Parameters:**
- `flash_stream`: Flash configuration structure

```c
void flash_setting(Flash flash_stream) {
    output_high(flash_stream.cs_pin);  // Deselect flash (CS high)
}
```

**Usage:**
```c
Flash my_flash = {SPI_0, PIN_C0, MT25QL128ABA};
flash_setting(my_flash);
```

#### `read_id(Flash flash_stream)`
**Purpose:** Read and verify flash chip identification  
**Parameters:**
- `flash_stream`: Flash configuration structure
**Returns:** 
- `true` (1): Flash connected and recognized
- `false` (0): Flash not detected or invalid

```c
int8 read_id(Flash flash_stream) {
    int8 flash_cmd = CMD_READ_ID;
    int8 chip_id[20];
    
    output_low(flash_stream.cs_pin);
    spi_xfer_and_read_select_stream(flash_stream, &flash_cmd, 1, chip_id, 20);
    output_high(flash_stream.cs_pin);
    
    // Verify manufacturer ID (0x20 = Micron)
    if(chip_id[0] == 0x20) {
        return true;    // Flash detected
    }
    return false;       // Flash not detected
}
```

#### `status_register(Flash flash_stream)`
**Purpose:** Read flash status register  
**Parameters:**
- `flash_stream`: Flash configuration structure
**Returns:** 8-bit status register value

**Status Register Bits:**
- Bit 0: Write In Progress (WIP) - 1 = busy, 0 = ready
- Bit 1: Write Enable Latch (WEL)
- Bit 2-7: Various status flags

```c
int8 status_register(Flash flash_stream) {
    int8 flash_cmd = CMD_READ_STATUS_REGISTER;
    int8 status_reg;
    
    output_low(flash_stream.cs_pin);
    spi_xfer_and_read_select_stream(flash_stream, &flash_cmd, 1, &status_reg, 1);
    output_high(flash_stream.cs_pin);
    
    return status_reg;
}
```

---

## 📡 SPI Communication Layer

### Low-Level SPI Functions

#### `spi_xfer_select_stream()`
**Purpose:** Send multiple bytes to selected SPI stream  
**Parameters:**
- `flash_stream`: Flash configuration structure
- `write_data`: Pointer to data array to send
- `write_amount`: Number of bytes to send

#### `spi_xfer_and_read_select_stream()`
**Purpose:** Send command then read response  
**Parameters:**
- `flash_stream`: Flash configuration structure
- `write_data`: Command data to send
- `write_amount`: Number of command bytes
- `read_data`: Buffer for received data
- `read_amount`: Number of bytes to read

#### `spi_xfer_two_datas_select_stream()`
**Purpose:** Send command followed by data payload  
**Parameters:**
- `flash_stream`: Flash configuration structure
- `cmd_data`: Command bytes
- `cmd_amount`: Number of command bytes
- `write_data`: Data payload
- `write_amount`: Number of data bytes

---

## 💾 Flash Operations

### Erase Operations

#### `sector_erase(Flash flash_stream, unsigned int32 sector_address)`
**Purpose:** Erase 64KB sector  
**Parameters:**
- `flash_stream`: Flash configuration structure
- `sector_address`: Starting address of sector to erase

**Address Alignment:**
- MT25QL128ABA: Must be aligned to 64KB boundary
- MT25QL01GBBB: Must be aligned to 64KB boundary

**Erase Time:** ~150ms to 2s depending on flash model

```c
// Example: Erase sector starting at address 0x10000 (64KB)
sector_erase(my_flash, 0x10000);
```

#### `subsector_32kByte_erase(Flash flash_stream, unsigned int32 subsector_address)`
**Purpose:** Erase 32KB subsector  
**Parameters:**
- `flash_stream`: Flash configuration structure
- `subsector_address`: Starting address of subsector

**Address Alignment:** Must be aligned to 32KB boundary  
**Erase Time:** ~120ms to 1.6s

```c
// Example: Erase 32KB subsector at address 0x8000
subsector_32kByte_erase(my_flash, 0x8000);
```

#### `subsector_4kByte_erase(Flash flash_stream, unsigned int32 subsector_address)`
**Purpose:** Erase 4KB subsector  
**Parameters:**
- `flash_stream`: Flash configuration structure
- `subsector_address`: Starting address of subsector

**Address Alignment:** Must be aligned to 4KB boundary  
**Erase Time:** ~25ms to 400ms

```c
// Example: Erase 4KB subsector at address 0x1000
subsector_4kByte_erase(my_flash, 0x1000);
```

### Read Operations

#### `read_data_byte(Flash flash_stream, unsigned int32 read_address)`
**Purpose:** Read single byte from flash  
**Parameters:**
- `flash_stream`: Flash configuration structure
- `read_address`: Address to read from
**Returns:** 8-bit data value

```c
// Read byte from address 0x1000
int8 data = read_data_byte(my_flash, 0x1000);
```

#### `read_data_bytes(Flash flash_stream, unsigned int32 read_start_address, int8 *read_data, unsigned int32 read_amount)`
**Purpose:** Read multiple bytes from flash  
**Parameters:**
- `flash_stream`: Flash configuration structure
- `read_start_address`: Starting address
- `read_data`: Buffer to store read data
- `read_amount`: Number of bytes to read

```c
// Read 256 bytes starting from address 0x1000
int8 buffer[256];
read_data_bytes(my_flash, 0x1000, buffer, 256);
```

### Write Operations

#### `write_data_byte(Flash flash_stream, unsigned int32 write_address, int8 write_data)`
**Purpose:** Write single byte to flash  
**Parameters:**
- `flash_stream`: Flash configuration structure
- `write_address`: Address to write to
- `write_data`: 8-bit data to write

**Important Notes:**
- Target location must be erased (0xFF) before writing
- Write operation automatically handles write enable sequence
- Function waits for completion before returning

```c
// Write byte 0xAA to address 0x1000
write_data_byte(my_flash, 0x1000, 0xAA);
```

#### `write_data_bytes(Flash flash_stream, unsigned int32 write_start_address, int8 *write_data, unsigned int16 write_amount)`
**Purpose:** Write multiple bytes to flash  
**Parameters:**
- `flash_stream`: Flash configuration structure
- `write_start_address`: Starting address
- `write_data`: Data array to write
- `write_amount`: Number of bytes to write

**Page Programming Constraints:**
- Maximum 256 bytes per page program operation
- Cannot cross page boundaries (256-byte aligned)
- All target locations must be erased first

```c
// Write 100 bytes to flash starting at address 0x1000
int8 data[100] = {0x01, 0x02, 0x03, ...};
write_data_bytes(my_flash, 0x1000, data, 100);
```

---

## 🚨 Error Handling

### Timeout Mechanism
All erase and write operations include timeout protection:

```c
unsigned int8 timeout_counter = 0;
while((status_register(flash_stream) & 0x01) == 1) {  // Check WIP bit
    if(timeout_counter > 10)
        delay_ms(200);  // Longer delay for extended operations
    else
        delay_ms(10);   // Short delay for quick operations
    
    if(timeout_counter > 100) {
        // Timeout occurred - operation failed
        break;
    }
    timeout_counter++;
}
```

### Status Register Monitoring
The library continuously monitors the Write In Progress (WIP) bit:
- `(status_register(flash_stream) & 0x01) == 1`: Flash is busy
- `(status_register(flash_stream) & 0x01) == 0`: Flash is ready

### Debug Output
When `DEBUG` is defined, the library provides detailed operation feedback:
```c
#ifdef DEBUG
    fprintf(DEBUG_PORT, "flash sector erase complete\r\n");
#endif
```

---

## 🎯 Usage Examples

### Complete Flash Initialization
```c
#include "flash.h"

// Define flash configuration
Flash external_flash = {
    .spi_stream_id = SPI_0,
    .cs_pin = PIN_C0,
    .flash_model = MT25QL128ABA
};

void flash_init_example() {
    // Initialize flash interface
    flash_setting(external_flash);
    
    // Verify flash connection
    if(read_id(external_flash)) {
        printf("Flash initialized successfully\n");
    } else {
        printf("Flash initialization failed\n");
        return;
    }
}
```

### Data Storage Example
```c
void data_storage_example() {
    unsigned int32 base_address = 0x10000;  // 64KB aligned
    int8 write_buffer[256];
    int8 read_buffer[256];
    
    // Prepare test data
    for(int i = 0; i < 256; i++) {
        write_buffer[i] = i;
    }
    
    // Erase sector before writing
    sector_erase(external_flash, base_address);
    
    // Write data
    write_data_bytes(external_flash, base_address, write_buffer, 256);
    
    // Read back and verify
    read_data_bytes(external_flash, base_address, read_buffer, 256);
    
    // Verify data integrity
    int8 error_count = 0;
    for(int i = 0; i < 256; i++) {
        if(write_buffer[i] != read_buffer[i]) {
            error_count++;
        }
    }
    
    if(error_count == 0) {
        printf("Data verification successful\n");
    } else {
        printf("Data verification failed: %d errors\n", error_count);
    }
}
```

### Multiple Flash Management
```c
// Define multiple flash devices
Flash flash_array[2] = {
    {SPI_0, PIN_C0, MT25QL128ABA},  // 128Mbit flash on SPI_0
    {SPI_1, PIN_C1, MT25QL01GBBB}   // 1Gbit flash on SPI_1
};

void multi_flash_example() {
    for(int i = 0; i < 2; i++) {
        flash_setting(flash_array[i]);
        
        if(read_id(flash_array[i])) {
            printf("Flash %d: Connected\n", i);
        } else {
            printf("Flash %d: Not detected\n", i);
        }
    }
}
```

---

## 🗺️ Memory Organization

### MT25QL128ABA (128 Mbit / 16 MB)
```
Address Range: 0x000000 - 0xFFFFFF (24-bit addressing)

Memory Hierarchy:
├── 256 Sectors (64KB each)
├── 512 Subsectors (32KB each)  
└── 4096 Subsectors (4KB each)

Page Size: 256 bytes
Sector Size: 64KB (65,536 bytes)
Total Capacity: 16,777,216 bytes
```

### MT25QL01GBBB (1 Gbit / 128 MB)
```
Address Range: 0x00000000 - 0x07FFFFFF (32-bit addressing)

Memory Hierarchy:
├── 2048 Sectors (64KB each)
├── 4096 Subsectors (32KB each)
└── 32768 Subsectors (4KB each)

Page Size: 256 bytes
Sector Size: 64KB (65,536 bytes)
Total Capacity: 134,217,728 bytes
```

### Address Alignment Requirements

| Operation | Alignment | MT25QL128ABA | MT25QL01GBBB |
|-----------|-----------|--------------|--------------|
| Page Program | 256 bytes | 0x000000-0x0000FF | 0x00000000-0x000000FF |
| 4KB Erase | 4KB | 0x001000 | 0x00001000 |
| 32KB Erase | 32KB | 0x008000 | 0x00008000 |
| 64KB Erase | 64KB | 0x010000 | 0x00010000 |

---

## ✅ Best Practices

### 1. Flash Initialization Sequence
```c
// Recommended initialization order
1. Configure SPI hardware
2. Set up CS pin as output
3. Call flash_setting()
4. Verify connection with read_id()
5. Check status register for ready state
```

### 2. Write Operations
```c
// Always follow this sequence for writes
1. Ensure target area is erased (contains 0xFF)
2. Call appropriate erase function if needed
3. Wait for erase completion
4. Perform write operation
5. Verify written data if critical
```

### 3. Erase Strategy
```c
// Choose appropriate erase size
- Use 4KB erase for small data updates
- Use 32KB erase for medium-sized data blocks
- Use 64KB erase for large data areas or bulk operations
```

### 4. Error Handling
```c
// Always check operation results
if(!read_id(flash)) {
    // Handle initialization failure
    return ERROR_FLASH_NOT_DETECTED;
}

// Monitor status for long operations
int8 status = status_register(flash);
if(status & 0x01) {
    // Flash is still busy
    // Wait or handle accordingly
}
```

### 5. Power Management
```c
// For battery-powered applications
- Check flash status before power-down
- Ensure all operations complete before sleep
- Consider deep power-down commands if supported
```

---

## 🔧 Troubleshooting

### Common Issues and Solutions

#### 1. Flash Not Detected (`read_id()` returns false)

**Possible Causes:**
- Incorrect SPI wiring
- Wrong CS pin configuration
- SPI clock speed too high
- Flash not powered properly

**Solutions:**
```c
// Check SPI configuration
- Verify MOSI, MISO, SCK connections
- Ensure CS pin is correctly defined
- Try lower SPI clock frequency
- Check power supply voltage (typically 2.7V-3.6V)
```

#### 2. Write Operations Fail

**Possible Causes:**
- Target area not erased
- Page boundary crossing
- Flash is write-protected

**Solutions:**
```c
// Proper write sequence
1. Erase target area first
2. Verify erase completion
3. Check write enable status
4. Ensure write size doesn't cross page boundaries
```

#### 3. Timeout Errors

**Possible Causes:**
- Flash hardware failure
- Incorrect timing parameters
- SPI communication errors

**Solutions:**
```c
// Increase timeout values for debugging
if(timeout_counter > 200) {  // Increased from 100
    // Extended timeout for problematic flash
}

// Check status register manually
int8 status = status_register(flash);
if(status & 0x01) {
    printf("Flash still busy: 0x%02X\n", status);
}
```

#### 4. Data Corruption

**Possible Causes:**
- Insufficient erase before write
- SPI timing issues
- Power supply noise

**Solutions:**
```c
// Implement data verification
write_data_bytes(flash, addr, data, size);
read_data_bytes(flash, addr, verify_buffer, size);

// Compare and retry if needed
if(memcmp(data, verify_buffer, size) != 0) {
    // Retry operation or flag error
}
```

### Debug Checklist

1. **Hardware Verification:**
   - [ ] SPI connections verified with oscilloscope
   - [ ] CS pin toggles correctly
   - [ ] Power supply stable
   - [ ] Clock frequency within flash specifications

2. **Software Verification:**
   - [ ] Flash structure properly initialized
   - [ ] SPI stream configuration matches hardware
   - [ ] Erase operations complete before write
   - [ ] Address alignment requirements met

3. **Communication Verification:**
   - [ ] JEDEC ID reads correctly (0x20 for Micron)
   - [ ] Status register responds to commands
   - [ ] Write enable sequence working
   - [ ] Timeout handling appropriate for flash speed

---

## 📚 Additional Resources

### Related Documentation
- Micron MT25QL128ABA Datasheet
- Micron MT25QL01GBBB Datasheet
- SPI Protocol Specification
- CCS C Compiler SPI Functions Reference

### Development Tools
- Logic analyzer for SPI debugging
- Flash programming utilities
- Memory hex editors for data verification

### Performance Specifications

| Operation | MT25QL128ABA | MT25QL01GBBB | Notes |
|-----------|-------------|-------------|-------|
| Read Speed | Up to 108 MHz | Up to 108 MHz | Clock dependent |
| Page Program | 0.4-3ms | 0.4-3ms | 256 bytes max |
| 4KB Erase | 25-400ms | 25-400ms | Typical 45ms |
| 32KB Erase | 120-1600ms | 120-1600ms | Typical 200ms |
| 64KB Erase | 150-2000ms | 150-2000ms | Typical 300ms |

---

## 📄 Version History

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 2.2 | Current | T. Kawai | Current implementation |
| 2.1 | Previous | T. Kawai | Previous features |
| 2.0 | Earlier | T. Kawai | Major revision |

---

**© 2024 - SPI Flash Memory Operation Library Documentation**  
**Last Updated:** Current  
**Maintainer:** T. Kawai

*This documentation provides comprehensive coverage of the flash.c library. For specific implementation details, refer to the source code and manufacturer datasheets.*
