/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::ArrowGlyph class.
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: main.cpp 1379 2012-12-03 06:54:05Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include <kvs/Message>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/ArrowGlyph>
#include <kvs/TornadoVolumeData>
#include <Lib/Application.h>
#include <Lib/Screen.h>
#include <Lib/TouchEventListener.h>


/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [i] argument counter
 *  @param  argv [i] argument values
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    kvs::oculus::Application app( argc, argv );

    /* Read volume data from the specified data file. If the data file is not
     * specified, tornado volume data is created by using kvs::TornadoVolumeData class.
     */
    kvs::StructuredVolumeObject* object = NULL;
    if ( argc > 1 ) object = new kvs::StructuredVolumeImporter( std::string( argv[1] ) );
    else            object = new kvs::TornadoVolumeData( kvs::Vector3ui( 8, 8, 8 ) );

    if ( !object )
    {
        kvsMessageError( "Cannot create a structured volume object." );
        return false;
    }

    // Create an arrow glyph renderer.
    kvs::ArrowGlyph* glyph = new kvs::ArrowGlyph();
    if ( !glyph )
    {
        kvsMessageError( "Cannot creat an arrow glyph.");
        return false;
    }

    // Set properties.
    const kvs::TransferFunction transfer_function( 256 );
    const kvs::ArrowGlyph::ArrowType type = kvs::ArrowGlyph::TubeArrow;
    glyph->setTransferFunction( transfer_function );
    glyph->setType( type );

    kvs::oculus::Screen screen( &app );
    screen.registerObject( object, glyph );
    screen.setTitle( "kvs::ArrowGlyph" );
    screen.show();

    kvs::oculus::TouchEventListener event( &screen );
    event.setRotationFactor( 1200.0f );
    event.setTranslationFactor( 2000.0f );
    event.setScalingFactor( 10000.0f );

    return app.run();
}
