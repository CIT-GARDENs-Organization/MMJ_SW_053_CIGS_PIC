# ミッションの開発方法


## 手順
1. 初期設定
  - `config.h`
    - **`#pin_select`** → それぞれ適切なものに変更する
    - **`#use`** → それぞれ適切なものに変更する
    - **`define`** → SPIのCSピンを適切なものに変更する
  
  - `communication.h`
    - **19行目** `#SELF_DEVICE_ID` を、6 ~ 17行目の定数の中から適切な機器名に選択する。

  - `smf_queue.h` 
    - **7行目** → `APRS_DATA`のように、データ種を記入する([MemoryMap](https://github.com/CIT-GARDENs-Organization/MIS_MCU_CCSC/blob/main/docs/memory_map.png)を参照すること)
  - `smf_queue.c`
    - **39行目** → 先に記入したデータ種ごとに、SMF内のそのデータの開始アドレス、終了アドレス、mission_flagを記入する。([MemoryMap](https://github.com/CIT-GARDENs-Organization/MIS_MCU_CCSC/blob/main/docs/memory_map.png)を参照すること)


2. ミッションの実装
  - `missoin.h` **20行目以降** → 実装するミッションの関数名を記入する。返り値は`void`、引数は`(unsigned int8 parameter[])`である。
  - `mission.c` **40行目** → 先程の関数をswitch-caseの中に追記する。
  - `mission.c` **97行目** → 先程の関数の実装をする。

## シミュレーター
- 本ソフトは以下のソフトと併用することで実際のBOSS PICとの通信からミッション実行、終了までの動作をシミュレーションできる
- 本ソフトは擬似的にすでに3つのミッションが実装されているため実際に実行して動作理解を深めてから実装に入るとよい

[BOSS PIC simulater](https://github.com/CIT-GARDENs-Organization/BOSS_PIC_simulator)

| CMD ID     | 使用パラメーター         | 説明                                         |
|:-----------|:------------            |:------------                                |
| 00         | なし | 5秒待機し、SMFへコピーするデータを生成する                                 |
| 01         | なし | 5秒待機する                   |
| 02         | なし | 最初にSMFへアクセスし、10秒秒待機する  |


---

## 制約 & その他  

### **⚠️必ず守らなければならないこと**
- ミッション実行中において、最低5秒に1回は`void check_and_respond_to_boss()`という**関数を呼ぶこと**
  - ミッション実行中、BOSS PICからstatus checkフレームが来たかどうかの判定とそれに対する返信をこの関数で行う。
  - できれば1秒、2秒以内が好ましい

### **⭕️許可されていること**
- 新規ファイル作成
- 新規関数作成

## **mission.c内で使用可能な機能一覧**
### 一覧
- `mission_tools.h`
  - `void check_and_respond_to_boss()` (**重要**)
  - `int1 req_use_smf()`
  - `void finished_use_smf()`

- `smf_queue.h`
  - `void enqueue_smf_data(SmfDataStruct *data)`

- `flash.h`
  - (`Flash`構造体の引数は`mis_fm`か`smf`を指定する)
  - `void flash_setting(Flash flash_stream);`
  - `int8 status_register(Flash flash_stream);`
  - `int8 read_id(Flash flash_stream);`
  - `void sector_erase(Flash flash_stream, unsigned int32 sector_address);`
  - `void subsector_32kByte_erase(Flash flash_stream, unsigned int32 subsector_address);`
  - `void subsector_4kByte_erase(Flash flash_stream, unsigned int32 subsector_address);`
  - `int8 read_data_byte(Flash flash_stream, unsigned int32 read_address);`
  - `void read_data_bytes(Flash flash_stream, unsigned int32 read_start_address, int8 *read_data, unsigned int32 read_amount);`    
  - `void write_data_byte(Flash flash_stream, unsigned int32 write_address,int8 write_data);`
  - `void write_data_bytes(Flash flash_stream, unsigned int32 write_start_address, int8 *write_data, unsigned int16 write_amount);`


### 説明
- BOSS PICとの通信
  - `void check_and_respond_to_boss()`を実行する
  - この関数は、BOSS PICからのコマンドの有無の判定と、受信していた場合の返信を行う
  - **最低でもミッション実行中に5秒以内毎に実行されなければならない**

- SMFへのデータコピー
  - `void enqueue_smf_data(SmfDataStruct *data)`を実行する
  - 例
  ```c
  SmfDataStruct data;
  data.mission_type = APRD_DATA; // コピーする目的のデータ種別
  data.src = 0x00000000; // コピー元のMIS_FMのアドレス
  data.size = 0x00000000; // コピーするデータのサイズ
  enqueue_smf_data(&data); // SMFへのデータコピーを実行する
  ```
- SMFの利用(読み取りのみなど)
  - `int1 req_use_smf()`を実行すること
    - この関数は、次回BOSS PICからstatus checkが来た時にSMFの使用要求を送り、使用の可否を返り値として返す。
    - 使用可能な場合、`TRUE`が返ってくる 
    - 使用不可の場合、`FALSE`が返ってくる
    - 現在は例外処理として10分間この関数が実行されると`FALSE`を返すようにしている。
    - 何度実行しても構わない
  - **SMFの使用終了後は必ず`void finished_use_smf()`を実行すること。**
---


## 今後の更新予定 (作担当者: GARDENs)
1. バス系からの衛星内時間配布機能
