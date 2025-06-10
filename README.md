# GomokuAI

GomokuAI 是一個在終端機上執行的五子棋 (Gomoku) 遊戲，使用
C++20 實作，支援人類玩家與簡易 AI 對弈。

## 特色

- 15x15 棋盤，直接在終端機操作
- 透過方向鍵移動游標，Enter 放置棋子，Q 離開遊戲
- `--p1=ai`、`--p2=ai` 參數可分別啟用 AI 玩家
- AI 會嘗試贏棋、阻擋對手，或盡量落在鄰近位置

## 專案結構

```bash
.
├── gen_compile_commands.sh   # 產生 compile_commands.json 的腳本
├── include/                  # 標頭檔
├── src/                      # 程式實作
├── tests/                    # 測試程式
├── Makefile
├── README.md
└── AGENTS.md                 # OpenAI Codex 引導文件
```

### 重要檔案

- `include/` 與 `src/`：遊戲各模組的標頭與實作檔案。
- `Board.hpp`：定義棋盤大小與勝負檢查函式。
- `Game.hpp`：組合 `Terminal`、`Renderer` 等元件的遊戲主體。
- `Terminal.hpp`：處理鍵盤輸入與 ANSI 控制序列。

主要模組說明：

- `Board`, `Renderer`, `Terminal`：棋盤資料與終端機輸入/顯示處理
- `AIPlayer`：AI 下棋邏輯
- `HumanPlayer`：處理玩家輸入

## 物件關係與主要邏輯

`Terminal` 會將終端機切換成 raw mode，讀取鍵盤輸入並輸出 ANSI 控制序列；
`Renderer` 依賴 `Terminal` 與 `Board` 繪製棋盤並處理游標移動；
`Player` 為取得下一步的介面，`HumanPlayer` 透過 `Renderer` 回報游標位置；
`AIPlayer` 以簡單規則與 Negamax 搜尋產生落子。

`Game` 統整以上元件，負責遊戲流程：繪製棋盤、取得玩家下一步、放置棋子並判斷勝負。

## 建置方式

本專案以 `make` 進行建置，需先安裝支援 C++20 的編譯器（例如 `g++`）。

```bash
make        # 產生執行檔 bin/gomoku
make clean  # 移除產生的檔案
```

若需給 clangd 等工具使用 `compile_commands.json`，可執行（需先安裝 `bear`）：

```bash
./gen_compile_commands.sh
```

## 執行遊戲

建置完成後，可透過下列指令執行：

```bash
./bin/gomoku [--p1=ai] [--p2=ai]
```

依需求指定其中一方為 AI。

## 執行測試

所有測試置於 `tests/` 目錄，可透過以下指令編譯並執行：

```bash
make test
```

每個測試程式都會在 `bin/tests/` 下產生對應的可執行檔並被執行。
