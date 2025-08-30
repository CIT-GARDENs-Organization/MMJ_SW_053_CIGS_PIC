#ifndef SMF_QUEUE_H
#define SMF_QUEUE_H

typedef enum {
   ENUM_SMF_WRITE,    // SMF書き込み操作
   ENUM_SMF_READ,     // SMF読み込み操作
   ENUM_SMF_ERASE     // SMF消去操作
} FunctionType;

// SMF書き込みモード
typedef enum {
   SMF_WRITE_OVERWRITE = 0x00,  // 上書きモード
   SMF_WRITE_APPEND = 0x01,     // 追記モード
   SMF_WRITE_CIRCULAR = 0x02    // 循環書き込みモード（endまで行ったらstartに戻る）
} SmfWriteMode;

// データソースタイプ
typedef enum {
   SOURCE_MISF_UNCOPIED = 0x00, // MISF未コピーデータを転送
   SOURCE_MISF_MANUAL = 0x01    // MISF指定アドレス・サイズを転送
} DataSourceType;


typedef enum{
   CIGS_DATA_TABLE,
   CIGS_PICLOG_DATA,
   CIGS_ENVIRO_DATA,
   CIGS_IV1_HEADER,
   CIGS_IV1_DATA,
   CIGS_IV2_HEADER,
   CIGS_IV2_DATA
} MissionID;



typedef struct {
   int32 start_address;
   int32 end_address;
} SmfAddressStruct;

typedef struct{
   int32 misf_start_address;
   int32 misf_size;
} SmfWriteStruct;

typedef struct {
    MissionID mission_id;                // ミッションID
    FunctionType func_type;         // 操作タイプ
    SmfWriteMode write_mode;        // 書き込みモード（書き込み時のみ）
    DataSourceType source_type;     // データソースタイプ
    int32 misf_start_addr;          // MISF手動指定時の開始アドレス
    int32 misf_size;                // MISF手動指定時のサイズ
} FlashOperationStruct;

#define SMF_QUEUE_SIZE 16
typedef struct {
   FlashOperationStruct entries[SMF_QUEUE_SIZE];  // キューエントリー配列
   int8 head_index;                               // 取り出し位置（読み取りインデックス）
   int8 tail_index;                               // 追加位置（書き込みインデックス）
} FlashQueueStruct;



// _____________________ values _________________

volatile FlashQueueStruct flash_queue = {0};

// MISF/SMF統合管理インスタンス（ミッション別）
//volatile MisfSmfManagerStruct misf_smf_manager[8] = {0}; // 最大8ミッション対応


// ___________________ functions ________________

// MISF/SMF管理関数
void init_misf_smf_manager(int8 mission_id, int32 misf_start, int32 misf_size, int32 smf_start, int32 smf_size);
void update_misf_data(int8 mission_id, int32 data_size);
void process_misf_to_smf_transfer(int8 mission_id);
int32 get_uncopied_data_size(int8 mission_id);
void mark_data_copied(int8 mission_id, int32 copied_size);
int1 is_misf_full(int8 mission_id);
//MisfSmfManagerStruct* get_misf_smf_manager(int8 mission_id);

void enqueue_flash_operation(FlashOperationStruct *data);

FlashOperationStruct *dequeue_flash_operation();

FlashOperationStruct *peek_flash_operation(void);

void remove_flash_operation(void);

int1 is_empty_flash_queue(void);

SmfAddressStruct get_smf_address_struct(MissionID mission_id);

// 便利な関数
void enqueue_uncopied_data(int8 mission_id, SmfWriteMode write_mode);
void enqueue_manual_data(int8 mission_id, int32 misf_addr, int32 size, SmfWriteMode write_mode);
void enqueue_read_data(int32 smf_addr, int32 size);
void enqueue_erase_data(int32 smf_addr, int32 size);

// 統合管理対応の便利な関数
void enqueue_auto_transfer(int8 mission_id);  // 未コピーデータの自動転送
void complete_transfer_and_update_counter(int8 mission_id, int32 transferred_size);  // 転送完了とカウンタ更新

SmfWriteStruct get_smf_write_struct(MissionID mission_id);
#endif   //SMF_QUEUE_H
