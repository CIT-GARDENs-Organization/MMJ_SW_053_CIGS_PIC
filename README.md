### 試験方法
 ・送信CMD : 10 00 00 00 00 00 00 00 00 00
 ・詳細    : ADCの電圧取得ポートと電流取得ポートからデータをとる
 ・出力形式は電圧、電流の順
 ・ADCスペック：0-2.5V, 12bit
 ・DACスペック：0-3.3V, 8bit

### 検証済み
・オペアンプ用-3.3V, DAC、電流センサ用のVref
・ADCのDACの電圧を変え、ADCでデータを取得

### 確認不具合
・ADCの関数はadc_readata()を使う
・ADCの関数の引数にはFABのコード対応するポートを調べ、参照する


### 構文
- 構造体はすべてポインターで操作する
- 構造体は末尾に`_t`をつける
- ポインターの場合は`_ptr`を末尾につける
```C
typedef union{
    struct{
        unsigned int8 start_marker;
        unsigned int8 reserved;
        unsigned int8 command;
        unsigned int8 port_num;
        unsigned int16 data[2];
    }fields;

    unsigned int8 raw[PACKET_SIZE];
}iv_packet_t;

iv_packet_t iv_packet;
iv_packet_t *iv_packet_ptr = &iv_packet;
```


