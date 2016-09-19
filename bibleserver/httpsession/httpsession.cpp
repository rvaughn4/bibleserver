
#include "httpsession.h"
#include "../httpserver/httpserver.h"
#include "../httprequest/httprequest.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#include <string.h>

namespace bibleserver
{

    //ctor
    httpsession::httpsession( int skt, httpserver *svr, unsigned int ms_time )
    {
        this->skt = skt;
        this->svr = svr;
        this->ms_start = ms_time;
        this->ms_last = ms_time;
        this->req = 0;
    }

    //dtor
    httpsession::~httpsession( void )
    {
        this->svr->removeSelSocket( this->skt );
        if( this->req )
            delete this->req;
        close( this->skt );
    }

    //run session, returns true if should continue to run
    bool httpsession::run( unsigned int ms_time )
    {
        if( ms_time - 3000 > this->ms_start )
            return 0;

        if( !this->req )
            this->req = new httprequest( this->skt, this->svr );
        if( !this->req )
            return 0;
        this->svr->addSelSocket( this->skt );

        return this->req->run();
    }

};

