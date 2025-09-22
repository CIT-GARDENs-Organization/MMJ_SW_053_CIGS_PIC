#include "mt25q copy.h"

MT25Q_Cmd_t mt25q_make_cmd(MT25Q_Model_t model, enum MT25Q_CmdCode_t cmd_type, unsigned long address)
{
    MT25Q_Cmd_t cmd = {0};
    switch (cmd_type) {
        case MT25Q_READ:
            cmd = mt25q_address(model, address);
            break;
        case MT25Q_WRITE:
            cmd = mt25q_address(model, address);
            break;
        case MT25Q_ERASE:
            cmd = mt25q_address(model, address);
            break;
        default:
            break;
    }
    cmd.data.parts.cmd = cmd_type;
    cmd.length += 1; // コマンドバイト分を追加
    return cmd;
}

MT25Q_Cmd_t mt25q_address(MT25Q_Model_t model, unsigned long address)
{
    MT25Q_Cmd_t cmd = {0};
    switch (model) {
        case MT25QL128ABA:
            cmd.data.parts.addr[0] = (address >> 16) & 0xFF; // アドレスの上位バイト
            cmd.data.parts.addr[1] = (address >> 8) & 0xFF;  // アドレスの中間バイト
            cmd.data.parts.addr[2] = address & 0xFF;         // アドレスの下位バイト
            cmd.length += 3;
            break;
        case MT25QL01GBBB:
            cmd.data.parts.addr[0] = (address >> 24) & 0xFF; // アドレスの最上位バイト
            cmd.data.parts.addr[1] = (address >> 16) & 0xFF; // アドレスの上位バイト
            cmd.data.parts.addr[2] = (address >> 8) & 0xFF;  // アドレスの中間バイト
            cmd.data.parts.addr[3] = address & 0xFF;         // アドレスの下位バイト
            cmd.length += 4;
            break;
        default:
            return cmd; // エラー
    }
    return cmd;
}

