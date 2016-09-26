
#include "httpresponse_home.h"
#include "../httprequest/httprequest.h"
#include <fstream>
#include <string>

namespace bibleserver
{

    //ctor
    httpresponse_home::httpresponse_home( httprequest *req, int skt ) : httpresponse( req, skt )
    {
        this->req = req;
    }

    //dtor
    httpresponse_home::~httpresponse_home( void )
    {

    }

    //run response
    bool httpresponse_home::run( void )
    {
        std::fstream f;
        char *c;
        unsigned int fsz;


        f.open( "template.html", f.binary | f.in );
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
        this->sendHTML( c, fsz );

        delete[] c;
        return 0;
    }

};



