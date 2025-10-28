# ライブラリ仕様書 `communication`


## 1. 概要
このライブラリは、機器間の通信機能ライブラリ`communication`を用いて、更に拡張的な、MIS MCUのみが使用する独自の機器通信の機能を提供する


## 2. 関数一覧
「<span style="color:red;">+</span>」: 外部ファイルからアクセス可能な関数<br>
「<span style="color:blue;">-</span>」: 内部専用関数

1. <span style="color:red;">+</span>Command make_receive_command(unsigned int8 receive_signal[], int8 receive_signal_size);
2. <span style="color:red;">+</span>void transmit_ack();
3. <span style="color:red;">+</span>void transmit_status();
4. <span style="color:red;">+</span>void check_and_respond_to_boss();
5. <span style="color:red;">+</span>int1 req_use_smf();



## 3. 関数詳細
### 1. trasnsmit_ack

**概要**  
BOSS PICにACKフレームを送信する

**引数**
無し

**戻り値**  
`void`

**副作用**  
- BOSS PICとの通信

---

### 2. trasnsmit_status

**概要**  
BOSS PICにmis_mcu_statusフレームを送信する

**引数**
無し

**戻り値**  
`void`

**副作用**  
- BOSS PICとの通信