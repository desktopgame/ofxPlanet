ofxPlanet
=====================================

ofxPlanetは地形生成ライブラリです。  
![スクリーンショット](mov.gif)

Introduction
------------
ofxPlanetは地形生成ライブラリです。  
インスタンシングを使用して、平面だけで構成された世界を高速に描画します。  
* 2Dパーリンノイズで地形を生成できます。
* 3Dパーリンノイズで空洞を生成できます。
* 重み付けテーブルを使用して任意の構造物を生成できます。

License
-------
MIT License

Installation
------------
このフォルダをopenFrameworks/addons/に置いてください。

Dependencies
------------
* [ofxSOIL](https://github.com/desktopgame/ofxSOIL)

Compatibility
------------
* of_v0.10.1_vs2017_release
* Windows10

Known issues
------------
現在のところ、既知の不具合はありません。

Version history
------------

### Version 0.1 (2020/02/03):
最初のリリースです。

### Version 0.2 (2020/02/011):
* ライティングシステムを実装
  * 現在のところ太陽光のみをサポート
* レイキャスト関数を追加
  * インターフェイス: world.raycast(origin, dir, length) -> RaycastResult
* チャンクシステムを実装
  * インターフェイス: world.getChunk() -> Chunk

### Version 1.1 (2020/02/23):
インテルのグラフィックスドライバ固有の問題に対するワークアラウンドを実装

### Version 1.1.1 (2020/02/24):
チャンクの分割ローディングをサポート

### Version 1.1.2 (2020/02/24):
軽微な修正

### Version 1.2(2020/03/02):
* 無限のワールドを実装
* 以前の有限なワールドクラスは FixedWorld にリネーム
* Planetクラスを削除
* ソースフォルダのライセンスファイルを削除
  * このissueで報告された問題です。 [issue#1](https://github.com/desktopgame/ofxPlanet/issues/1)