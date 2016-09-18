
#include "bibleserver.h"
#include "../httpserver/httpserver.h"
#include <thread>

namespace bibleserver
{

    //ctor
    bibleserver::bibleserver( void )
    {

    }

    //dtor
    bibleserver::~bibleserver( void )
    {

    }

    //run until finished
    void bibleserver::runUntilDone( void )
    {
        httpserver *svr;
        unsigned int t;

        svr = new httpserver( "" );

        t = 0;
        while( 1 )
        {
            svr->run( 0, 0, t );
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            t += 100;
        }


    }

};
