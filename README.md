# Fire-language
個人的に趣味で開発しているプログラミング言語です。
Rust の構文に似ている静的型付けで、実行時コンパイル機能を搭載したインタプリタ言語。

# Build
```
mkdir build
cd build
cmake -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=memory" ..
make
```

Discord サーバー：
https://discord.gg/CURgAtGH8Z