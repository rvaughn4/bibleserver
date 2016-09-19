
#include "httpresponse_favicon.h"
#include "../httprequest/httprequest.h"
#include <fstream>
#include <string>

namespace bibleserver
{

    //ctor
    httpresponse_favicon::httpresponse_favicon( httprequest *req, int skt ) : httpresponse( req, skt )
    {
        this->req = req;
    }

    //dtor
    httpresponse_favicon::~httpresponse_favicon( void )
    {

    }

    //run response
    bool httpresponse_favicon::run( void )
    {
        std::fstream f;
        std::string s;
        char *c;
        unsigned int sz, fsz;

        c = this->req->getPathString( &sz );
        if( sz > 1 && *c == 47 )
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
        this->sendIcon( c, fsz );

        delete[] c;
        return 0;
    }

};


