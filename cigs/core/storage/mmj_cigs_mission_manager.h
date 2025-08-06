#ifndef MMJ_CIGS_MISSION_MANAGER_H
#define MMJ_CIGS_MISSION_MANAGER_H

// ミッションID定義
#define ID_CIGS_MEASURE_DATA 0x01
#define ID_CIGS_PICLOG       0x02

// ミッション管理用の統合構造体
typedef struct {
   int8 mission_id;                    // ミッションID (0x01: CIGS_MEASURE, 0x02: CIGS_PICLOG)
   int32 start_address;                // 開始アドレス
   int32 end_address;                  // 終了アドレス
   int32 used_size;                    // 使用済みサイズ
   int32 loop_counter;                 // ループカウンター
   int32 uncopyed_counter;             // 未コピーデータカウンター
   int32 write_counter;                // 書き込みカウンター（PICLOGのみ使用）
} CigsMissionStruct;

// ミッション管理構造体のグローバルインスタンス
extern CigsMissionStruct cigs_measure_mission;
extern CigsMissionStruct cigs_piclog_mission;

// ミッション管理関数
CigsMissionStruct* get_cigs_mission_by_id(int8 mission_id);
void update_cigs_uncopyed_counter(int8 mission_id, int32 size_change);
void reset_cigs_uncopyed_counter(int8 mission_id);
void init_cigs_mission_structs(void);
CigsMissionStruct get_cigs_mission_struct(int8 mission_id);

// ミッションID定義
#define ID_CIGS_MEASURE_DATA 0x01
#define ID_CIGS_PICLOG       0x02

#endif // MMJ_CIGS_MISSION_MANAGER_H
