# Fire-language
個人的に趣味で開発しているプログラミング言語です。<br>
Rust の構文に似ている静的型付けで、実行時コンパイル機能を搭載したインタプリタ言語。

# Build
リンカに mold を使用しているので、ない場合はインストールしてください <br>
https://github.com/rui314/mold

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=mold" ..
make
```

Discord サーバー：
https://discord.gg/CURgAtGH8Z