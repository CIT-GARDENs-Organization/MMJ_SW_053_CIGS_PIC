#ifndef MMJ_CIGS_MAIN_H
#define MMJ_CIGS_MAIN_H

#opt 0 // 0 = no optimization

//==============================================================================
// CCS C 最適化構造: ヘッダー分散 + .cファイル統合
//==============================================================================

// レベル1: システム設定
#include "mmj_cigs_config.h"

// レベル2: ハードウェア抽象化層  
#include "../hardware/mcu/timer.h"
#include "../hardware/mcu/uart.h"
#include "../hardware/devices/ad7490_driver.h"
#include "../hardware/devices/mcp4901_driver.h"

// レベル3: 基本ライブラリヘッダー（型定義・通信・ツール）
#include "../../lib/communication/typedef_content.h"
#include "../../lib/communication/value_status.h"
#include "../../lib/tool/smf_function.h"
#include "../../lib/tool/calc_tools.h"
#include "../../lib/tool/smf_queue.h"

// レベル4: デバイスドライバヘッダー
#include "../../lib/device/mt25q.h"
#include "../../lib/device/ad7490.h"
#include "../../lib/device/mcp4901.h"

// レベル5: 通信ライブラリヘッダー
#include "../../lib/communication/communication.h"
#include "../../lib/communication/communication_driver.h"
#include "../../lib/communication/mission_tools.h"

// レベル6: コア機能ヘッダー
#include "../core/measurement/mmj_cigs_func.h"
#include "../core/storage/mmj_cigs_flash.h"
#include "../core/logging/mmj_cigs_piclog.h"

// レベル7: アプリケーションヘッダー
#include "../application/mission/mmj_cigs_excute_mission.h"
#include "../application/mission/mmj_cigs_mode_mission.h"
#include "../application/mission/mmj_cigs_mode_flash.h"


//==============================================================================
// .cファイル統合（CCS C単一コンパイル単位）
//==============================================================================

// ハードウェア層実装ファイル
#include "../hardware/mcu/src/timer.c"
#include "../hardware/mcu/src/uart.c"
#include "../hardware/devices/src/ad7490_driver.c"
#include "../hardware/devices/src/mcp4901_driver.c"

// ライブラリ実装ファイル
#include "../../lib/device/ad7490.c"
#include "../../lib/device/mcp4901.c"
#include "../../lib/device/mt25q.c"
#include "../../lib/tool/calc_tools.c"
#include "../../lib/tool/smf_function.c"
#include "../../lib/tool/smf_queue.c"
#include "../../lib/communication/communication.c"
#include "../../lib/communication/communication_driver.c"
#include "../../lib/communication/mission_tools.c"

// コア機能実装ファイル
#include "../core/measurement/mmj_cigs_func.c"
#include "../core/storage/mmj_cigs_flash.c"
#include "../core/logging/mmj_cigs_piclog.c"

// アプリケーション実装ファイル
#include "../application/mission/mmj_cigs_excute_mission.c"
#include "../application/mission/mmj_cigs_mode_mission.c"
#include "../application/mission/mmj_cigs_mode_flash.c"

#endif // MMJ_CIGS_MAIN_H
//------------------End of File------------------
