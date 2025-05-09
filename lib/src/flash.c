///////////////////////////////////////////////////////////////////////////////
//////////////////////////GDNS_226_FlashOperation.c////////////////////////////
//////////////////////////////////ver 2.2//////////////////////////////////////
/////////////////////////////last editor:T Kawai///////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////redefinition of spi communication///////////////////////
//send multi bytes
void spi_xfer_select_stream(Flash flash_stream, int8 *write_data, unsigned int16 write_amount){
   switch(flash_stream.spi_stream_id){
      case SPI_0:
         for(unsigned int16 spi_xfer_num = 0;spi_xfer_num < write_amount;spi_xfer_num++)
            spi_xfer(FLASH_STREAM0,write_data[spi_xfer_num]);
         break;
         
      case SPI_1:
         for(unsigned int16 spi_xfer_num = 0;spi_xfer_num < write_amount;spi_xfer_num++)
            spi_xfer(FLASH_STREAM1,write_data[spi_xfer_num]);
         break;
   
      case SPI_2:
         for(unsigned int16 spi_xfer_num = 0;spi_xfer_num < write_amount;spi_xfer_num++)
            spi_xfer(FLASH_STREAM2,write_data[spi_xfer_num]);
         break;
   
      case SPI_3:
         for(unsigned int16 spi_xfer_num = 0;spi_xfer_num < write_amount;spi_xfer_num++)
            spi_xfer(FLASH_STREAM3,write_data[spi_xfer_num]);
         break;
         
      default:
         break;
   }
   return;
}

//send multi bytes then receive multi bytes 
void spi_xfer_and_read_select_stream(Flash flash_stream, int8 *write_data, unsigned int16 write_amount, int8 *read_data, unsigned int32 read_amount){
   switch(flash_stream.spi_stream_id){
      case SPI_0:
      for(unsigned int16 spi_xfer_num = 0;spi_xfer_num < write_amount;spi_xfer_num++)
         spi_xfer(FLASH_STREAM0, write_data[spi_xfer_num]);
      for(unsigned int32 spi_rcv_num = 0;spi_rcv_num < read_amount;spi_rcv_num++)
         read_data[spi_rcv_num] = spi_xfer(FLASH_STREAM0);
         break;
         
      case SPI_1:
         for(unsigned int16 spi_xfer_num = 0;spi_xfer_num < write_amount;spi_xfer_num++)
            spi_xfer(FLASH_STREAM1, write_data[spi_xfer_num]);
         for(unsigned int32 spi_rcv_num = 0;spi_rcv_num < read_amount;spi_rcv_num++)
            read_data[spi_rcv_num] = spi_xfer(FLASH_STREAM1);
         break;
   
      case SPI_2:
         for(unsigned int16 spi_xfer_num = 0;spi_xfer_num < write_amount;spi_xfer_num++)
            spi_xfer(FLASH_STREAM2, write_data[spi_xfer_num]);
         for(unsigned int32 spi_rcv_num = 0;spi_rcv_num < read_amount;spi_rcv_num++)
            read_data[spi_rcv_num] = spi_xfer(FLASH_STREAM2);
         break;
   
      case SPI_3:
         for(unsigned int16 spi_xfer_num = 0;spi_xfer_num < write_amount;spi_xfer_num++)
            spi_xfer(FLASH_STREAM3, write_data[spi_xfer_num]);
         for(unsigned int32 spi_rcv_num = 0;spi_rcv_num < read_amount;spi_rcv_num++)
            read_data[spi_rcv_num] = spi_xfer(FLASH_STREAM3);
         break;
         
      default:
         break;
   }
   return;
}

