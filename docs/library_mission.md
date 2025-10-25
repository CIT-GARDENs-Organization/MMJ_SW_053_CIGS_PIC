# ライブラリ仕様書 `mission`

## 1. 概要
このライブラリは、MIS MCUにおけるミッション実行機能を提供する

## 2. 関数一覧
「<span style="color:red;">+</span>」: 外部ファイルからアクセス可能な関数<br>
「<span style="color:blue;">-</span>」: 内部専用関数(static)

1. <span style="color:red;">+</span>void execute_mission(unsigned int8 *content) <br>
(以降は各々の実装するミッションが入る。<br>本来はstaticであるが、実装の敷居を下げるためstaticはなくても良い)
1. <span style="color:blue;">-</span>void example_00(unsigned int8 parameter[])
2. <span style="color:blue;">-</span>void example_01(unsigned int8 parameter[])
3. <span style="color:blue;">-</span>void example_02(unsigned int8 parameter[])

## 3. 関数詳細
### 1. execute_mission

**概要**  
引数のデータをコマンドIDとパラメーターに分け、コマンドIDに対応したミッション（関数）を実行する。

**引数**

| 引数名 | 型 | 説明 |
| - | - | - |
| `content` | `unsigned int8 *` | 受信したアップリンクコマンドフレームのペイロード部のポインタ |

**戻り値**  
`void`

**副作用**  
- 各ミッションの実行
  - ミッション終了後のMIS MCUの起動保証時間`duration_second`の操作の可能性
  - SMFへコピーするデータのリスト`smf_data`の操作の可能性
  - BOSS PICとの通信の可能性
- 実行済みミッションリスト`executed_mission`・`executed_mission_count`の操作 