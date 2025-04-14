# C言語におけるPIC用命名規則

## 1. ファイル名
- ファイル名は小文字のスネークケースを使用する。
    例: `main.c`, `timer_config.h`

## 2. 変数名
- ローカル変数: 小文字のスネークケースを使用する。
    例: `counter_value`, `adc_result`
- グローバル変数: `g_`プレフィックスを付け、小文字のスネークケースを使用する。
    例: `g_system_status`, `g_timer_count`

## 3. 定数
- 全て大文字のスネークケースを使用する。
    例: `MAX_BUFFER_SIZE`, `TIMER_PERIOD_MS`

## 4. 関数名
- 小文字のスネークケースを使用する。
    例: `init_timer()`, `read_adc_value()`

## 5. マクロ
- 全て大文字のスネークケースを使用する。
    例: `#define LED_ON 1`, `#define TIMER_PRESCALER 8`

## 6. 構造体・列挙型
- 構造体名: 頭文字を大文字にし、キャメルケースを使用する。
    例: `typedef struct TimerConfig`
- 列挙型名: 頭文字を大文字にし、キャメルケースを使用する。
    例: `typedef enum AdcChannel`

## 7. メンバ変数
- 小文字のスネークケースを使用する。
    例: `uint16_t timer_period;`, `uint8_t adc_channel;`

## 8. ポインタ変数
- 変数名の先頭に`p_`を付ける。
    例: `uint8_t *p_data_buffer;`, `TimerConfig *p_timer_config;`

## 9. モジュール名
- モジュール名は小文字のスネークケースを使用する。
    例: `timer.c`, `adc.c`

## 10. コメント
- 関数や重要なコードには適切なコメントを記載する。
    例:
    ```c
    /**
     * @brief Initialize the timer module.
     * @param config Pointer to the timer configuration structure.
     */
    void init_timer(const TimerConfig *p_config);
    ```

## 11. その他
- グローバル変数や関数は、モジュール名をプレフィックスとして付ける。
    例: `timer_start()`, `adc_read_value()`

## 12. 配列
- 配列名は通常の変数名と同様に小文字のスネークケースを使用する。
- ポインタとの混同を避けるため、名前から意味が明確に分かるようにする。
- グローバル配列には g_ プレフィックスを付ける。
- ポインタ配列には p_ プレフィックスを付ける。
