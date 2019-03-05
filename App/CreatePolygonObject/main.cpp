//#include <kvs/glut/Application>
//#include <kvs/glut/Screen>
#include <kvs/PolygonObject>

#include <Lib/Application.h>
#include <Lib/Screen.h>
#include <Lib/TouchController.h>
// 点の数
const size_t N = 4;

// ポリゴンの数
const size_t P = 4;

// 座標値配列の定義
kvs::Real32 CoordArray[ N * 3 ] = {
    1.0, 0.0, 2.0,  // 点0の座標 (1.0, 0.0, 2.0)
    2.0, 0.0, 2.0,  // 点1の座標 (2.0, 0.0, 2.0)
    1.5, 0.8, 1.7,  // 点2の座標 (1.5, 0.8, 1.7)
    1.5, 0.0, 1.1   // 点3の座標 (1.5, 0.0, 1.1)
};

// カラー値配列の定義
kvs::UInt8 ColorArray[ N * 3 ] = {
    255,   0,   0,  // 点0の色 赤(255,   0,   0)
      0, 255,   0,  // 点1の色 緑(  0, 255,   0)
      0,   0, 255,  // 点2の色 青(  0,   0, 255)
    255, 255,   0   // 点3の色 黄(255, 255,   0)
};

// 法線ベクトル配列の定義
kvs::Real32 NormalArray[ N * 3 ] = {
     0.00,  0.3,  0.8,  // 点0に対する法線ベクトル
     0.00, -0.9,  0.0,  // 点1に対する法線ベクトル
     0.72,  0.3, -0.4,  // 点2に対する法線ベクトル
    -0.72,  0.3, -0.4   // 点3に対する法線ベクトル
};

// 接続情報配列の定義
kvs::UInt32 ConnectionArray[ P * 3 ] = {
    0, 1, 2,  // 点0, 1, 2を接続
    0, 3, 1,  // 点0, 3, 1を接続
    1, 3, 2,  // 点0, 3, 2を接続
    0, 2, 3   // 点0, 2, 3を接続
};

// ポリゴンオブジェクトを生成する関数
kvs::PolygonObject* CreatePolygonObject()
{
    // KVSの配列クラス（kvs::ValueArray）にセットする。
    kvs::ValueArray<kvs::Real32> coords( CoordArray, N * 3 );
    kvs::ValueArray<kvs::UInt8> colors( ColorArray, N * 3 );
    kvs::ValueArray<kvs::Real32> normals( NormalArray, N * 3 );
    kvs::ValueArray<kvs::UInt32> connections( ConnectionArray, P * 3 );

    // ポリゴンオブジェクトを生成する。
    kvs::PolygonObject* object = new kvs::PolygonObject();
    object->setCoords( coords );
    object->setColors( colors );
    object->setNormals( normals );
    object->setConnections( connections );
    object->setPolygonType( kvs::PolygonObject::Triangle );
    object->setColorType( kvs::PolygonObject::VertexColor );
    object->setNormalType( kvs::PolygonObject::VertexNormal );

    // 生成したポリゴンオブジェクトのポインタを返す。
    return object;
}

// メイン関数
int main( int argc, char** argv )
{
    kvs::oculus::Application app( argc, argv );

    // ポリゴンオブジェクトの生成
    kvs::PolygonObject* object = CreatePolygonObject();

    kvs::oculus::Screen screen( &app );
    screen.setTitle( "Polygon Object" );
    screen.registerObject( object );
    screen.show();

    kvs::oculus::TouchController controller( &screen );

    return app.run();
}