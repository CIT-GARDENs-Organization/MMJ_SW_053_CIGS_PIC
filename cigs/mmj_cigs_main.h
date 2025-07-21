#ifndef MMJ_CIGS_MAIN_H
#define MMJ_CIGS_MAIN_H

#opt 0 // 0 = no optimization

//==============================================================================
// CCS C 最適化構造: ヘッダー分散 + .cファイル統合
//==============================================================================

// レベル1: ハードウェア抽象化層
#include "mmj_cigs_config.h"

// レベル2: 基本ライブラリヘッダー（型定義・通信・ツール）

#include "../lib/communication/typedef_content.h"
#include "../lib/communication/value_status.h"
#include "../lib/tool/timer.h"
#include "../lib/communication/uart.h"
#include "../lib/tool/smf_function.h"

// レベル3: デバイスドライバヘッダー（必要に応じて）
#include "../lib/device/mt25q.h"
#include "../lib/device/ad7490.h"
#include "../lib/device/mcp4901.h"

// レベル4: 通信ライブラリヘッダー
#include "../lib/communication/communication.h"
#include "../lib/communication/communication_driver.h"
#include "../lib/communication/mission_tools.h"
#include "../lib/tool/calc_tools.h"
#include "../lib/tool/smf_queue.h"

// レベル5: アプリケーションヘッダー
#include "mmj_cigs_device.h"
#include "mmj_cigs_excute_mission.h"
#include "mmj_cigs_piclog.h"
#include "mmj_cigs_func.h"
#include "mmj_cigs_flash.h"
#include "mmj_cigs_mode_mission.h"
#include "mmj_cigs_mode_flash.h"


//==============================================================================
// .cファイル統合（CCS C単一コンパイル単位）
//==============================================================================

// ライブラリ実装ファイル
#include "../lib/device/ad7490.c"
#include "../lib/device/mcp4901.c"
#include "../lib/device/mt25q.c"
#include "../lib/tool/timer.c"
#include "../lib/tool/calc_tools.c"
#include "../lib/tool/smf_function.c"
#include "../lib/tool/smf_queue.c"
#include "../lib/communication/uart.c"
#include "../lib/communication/communication.c"
#include "../lib/communication/communication_driver.c"
#include "../lib/communication/mission_tools.c"

// アプリケーション実装ファイル
#include "src/mmj_cigs_flash.c"
#include "src/mmj_cigs_device.c"
#include "src/mmj_cigs_func.c"
#include "src/mmj_cigs_piclog.c"
#include "src/mmj_cigs_mode_flash.c"
#include "src/mmj_cigs_mode_mission.c"
#include "src/mmj_cigs_excute_mission.c"

#endif // MMJ_CIGS_MAIN_H
//------------------End of File------------------
