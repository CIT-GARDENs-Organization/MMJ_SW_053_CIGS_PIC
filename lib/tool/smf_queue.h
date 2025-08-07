#ifndef SMF_QUEUE_H
#define SMF_QUEUE_H

typedef enum {
   SMF_WRITE = 0x00,    // SMF書き込み操作
   SMF_READ = 0x01,     // SMF読み込み操作
   SMF_ERASE = 0x02     // SMF消去操作
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

// MISF/SMF統合管理構造体
typedef struct {
   // MISF情報
   int32 misf_start_addr;          // MISF開始アドレス
   int32 misf_end_addr;            // MISF終了アドレス
   int32 misf_current_write_addr;  // MISF現在の書き込み位置
   int32 misf_uncopied_count;      // MISF未コピーデータ数
   int32 misf_total_size;          // MISF総サイズ
   
   // SMF情報
   int32 smf_start_addr;           // SMF開始アドレス
   int32 smf_end_addr;             // SMF終了アドレス
   int32 smf_current_write_addr;   // SMF現在の書き込み位置
   int32 smf_copied_count;         // SMFコピー済みデータ数
   int32 smf_total_size;           // SMF総サイズ
   
   // 管理情報
   int8 mission_id;                // ミッションID
   int1 is_full;                   // MISFフル状態フラグ
   int1 transfer_in_progress;      // 転送進行中フラグ
} MisfSmfManagerStruct;

/*
 * 使用例:
 * 
 * 1. ミッション初期化:
 * init_misf_smf_manager(0x01, 0x160000, 0x10000, 0x200000, 0x20000);
 * 
 * 2. MISFにデータ追加時:
 * update_misf_data(0x01, 256);  // 256バイトのデータを追加
 * 
 * 3. 未コピーデータの自動転送:
 * enqueue_auto_transfer(0x01);  // 未コピーデータを自動的にSMFに転送キュー
 * 
 * 4. 転送完了時:
 * complete_transfer_and_update_counter(0x01, 256);  // 256バイト転送完了、カウンタ更新
 * 
 * 5. 従来の使用例（下位互換）:
 * FlashOperationStruct uncopied_data = {
 *     .mission_id = 0x01,
 *     .func_type = SMF_WRITE,
 *     .write_mode = SMF_WRITE_CIRCULAR,
 *     .source_type = SOURCE_MISF_UNCOPIED,
 *     .manager = get_misf_smf_manager(0x01)
 * };
 * 
 * 6. 指定アドレスの上書き:
 * FlashOperationStruct manual_data = {
 *     .mission_id = 0x01,
 *     .func_type = SMF_WRITE,
 *     .write_mode = SMF_WRITE_OVERWRITE,
 *     .source_type = SOURCE_MISF_MANUAL,
 *     .misf_start_addr = 0x160000,
 *     .misf_size = 1024,
 *     .manager = get_misf_smf_manager(0x01)
 * };
 */

typedef struct {
   int32 start_address;
   int32 end_address;
} SmfMissionStruct;

typedef struct {
    int8 mission_id;                // ミッションID
    int32 src;                      // ソースアドレス（MISFアドレス）
    int32 size;                     // データサイズ
    FunctionType func_type;         // 操作タイプ
    SmfWriteMode write_mode;        // 書き込みモード（書き込み時のみ）
    DataSourceType source_type;     // データソースタイプ
    int32 misf_start_addr;          // MISF手動指定時の開始アドレス
    int32 misf_size;                // MISF手動指定時のサイズ
    MisfSmfManagerStruct *manager;  // MISF/SMF管理構造体へのポインタ
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
volatile MisfSmfManagerStruct misf_smf_manager[8] = {0}; // 最大8ミッション対応


// ___________________ functions ________________

// MISF/SMF管理関数
void init_misf_smf_manager(int8 mission_id, int32 misf_start, int32 misf_size, int32 smf_start, int32 smf_size);
void update_misf_data(int8 mission_id, int32 data_size);
void process_misf_to_smf_transfer(int8 mission_id);
int32 get_uncopied_data_size(int8 mission_id);
void mark_data_copied(int8 mission_id, int32 copied_size);
int1 is_misf_full(int8 mission_id);
MisfSmfManagerStruct* get_misf_smf_manager(int8 mission_id);

void enqueue_flash_operation(FlashOperationStruct *data);

FlashOperationStruct *dequeue_flash_operation();

FlashOperationStruct *peek_flash_operation(void);

void remove_flash_operation(void);

int1 is_empty_flash_queue(void);

SmfMissionStruct get_smf_mission_struct(FunctionType func_type);

// 便利な関数
void enqueue_uncopied_data(int8 mission_id, SmfWriteMode write_mode);
void enqueue_manual_data(int8 mission_id, int32 misf_addr, int32 size, SmfWriteMode write_mode);
void enqueue_read_data(int32 smf_addr, int32 size);
void enqueue_erase_data(int32 smf_addr, int32 size);

// 統合管理対応の便利な関数
void enqueue_auto_transfer(int8 mission_id);  // 未コピーデータの自動転送
void complete_transfer_and_update_counter(int8 mission_id, int32 transferred_size);  // 転送完了とカウンタ更新

#endif

