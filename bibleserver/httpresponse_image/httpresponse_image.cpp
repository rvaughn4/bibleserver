
#include "httpresponse_image.h"
#include "../httprequest/httprequest.h"
#include <fstream>
#include <string>

namespace bibleserver
{

    //ctor
    httpresponse_image::httpresponse_image( httprequest *req, int skt ) : httpresponse( req, skt )
    {
        this->req = req;
    }

    //dtor
    httpresponse_image::~httpresponse_image( void )
    {

    }

    //run response
    bool httpresponse_image::run( void )
    {
        std::fstream f;
        std::string s;
        char *c;
        unsigned int sz, fsz, i;

        c = this->req->getPathString( &sz );

        for( i = 0; i < sz; i++ )
        {
            if
            (
             !(

                ( c[ i ] >= 65 && c[ i ] <= 90 ) ||
                ( c[ i ] >= 97 && c[ i ] <= 122 ) ||
                ( c[ i ] >= 48 && c[ i ] <= 57 ) ||
                ( c[ i ] == 46 )
             )
             )
             c[ i ] = 48;
        }

        if( sz > 1 && *c == 48 )
            s.assign( c + 1, sz - 1 );
        else
            s.assign( c, sz );

        f.open( s.c_str(), f.binary | f.in );
        if( !f.is_open() )
        {
            this->send404( 0, 0 );
            return 0;
        }

        f.seekg( 0, f.end );
        fsz = f.tellg();
        f.seekg( 0, f.beg );

        c = new char[ fsz ];
        if( !c )
        {
            this->send500( 0, 0 );
            return 0;
        }

        f.read( c, fsz );
        this->sendData( c, fsz );

        delete[] c;
        return 0;
    }

};


