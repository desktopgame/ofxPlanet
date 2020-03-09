# 頂点レイアウト
このページはofxPlanetのシェーダをカスタマイズするためのドキュメントです。

## Attribute Variables
### Vertex
Vertex は形状の頂点です。属性インデックスは0で、 型は "vec3" です。
これはopenFrameworksのデフォルトバインディングを使用しています。

### TexCoord
TexCoord はテクスチャのUV座標です。 属性インデックスは3で、型は "vec2" です。
これはopenFrameworksのデフォルトバインディングを使用しています。

### Position
Position は形状の位置です。 属性インデックスは4で、型は"ivec3"です。
この変数が必要なのは、インスタンシング描画のためです。

## Uniform Variables
### Brightness
Brightness はライティングに使用します。  
GLSLにおいて "uBrightness" という名前の "float" 型変数で定義される必要があります。
### ModelMatrix
ModelMatrix はメモリ節約のために使用します。  
GLSLにおいて "uModelMatrix" という名前の "mat4" 型変数で定義される必要があります。

