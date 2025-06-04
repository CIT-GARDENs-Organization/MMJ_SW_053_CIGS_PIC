ExecutedMission get_executed_mission()
{
   static ExecutedMission executed_mission = {0};
   return executed_mission;
}

int1 is_executed_mission(unsigned int8 command_id)
{
   ExecutedMission executed_mission = get_executed_mission();
   
   for (int i = 0; i < executed_mission.executed_mission_count; i++)
      if (command_id == executed_mission.executed_mission[i])
         return TRUE;
      
   return FALSE;
}

void add_executed_mission(unsigned int8 command_id)
{
   ExecutedMission executed_mission = get_executed_mission();
   
   if (executed_mission.executed_mission_count != EXECUTED_MISSION_SIZE)
      executed_mission.executed_mission[executed_mission.executed_mission_count++] = command_id;
   else
      fprintf(PC, "!!!overflow executed_mission list\r\n");
}


int1 req_use_smf()
{
   fprintf(PC, "Start SMF using reqest seaquence\r\n");
   status = SMF_USE_REQ;
   is_use_smf_req_in_mission = TRUE;
   
   while (TRUE)
   {
      for (int16 i = 0; i < 1200; i++) // 10 min
      {
         if (boss_receive_buffer_size > 0)
         {
            Command command = make_receive_command(boss_receive_buffer, boss_receive_buffer_size);
            clear_receive_signal(boss_receive_buffer, &boss_receive_buffer_size);
            if (command.frame_id == STATUS_CHECK)
            {
               transmit_status();
               break;
            }
            else
            {
               fprintf(PC, "Error! Receiving command inconsistent with the design\r\n");
            }
         }
         delay_ms(500);
      }
      
      for (int16 i = 0; i < 1200; i++) // 10 min
      {
         if (boss_receive_buffer_size > 0)
         {
            Command command = make_receive_command(boss_receive_buffer, boss_receive_buffer_size); 
            clear_receive_signal(boss_receive_buffer, &boss_receive_buffer_size);
            if (command.frame_id == IS_SMF_AVAILABLE)
            {
               if (command.content[0] == ALLOW)
               {
                  fprintf(PC, "SMF use request allowed\r\n");
                  transmit_ack();
                  goto NEXT;
               }
               else
               {
                  fprintf(PC, "SMF use request denyed\r\n");
                  fprintf(PC, "Retry request to BOSS PIC\r\n");
                  transmit_ack();
                  break;
               }
            }
            else
            {
               fprintf(PC, "Error! Receiving command inconsistent with the design\r\n");
            }
         }
         delay_ms(500);
      }
   }
   
NEXT:
   is_use_smf_req_in_mission = FALSE;
   status = COPYING;
   return TRUE;
   fprintf(PC, "End SMF using reqest seaquence\r\n");
}


void finished_use_smf()
{
   status = EXECUTING_MISSION;
}

void executed_mission_pop(void)
{
   ExecutedMission executed_mission = get_executed_mission();
   executed_mission.executed_mission[executed_mission.executed_mission_count--] = 0x00;
}

void continue_mis_mcu(int16 duration_second)
{  
   duration_sec = duration_second + sec;
}

void check_and_respond_to_boss()
{
   if (kbhit())
   {
      fgetc(BOSS);
      transmit_status();
   }
}