//send multi bytes(ex:cmd) then send other multi bytes(for write multi bytes) 
void spi_xfer_two_datas_select_stream(Flash flash_stream, int8 *cmd_data, unsigned int8 cmd_amount, int8 *write_data, unsigned int16 write_amount){
   switch(flash_stream.spi_stream_id){
      case SPI_0:
      for(unsigned int8 spi_xfer_num = 0;spi_xfer_num < cmd_amount;spi_xfer_num++)
         spi_xfer(FLASH_STREAM0, cmd_data[spi_xfer_num]);
      for(unsigned int16 spi_xfer_num = 0;spi_xfer_num < write_amount;spi_xfer_num++)
         spi_xfer(FLASH_STREAM0, write_data[spi_xfer_num]);
         break;
         
      case SPI_1:
         for(unsigned int8 spi_xfer_num = 0;spi_xfer_num < cmd_amount;spi_xfer_num++)
            spi_xfer(FLASH_STREAM1, cmd_data[spi_xfer_num]);
         for(unsigned int16 spi_xfer_num = 0;spi_xfer_num < write_amount;spi_xfer_num++)
            spi_xfer(FLASH_STREAM1, write_data[spi_xfer_num]);
         break;
   
      case SPI_2:
         for(unsigned int8 spi_xfer_num = 0;spi_xfer_num < cmd_amount;spi_xfer_num++)
            spi_xfer(FLASH_STREAM2, cmd_data[spi_xfer_num]);
         for(unsigned int16 spi_xfer_num = 0;spi_xfer_num < write_amount;spi_xfer_num++)
            spi_xfer(FLASH_STREAM2, write_data[spi_xfer_num]);
         break;
   
      case SPI_3:
         for(unsigned int8 spi_xfer_num = 0;spi_xfer_num < cmd_amount;spi_xfer_num++)
            spi_xfer(FLASH_STREAM3, cmd_data[spi_xfer_num]);
         for(unsigned int16 spi_xfer_num = 0;spi_xfer_num < write_amount;spi_xfer_num++)
            spi_xfer(FLASH_STREAM3, write_data[spi_xfer_num]);
         break;
         
      default:
         break;
   }
   return;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void flash_setting(Flash flash_stream){
   output_high(flash_stream.cs_pin);
}

int8 status_register(Flash flash_stream){
   int8 flash_cmd = CMD_READ_STATUS_REGISTER;
   int8 status_reg;
   output_low(flash_stream.cs_pin);                                              //lower the CS PIN
   spi_xfer_and_read_select_stream(flash_stream, &flash_cmd, 1, &status_reg, 1);
   output_high(flash_stream.cs_pin);                                             //take CS PIN higher back
   #ifdef DEBUG
      if((status_reg & 0x01) == true)                                          //masking status bit   
         fprintf(DEBUG_PORT,"flash busy\n\r");
   #endif
   return status_reg;  
}

//
//->success:True,fail:false
int8 read_id(Flash flash_stream){
   int8 flash_cmd = CMD_READ_ID;
   int8 chip_id[20];
   output_low(flash_stream.cs_pin);
   spi_xfer_and_read_select_stream(flash_stream, &flash_cmd, 1, chip_id, 20);
   output_high(flash_stream.cs_pin);
   #ifdef DEBUG
      fprintf(DEBUG_PORT,"Read ID:");
      for(int8 print_counter = 0;print_counter < 20;print_counter++)
         fprintf(DEBUG_PORT,"%x ",chip_id[print_counter]);
      fprintf(DEBUG_PORT,"\r\n");
   #endif
   //chip id check
   if(chip_id[0] == 0x20){  
      #ifdef DEBUG
         fprintf(DEBUG_PORT,"flash connect OK\r\n");
      #endif
      return true;
   }
   else{
      #ifdef DEBUG
         fprintf(DEBUG_PORT,"flash not connect\r\n");
      #endif
      return false;
   }
}

void sector_erase(Flash flash_stream, unsigned int32 sector_address){
//!   #ifdef DEBUG
//!      fprintf(DEBUG_PORT,"Sector Erase\r\n");
//!   #endif
   if(flash_stream.flash_model == MT25QL128ABA){
      int8 write_enable_cmd = CMD_WRITE_ENABLE;
      unsigned int8 flash_cmd[4];
//!      #ifdef DEBUG
//!         fprintf(DEBUG_PORT,"FLASH MODEL:MT25QL128ABA\r\n");
//!      #endif
      flash_cmd[0] = CMD_SECTOR_ERASE;
      flash_cmd[1] = (unsigned int8)((sector_address>>16) & 0xff);   // 0x 00 _ _ 00 00
      flash_cmd[2] = (unsigned int8)((sector_address>>8) & 0xff);    // 0x 00 00 _ _ 00
      flash_cmd[3] = (unsigned int8)((sector_address) & 0xff);       // 0x 00 00 00 _ _
            
      //Write enable sequence
      output_low(flash_stream.cs_pin);
      spi_xfer_select_stream(flash_stream, &write_enable_cmd, 1);
      output_high(flash_stream.cs_pin);
      //Write enabled
      
      output_low(flash_stream.cs_pin);
      ///////////////////////////////////////////////////////////////////
      spi_xfer_select_stream(flash_stream, flash_cmd, 4);
      //////////////////////////////////////////////////////////////////
      output_high(flash_stream.cs_pin);            //take CS PIN higher back
   }
   else if(flash_stream.flash_model == MT25QL01GBBB){
      int8 write_enable_cmd = CMD_WRITE_ENABLE;
      unsigned int8 flash_cmd[5];
//!      #ifdef DEBUG
//!         fprintf(DEBUG_PORT,"FLASH MODEL:MT25QL01GBBB\r\n");
//!      #endif
      flash_cmd[0] = CMD_4BYTE_SECTOR_ERASE;
      flash_cmd[1] = (unsigned int8)((sector_address>>24) & 0xff);   // 0x _ _ 00 00 00
      flash_cmd[2] = (unsigned int8)((sector_address>>16) & 0xff);   // 0x 00 _ _ 00 00
      flash_cmd[3] = (unsigned int8)((sector_address>>8) & 0xff);    // 0x 00 00 _ _ 00
      flash_cmd[4] = (unsigned int8)((sector_address) & 0xff);       // 0x 00 00 00 _ _
      
      //Write enable sequence
      output_low(flash_stream.cs_pin);
      spi_xfer_select_stream(flash_stream, &write_enable_cmd, 1);
      output_high(flash_stream.cs_pin);
      //Write enabled
      
      output_low(flash_stream.cs_pin);
      ///////////////////////////////////////////////////////////////////
      spi_xfer_select_stream(flash_stream, flash_cmd, 5);
      //////////////////////////////////////////////////////////////////
      output_high(flash_stream.cs_pin);            //take CS PIN higher back
   }
   
   else{
      #ifdef DEBUG
         fprintf(DEBUG_PORT,"error:flash model is invalid\r\n");
      #endif
   }
   
   //wait process finished
   unsigned int8 timeout_counter = 0;
   while((status_register(flash_stream) & 0x01) == 1){      //masking status bit
      if(timeout_counter > 10)
         delay_ms(200);
      else
         delay_ms(10);   
      
      if(timeout_counter > 100){
         #ifdef DEBUG
            fprintf(DEBUG_PORT,"flash timeout\r\n");
         #endif
         break;
      }  
      timeout_counter++;
   }
   #ifdef DEBUG
      fprintf(DEBUG_PORT,"flash sector erase complete\r\n");
   #endif
   return;
}

void subsector_32kByte_erase(Flash flash_stream, unsigned int32 subsector_address){
   if(flash_stream.flash_model == MT25QL128ABA){
      int8 write_enable_cmd = CMD_WRITE_ENABLE;
      unsigned int8 flash_cmd[4];
      
      flash_cmd[0] = CMD_SUBSECTOR_32KB_ERASE;
      flash_cmd[1] = (unsigned int8)((subsector_address>>16) & 0xff);   // 0x 00 _ _ 00 00
      flash_cmd[2] = (unsigned int8)((subsector_address>>8) & 0xff);    // 0x 00 00 _ _ 00
      flash_cmd[3] = (unsigned int8)((subsector_address) & 0xff);       // 0x 00 00 00 _ _
            
      //Write enable sequence
      output_low(flash_stream.cs_pin);
      spi_xfer_select_stream(flash_stream, &write_enable_cmd, 1);
      output_high(flash_stream.cs_pin);
      //Write enabled
      
      output_low(flash_stream.cs_pin);
      ///////////////////////////////////////////////////////////////////
      spi_xfer_select_stream(flash_stream, flash_cmd, 4);
      //////////////////////////////////////////////////////////////////
      output_high(flash_stream.cs_pin);            //take CS PIN higher back
   }
   else if(flash_stream.flash_model == MT25QL01GBBB){
      int8 write_enable_cmd = CMD_WRITE_ENABLE;
      unsigned int8 flash_cmd[5];
      
      flash_cmd[0] = CMD_4BYTE_SUBSECTOR_32KB_ERASE;
      flash_cmd[1] = (unsigned int8)((subsector_address>>24) & 0xff);   // 0x _ _ 00 00 00
      flash_cmd[2] = (unsigned int8)((subsector_address>>16) & 0xff);   // 0x 00 _ _ 00 00
      flash_cmd[3] = (unsigned int8)((subsector_address>>8) & 0xff);    // 0x 00 00 _ _ 00
      flash_cmd[4] = (unsigned int8)((subsector_address) & 0xff);       // 0x 00 00 00 _ _
      
      //Write enable sequence
      output_low(flash_stream.cs_pin);
      spi_xfer_select_stream(flash_stream, &write_enable_cmd, 1);
      output_high(flash_stream.cs_pin);
      //Write enabled
      
      output_low(flash_stream.cs_pin);
      ///////////////////////////////////////////////////////////////////
      spi_xfer_select_stream(flash_stream, flash_cmd, 5);
      //////////////////////////////////////////////////////////////////
      output_high(flash_stream.cs_pin);            //take CS PIN higher back
   }
   
   else{
      #ifdef DEBUG
         fprintf(DEBUG_PORT,"error:flash model is invalid\r\n");
      #endif
   }
   
   //wait process finished
   unsigned int8 timeout_counter = 0;
   while((status_register(flash_stream) & 0x01) == 1){                  //masking status bit
      if(timeout_counter > 10)
         delay_ms(200);
      else
         delay_ms(10);   
      
      if(timeout_counter > 100){
         #ifdef DEBUG
            fprintf(DEBUG_PORT,"flash timeout\r\n");
         #endif
         break;
      }  
      timeout_counter++;
   }
   #ifdef DEBUG
      fprintf(DEBUG_PORT,"flash 32kByte subsector erase complete\r\n");
   #endif
   return;
}

void subsector_4kByte_erase(Flash flash_stream, unsigned int32 subsector_address){
   if(flash_stream.flash_model == MT25QL128ABA){
      int8 write_enable_cmd = CMD_WRITE_ENABLE;
      unsigned int8 flash_cmd[4];
      
      flash_cmd[0] = CMD_SUBSECTOR_4KB_ERASE;
      flash_cmd[1] = (unsigned int8)((subsector_address>>16) & 0xff);   // 0x 00 _ _ 00 00
      flash_cmd[2] = (unsigned int8)((subsector_address>>8) & 0xff);    // 0x 00 00 _ _ 00
      flash_cmd[3] = (unsigned int8)((subsector_address) & 0xff);       // 0x 00 00 00 _ _
            
      //Write enable sequence
      output_low(flash_stream.cs_pin);
      spi_xfer_select_stream(flash_stream, &write_enable_cmd, 1);
      output_high(flash_stream.cs_pin);
      //Write enabled
      
      output_low(flash_stream.cs_pin);
      ///////////////////////////////////////////////////////////////////
      spi_xfer_select_stream(flash_stream, flash_cmd, 4);
      //////////////////////////////////////////////////////////////////
      output_high(flash_stream.cs_pin);            //take CS PIN higher back
   }
   else if(flash_stream.flash_model == MT25QL01GBBB){
      int8 write_enable_cmd = CMD_WRITE_ENABLE;
      unsigned int8 flash_cmd[5];
      
      flash_cmd[0] = CMD_4BYTE_SUBSECTOR_4KB_ERASE;
      flash_cmd[1] = (unsigned int8)((subsector_address>>24) & 0xff);   // 0x _ _ 00 00 00
      flash_cmd[2] = (unsigned int8)((subsector_address>>16) & 0xff);   // 0x 00 _ _ 00 00
      flash_cmd[3] = (unsigned int8)((subsector_address>>8) & 0xff);    // 0x 00 00 _ _ 00
      flash_cmd[4] = (unsigned int8)((subsector_address) & 0xff);       // 0x 00 00 00 _ _
      
      //Write enable sequence
      output_low(flash_stream.cs_pin);
      spi_xfer_select_stream(flash_stream, &write_enable_cmd, 1);
      output_high(flash_stream.cs_pin);
      //Write enabled
      
      output_low(flash_stream.cs_pin);
      ///////////////////////////////////////////////////////////////////
      spi_xfer_select_stream(flash_stream, flash_cmd, 5);
      //////////////////////////////////////////////////////////////////
      output_high(flash_stream.cs_pin);            //take CS PIN higher back
   }
   
   else{
      #ifdef DEBUG
         fprintf(DEBUG_PORT,"error:flash model is invalid\r\n");
      #endif
   }
   
   //wait process finished
   unsigned int8 timeout_counter = 0;
   while((status_register(flash_stream) & 0x01) == 1){                           //masking status bit
      if(timeout_counter > 10)
         delay_ms(200);
      else
         delay_ms(10);   
      
      if(timeout_counter > 100){
         #ifdef DEBUG
            fprintf(DEBUG_PORT,"flash timeout\r\n");
         #endif
         break;
      }  
      timeout_counter++;
   }
   #ifdef DEBUG
      fprintf(DEBUG_PORT,"flash 4kByte subsector erase complete\r\n");
   #endif
   return;
}
   
int8 read_data_byte(Flash flash_stream, unsigned int32 read_address){
   int8 read_data;
   if(flash_stream.flash_model == MT25QL128ABA){
      unsigned int8 flash_cmd[4];
      
      flash_cmd[0] = CMD_READ;
      flash_cmd[1] = (unsigned int8)((read_address>>16) & 0xff);   // 0x 00 _ _ 00 00
      flash_cmd[2] = (unsigned int8)((read_address>>8) & 0xff);    // 0x 00 00 _ _ 00
      flash_cmd[3] = (unsigned int8)((read_address) & 0xff);       // 0x 00 00 00 _ _
            
      output_low(flash_stream.cs_pin);
      ///////////////////////////////////////////////////////////////////
      spi_xfer_and_read_select_stream(flash_stream, flash_cmd, 4, &read_data, 1);
      //////////////////////////////////////////////////////////////////
      delay_us(2);
      output_high(flash_stream.cs_pin);            //take CS PIN higher back
   }
   else if(flash_stream.flash_model == MT25QL01GBBB){
      unsigned int8 flash_cmd[5];
      
      flash_cmd[0] = CMD_4BYTE_READ;
      flash_cmd[1] = (unsigned int8)((read_address>>24) & 0xff);   // 0x _ _ 00 00 00
      flash_cmd[2] = (unsigned int8)((read_address>>16) & 0xff);   // 0x 00 _ _ 00 00
      flash_cmd[3] = (unsigned int8)((read_address>>8) & 0xff);    // 0x 00 00 _ _ 00
      flash_cmd[4] = (unsigned int8)((read_address) & 0xff);       // 0x 00 00 00 _ _
   
      output_low(flash_stream.cs_pin);
      ///////////////////////////////////////////////////////////////////
      spi_xfer_and_read_select_stream(flash_stream, flash_cmd, 5, &read_data, 1);
      //////////////////////////////////////////////////////////////////
      delay_us(2);
      output_high(flash_stream.cs_pin);            //take CS PIN higher back
   }
   return read_data;
}
   
void read_data_bytes(Flash flash_stream, unsigned int32 read_start_address, int8 *read_data, unsigned int32 read_amount)
{
   if(flash_stream.flash_model == MT25QL128ABA){
      unsigned int8 flash_cmd[4];
      
      flash_cmd[0] = CMD_READ;
      flash_cmd[1] = (unsigned int8)((read_start_address>>16) & 0xff);   // 0x 00 _ _ 00 00
      flash_cmd[2] = (unsigned int8)((read_start_address>>8) & 0xff);    // 0x 00 00 _ _ 00
      flash_cmd[3] = (unsigned int8)((read_start_address) & 0xff);       // 0x 00 00 00 _ _
            
      output_low(flash_stream.cs_pin);
      ///////////////////////////////////////////////////////////////////
      spi_xfer_and_read_select_stream(flash_stream, flash_cmd, 4, read_data, read_amount);
      //////////////////////////////////////////////////////////////////
      delay_us(2);
      output_high(flash_stream.cs_pin);            //take CS PIN higher back
   }
   else if(flash_stream.flash_model == MT25QL01GBBB){
      unsigned int8 flash_cmd[5];
      
      flash_cmd[0] = CMD_4BYTE_READ;
      flash_cmd[1] = (unsigned int8)((read_start_address>>24) & 0xff);   // 0x _ _ 00 00 00
      flash_cmd[2] = (unsigned int8)((read_start_address>>16) & 0xff);   // 0x 00 _ _ 00 00
      flash_cmd[3] = (unsigned int8)((read_start_address>>8) & 0xff);    // 0x 00 00 _ _ 00
      flash_cmd[4] = (unsigned int8)((read_start_address) & 0xff);       // 0x 00 00 00 _ _
   
      output_low(flash_stream.cs_pin);
      ///////////////////////////////////////////////////////////////////
      spi_xfer_and_read_select_stream(flash_stream, flash_cmd, 5, read_data, read_amount);
      //////////////////////////////////////////////////////////////////
      delay_us(2);
      output_high(flash_stream.cs_pin);            //take CS PIN higher back
   }
   return;
}
   
void write_data_byte(Flash flash_stream, unsigned int32 write_address,int8 write_data)
{
   if(flash_stream.flash_model == MT25QL128ABA){
      int8 write_enable_cmd = CMD_WRITE_ENABLE;
      unsigned int8 flash_cmd[5];
      
      flash_cmd[0] = CMD_PAGE_PROGRAM;
      flash_cmd[1] = (unsigned int8)((write_address>>16) & 0xff);   // 0x 00 _ _ 00 00
      flash_cmd[2] = (unsigned int8)((write_address>>8) & 0xff);    // 0x 00 00 _ _ 00
      flash_cmd[3] = (unsigned int8)((write_address) & 0xff);       // 0x 00 00 00 _ _
      flash_cmd[4] = write_data;

      //Write enable sequence
      output_low(flash_stream.cs_pin);
      spi_xfer_select_stream(flash_stream, &write_enable_cmd, 1);
      output_high(flash_stream.cs_pin);
      //Write enabled
      
      output_low(flash_stream.cs_pin);
      ///////////////////////////////////////////////////////////////////
      spi_xfer_select_stream(flash_stream, flash_cmd, 5);
      //////////////////////////////////////////////////////////////////
      delay_us(2);
      output_high(flash_stream.cs_pin);            //take CS PIN higher back
   }
   else if(flash_stream.flash_model == MT25QL01GBBB){
      int8 write_enable_cmd = CMD_WRITE_ENABLE;
      unsigned int8 flash_cmd[6];
      
      flash_cmd[0] = CMD_4BYTE_PAGE_PROGRAM;
      flash_cmd[1] = (unsigned int8)((write_address>>24) & 0xff);   // 0x _ _ 00 00 00
      flash_cmd[2] = (unsigned int8)((write_address>>16) & 0xff);   // 0x 00 _ _ 00 00
      flash_cmd[3] = (unsigned int8)((write_address>>8) & 0xff);    // 0x 00 00 _ _ 00
      flash_cmd[4] = (unsigned int8)((write_address) & 0xff);       // 0x 00 00 00 _ _
      flash_cmd[5] = write_data;
   
      //Write enable sequence
      output_low(flash_stream.cs_pin);
      spi_xfer_select_stream(flash_stream, &write_enable_cmd, 1);
      output_high(flash_stream.cs_pin);
      //Write enabled
      output_low(flash_stream.cs_pin);
      ///////////////////////////////////////////////////////////////////
      spi_xfer_select_stream(flash_stream, flash_cmd, 6);
      //////////////////////////////////////////////////////////////////
      delay_us(2);
      output_high(flash_stream.cs_pin);            //take CS PIN higher back
   }
   
   //wait process finished
   unsigned int8 timeout_counter = 0;
   while((status_register(flash_stream) & 0x01) == 1){                           //masking status bit
      if(timeout_counter > 10)
         delay_ms(200);
      else
         delay_ms(10);   
      
      if(timeout_counter > 100){
         #ifdef DEBUG
            fprintf(DEBUG_PORT,"flash timeout\r\n");
         #endif
         break;
      }  
      timeout_counter++;
   }
   #ifdef DEBUG
      fprintf(DEBUG_PORT,"flash write complete\r\n");
   #endif
   return;
}

void write_data_bytes(Flash flash_stream, unsigned int32 write_start_address, int8 *write_data, unsigned int16 write_amount){
   if(flash_stream.flash_model == MT25QL128ABA){
      int8 write_enable_cmd = CMD_WRITE_ENABLE;
      unsigned int8 flash_cmd[4];
      
      flash_cmd[0] = CMD_PAGE_PROGRAM;
      flash_cmd[1] = (unsigned int8)((write_start_address>>16) & 0xff);   // 0x 00 _ _ 00 00
      flash_cmd[2] = (unsigned int8)((write_start_address>>8) & 0xff);    // 0x 00 00 _ _ 00
      flash_cmd[3] = (unsigned int8)((write_start_address) & 0xff);       // 0x 00 00 00 _ _

      //Write enable sequence
      output_low(flash_stream.cs_pin);
      spi_xfer_select_stream(flash_stream, &write_enable_cmd, 1);
      output_high(flash_stream.cs_pin);
      //Write enabled
      
      output_low(flash_stream.cs_pin);
      ///////////////////////////////////////////////////////////////////
      spi_xfer_two_datas_select_stream(flash_stream, flash_cmd, 4, write_data, write_amount);
      //////////////////////////////////////////////////////////////////
      delay_us(2);
      output_high(flash_stream.cs_pin);            //take CS PIN higher back
   }
   else if(flash_stream.flash_model == MT25QL01GBBB){
      int8 write_enable_cmd = CMD_WRITE_ENABLE;
      unsigned int8 flash_cmd[5];
      
      flash_cmd[0] = CMD_4BYTE_PAGE_PROGRAM;
      flash_cmd[1] = (unsigned int8)((write_start_address>>24) & 0xff);   // 0x _ _ 00 00 00
      flash_cmd[2] = (unsigned int8)((write_start_address>>16) & 0xff);   // 0x 00 _ _ 00 00
      flash_cmd[3] = (unsigned int8)((write_start_address>>8) & 0xff);    // 0x 00 00 _ _ 00
      flash_cmd[4] = (unsigned int8)((write_start_address) & 0xff);       // 0x 00 00 00 _ _
   
      //Write enable sequence
      output_low(flash_stream.cs_pin);
      spi_xfer_select_stream(flash_stream, &write_enable_cmd, 1);
      output_high(flash_stream.cs_pin);
      //Write enabled
      output_low(flash_stream.cs_pin);
      ///////////////////////////////////////////////////////////////////
      spi_xfer_two_datas_select_stream(flash_stream, flash_cmd, 5, write_data, write_amount);
      //////////////////////////////////////////////////////////////////
      delay_us(2);
      output_high(flash_stream.cs_pin);            //take CS PIN higher back
   }
   
   //wait process finished
   unsigned int8 timeout_counter = 0;
   while((status_register(flash_stream) & 0x01) == 1){                           //masking status bit
      if(timeout_counter > 10)
         delay_ms(200);
      else
         delay_ms(10);   
      
      if(timeout_counter > 100){
         #ifdef DEBUG
            fprintf(DEBUG_PORT,"flash timeout\r\n");
         #endif
         break;
      }  
      timeout_counter++;
   }
   #ifdef DEBUG
      fprintf(DEBUG_PORT,"flash write complete\r\n");
   #endif
   return;
}
