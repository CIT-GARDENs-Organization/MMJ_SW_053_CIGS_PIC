# ライブラリ仕様書 `communication`

## 1. 概要
このライブラリは、衛星においてすべての機器に共通して使用される機器間の通信機能を提供する

## 2. 関数一覧
「<span style="color:red;">+</span>」: 外部ファイルからアクセス可能な関数<br>
「<span style="color:blue;">-</span>」: 内部専用関数
1. 受信系
   1. <span style="color:red;">+</span>Command make_receive_command(unsigned int8 receive_signal[], int8 receive_signal_size)
   2. <span style="color:red;">+</span>void clear_receive_signal(unsigned int8 receive_signal[], int8 *receive_signal_size);
   3. <span style="color:blue;">-</span>int8 trim_receive_signal_header(unsigned int8 receive_signal[], int8 receive_signal_size);
   4. <span style="color:blue;">-</span>int8 get_content_size(unsigned int8 frame_id);
   5. <span style="color:blue;">-</span>int1 check_crc(unsigned int8 frame[], int8 receive_frame_size);
   6. <span style="color:blue;">-</span>int1 check_device_id(unsigned int8 device_id);

2. 送信系
   1. <span style="color:red;">+</span>void transmit_command(TransmitFrameId frame_id, unsigned int8 content[], int8 content_size);
   2.  <span style="color:blue;">-</span>void transmit(unsigned int8 data[], int8 data_size);

3. 共通
   1. <span style="color:blue;">-</span>unsigned int8 calc_crc8(unsigned int8 frame[], int8 payload_size);


## 3. 関数詳細
### 1.1. make_receive_command

**概要**  
生の受信信号を意味的に分割しCommandを生成する

**引数**
| 引数名 | 型 | 説明 |
| - | - | - |
| `receive_signal` | `unsigned int8 *` | 解析する生の受信信号のポインタ |
| `receive_signal_size` | `int8` | 解析する生の受信信号の長さ |


**戻り値**  
`Command`

**副作用**  
無し

---

### 1.2. clear_receive_signal

**概要**  
一度のMIS MCUの起動中に同じミッションを複数回実行したい場合、この関数をそのミッション内で一度使用する。

**引数**
| 引数名 | 型 | 説明 |
| - | - | - |
| `receive_signal` | `unsigned int8 *` | 解析する生の受信信号のポインタ |
| `receive_signal_size` | `int8 *` | 解析する生の受信信号の長さのポインタ |

**戻り値**  
`void`

**副作用**  
- `receive_signal`の0埋め
- `receive_signal_size`のへ0の代入

---

### 1.3. trim_receive_signal_header

**概要**  
生の受信信号のSFD(0x00)以前のノイズを除去する

**引数**
| 引数名 | 型 | 説明 |
| - | - | - |
| `receive_signal` | `unsigned int8 *` | 解析する生の受信信号のポインタ |
| `receive_signal_size` | `int8` | 解析する生の受信信号の長さ |

**戻り値**  
`int1`

**副作用**  
- 他の機器との通信
- 変数`status`の操作

---

### 1.4. get_content_size

**概要**  
受信信号のフレームIDからconentの長さを取得する

**引数**
| 引数名 | 型 | 説明 |
| - | - | - |
| `frame_id` | `unsigned int8` | フレームID |

**戻り値**  
`int8`

**副作用**  
無し

---

### 1.5. check_crc

**概要**  
受信フレームのCRCが正しいかを判定する

**引数**
| 引数名 | 型 | 説明 |
| - | - | - |
| `receive_signal` | `unsigned int8 *` | 解析する生の受信信号のポインタ |
| `receive_signal_size` | `int8` | 解析する生の受信信号の長さ |

**戻り値**  
`int1`

**副作用**  
無し

---

### 1.6. check_device_id

**概要**  
受信フレームのデバイスIDが正しいかを判定する

**引数**
| 引数名 | 型 | 説明 |
| - | - | - |
| `device_id` | `unsigned int8` | デバイスID |

**戻り値**  
`int1`

**副作用**  
無し

---

### 2.1. transmit_command

**概要**  
引数のコマンドを送信する

**引数**
| 引数名 | 型 | 説明 |
| - | - | - |
| `frame_id` | `TransmitFrameId` | 送信するフレームID |
| `content` | `unsinged int8 *` | 送信するデータのcontent |
| `content_size` | `int8` | 送信するcontentの長さ |
**戻り値**  
`void`

**副作用**  
- 他の機器との通信

---

### 2.2. transmit

**概要**  
引数のデータを送信する

**引数**
| 引数名 | 型 | 説明 |
| - | - | - |
| `data` | `unsigned int8 * ` | 送信データのポインタ |
| `data_size` | `int8` | 送信データの長さ |

**戻り値**  
`int1`

**副作用**  
- 他の機器との通信

---

### 3.1. calc_crc8

**概要**  
引数のペイロードののCRC8を計算する

**引数**
| 引数名 | 型 | 説明 |
| - | - | - |
| `frame` | `unsigned int8 *` | フレームのポインタ |
| `payload_size` | `int8` | フレームの長さ |

**戻り値**  
`unsigned int8`

**副作用**  
無し