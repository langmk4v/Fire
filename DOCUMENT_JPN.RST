Fire 言語仕様書
###############

.. contents:: 目次
   :depth: 4
   :local:

概要
^^^^^^^

本仕様書は、プログラミング言語 Fire の言語仕様書です。

Fire は、Rust に強く影響を受けた 静的型付けプログラミング言語であり、
明示的で安全な設計、読みやすさ、実装容易性を重視しています。

字句構造
^^^^^^^^

コメント

Fire では、行コメントとブロックコメントをサポートします。

行コメント
----------

``//`` から行末までがコメントとなります。

.. code-block::

   // 行コメント
   var x = 10; // 末尾コメント

ブロックコメント
----------------

``/*`` と ``*/`` に囲まれた範囲がコメントになります。
複数行にまたがることができます。

- ネストは禁止

.. code-block::

   /*
     ブロックコメント
     複数行可能
   */


型システム
^^^^^^^^^^

Fire は 強い**静的型システム** を採用します。

すべての式・変数・関数の戻り値は、コンパイル時に型が決定されます。

基本型
------

- ``None``   : 値を持たない単位型
- ``int``    : 符号付き整数型
- ``float``  : 浮動小数点数型
- ``bool``   : 真偽値型（``true`` / ``false``）
- ``char``   : Unicode 文字
- ``string`` : UTF-8 文字列


組み込みジェネリック型

* `Option<T>` : 値の有無を表す型
* `Vec<T>`    : 可変長配列
* `List<T>`   : 連結リスト
* `Dict<K,V>` : 連想配列

関数型


関数は第一級オブジェクトであり、以下の型を持ちます。

::

   function<戻り値型, 引数型...>


式 ( Expression )
^^^^^^^^^^^^^^^^^^^^^

式の概要

式は **値を生成する構文要素** です。
Fire では多くの構文が式として扱われます。

リテラル式


- 整数リテラル: ``123``
- 浮動小数点: ``3.14``
- 真偽値: ``true`` / ``false``
- 文字: ``'a'``
- 文字列: ``"abc"``


変数参照式

.. code-block::

   x

算術演算式


- ``+`` ``-`` ``*`` ``/`` ``%``

.. code-block::

   a + b * c


比較演算式

* ``==`` ``!=`` ``<`` ``<=`` ``>`` ``>=``

論理演算式


- ``&&`` ``||`` ``!``


代入式
~~~~~~

代入は式であり、代入された値を返します。

.. code-block::

   x = 10


関数呼び出し式

.. code-block::

   func(a, b)

メンバ参照式


.. code-block::

   obj.field


メソッド呼び出し式

.. code-block::

   obj.method(arg)

文（Statement）
^^^^^^^^^^^^^^^^

文は **処理の実行単位** であり、スコープや制御構造を定義します。

スコープ
------------

.. code-block::

   {
      stmt1;
      stmt2;
   }

if 文
------------

.. code-block::

   if cond {
      ...
   }
   else if cond2 {
      ...
   }
   else {
      ...
   }


match 文
------------

Rust 風のパターンマッチ

.. code-block::

   match value {
      Kind::A => { ... }
      Kind::B(x) => { ... }
      _ => { ... }
   }

while 文
------------

.. code-block::

   while cond {
       ...
   }


for 文
------------

.. code-block::

   for x in collection {
       ...
   }


loop 文
------------

.. code-block::

   loop {
       ...
   }


break / continue 文
------------------------

.. code-block::

   break;
   continue;

return 文
------------------------

.. code-block::

   return value;


try-catch 文
------------------------

.. code-block::

   try {
      ...
   }
   catch e: ExceptionType {
      ...
   }

変数
^^^^^^^^^

変数定義


.. code-block::

   var x = 10;


型注釈
~~~~~~

.. code-block::

   var s: string = "abc";


シャドウイング

.. code-block::

   var x : int = 1;

   var x = "aiueo";
      // ここから先は x の型は string

関数
^^^^^^^^^

関数定義
----------------

.. code-block::

   fn add(a: int, b: int) -> int {
       return a + b;
   }


関数テンプレート
----------------

.. code-block::

   fn add<T>(a: T, b: T) -> T {
      return a + b;
   }

クラス
^^^^^^^^^^

.. code-block::

   class MyClass {
      var num: int;

      fn new(n: int) -> MyClass {
         return MyClass(num: n);
      }

      fn get(self) -> int {
         return self.num;
      }
   }

列挙型
^^^^^^^^^^^^^^^^

.. code-block::

   enum Kind {
      A,
      B(int),
      C(int, string),
      D(a: int, b: int)
   }

名前空間
^^^^^^^^^^^

.. code-block::

   namespace A {
      ...
   }

   namespace A::B {

   }

モジュール
^^^^^^^^^^^

モジュールはファイル単位で定義されます。

EBNF 文法定義
^^^^^^^^^^^^^^^

以下は Fire 言語の完全な EBNF 定義です。

::

   program        ::= { top_level }
   top_level      ::= function | class | enum | namespace

   function       ::= "fn" IDENT [ template ] "(" [ params ] ")" "->" type block
   template       ::= "<" IDENT { "," IDENT } ">"
   params         ::= param { "," param }
   param          ::= IDENT ":" type

   class          ::= "class" IDENT [ template ] [ ":" IDENT ] "{" { class_item } "}"
   class_item     ::= field | method
   field          ::= "var" IDENT ":" type ";"
   method         ::= function

   enum           ::= "enum" IDENT "{" { enum_variant } "}"
   enum_variant   ::= IDENT [ "(" [ params ] ")" ] ","

   namespace      ::= "namespace" IDENT { "::" IDENT } block

   block          ::= "{" { statement } "}"
   statement      ::= block | if | while | for | loop | match | return | break | continue | expr ";"

   if             ::= "if" expr block { "else" "if" expr block } [ "else" block ]
   while          ::= "while" expr block
   for            ::= "for" IDENT "in" expr block
   loop           ::= "loop" block
   match          ::= "match" expr "{" { match_arm } "}"
   match_arm      ::= pattern "=>" block

   return         ::= "return" [ expr ] ";"
   break          ::= "break" ";"
   continue       ::= "continue" ";"

   expr           ::= assignment
   assignment     ::= logic_or [ "=" assignment ]
   logic_or       ::= logic_and { "||" logic_and }
   logic_and      ::= equality { "&&" equality }
   equality       ::= relational { ("==" | "!=") relational }
   relational     ::= additive { ("<" | "<=" | ">" | ">=") additive }
   additive       ::= multiplicative { ("+" | "-") multiplicative }
   multiplicative ::= unary { ("*" | "/" | "%") unary }
   unary          ::= ("!" | "-") unary | primary
   primary        ::= literal | IDENT | function_call | member_access | "(" expr ")"

   function_call  ::= IDENT "(" [ args ] ")"
   member_access  ::= primary "." IDENT
   args           ::= expr { "," expr }

   literal        ::= INT | FLOAT | STRING | CHAR | "true" | "false" | "None"
   type           ::= IDENT [ "<" type { "," type } ">" ]
