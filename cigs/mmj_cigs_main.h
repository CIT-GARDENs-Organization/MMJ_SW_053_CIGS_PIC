#ifndef MMJ_CIGS_MAIN_H
#define MMJ_CIGS_MAIN_H


#define SELF_DEVICE_ID SATO_PIC


#opt 0 // 0 = no optimization

//==============================================================================
// CCS C 最適化構造: ヘッダー分散 + .cファイル統合
//==============================================================================

// レベル1: システム設定
#include "hal/cigs_config.h"

// レベル2: ハードウェア抽象化層  
#include "hal/timer.h"
#include "hal/uart.h"
#include "hal/gpio.h"
#include "device_driver/ad7490_driver.h"
#include "device_driver/mcp4901_driver.h"

// レベル3: 基本ライブラリヘッダー（型定義・通信・ツール）
#include "../lib/communication/typedef_content.h"
#include "../lib/communication/value_status.h"
#include "../lib/tool/calc_tools.h"
#include "../lib/tool/smf_queue.h"
#include "../lib/tool/mmj_smf_memorymap.h"

// レベル4: デバイスドライバヘッダー
#include "../lib/device/mt25q.h"
#include "../lib/device/ad7490.h"
#include "../lib/device/mcp4901.h"

// レベル5: 通信ライブラリヘッダー
#include "../lib/communication/communication.h"
#include "../lib/communication/communication_driver.h"
#include "../lib/communication/mission_tools.h"

// レベル6: コア機能ヘッダー
#include "domain/cigs_iv.h"
#include "domain/cigs_flash.h"
#include "domain/cigs_piclog.h"
#include "domain/cigs_smf.h"

// レベル7: アプリケーションヘッダー
#include "application/cigs_excute_mission.h"
#include "application/cigs_mode_mission.h"
#include "application/cigs_mode_flash.h"


//==============================================================================
// .cファイル統合（CCS C単一コンパイル単位）
//==============================================================================

// ハードウェア層実装ファイル
// 実装は各 .c からビルドするためヘッダーでの取り込みは不要。必要なら下記のように直下参照に修正。
#include "hal/timer.c"
#include "hal/uart.c"
#include "hal/gpio.c"
#include "device_driver/ad7490_driver.c"
#include "device_driver/mcp4901_driver.c"

// ライブラリ実装ファイル
#include "../lib/device/ad7490.c"
#include "../lib/device/mcp4901.c"
#include "../lib/device/mt25q.c"
#include "../lib/tool/calc_tools.c"
#include "../lib/tool/smf_queue.c"
#include "../lib/communication/communication.c"
#include "../lib/communication/communication_driver.c"
#include "../lib/communication/mission_tools.c"

// コア機能実装ファイル
#include "domain/cigs_iv.c"
#include "domain/cigs_piclog.c"
#include "domain/cigs_smf.c"
#include "domain/cigs_flash.c"

// アプリケーション実装ファイル
#include "application/cigs_excute_mission.c"
#include "application/cigs_mode_mission.c"
#include "application/cigs_mode_flash.c"

#endif // MMJ_CIGS_MAIN_H
//------------------End of File------------------
