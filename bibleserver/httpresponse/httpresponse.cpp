
#include "httpresponse.h"
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
    httpresponse::httpresponse( httprequest *req, int skt )
    {
        this->req = req;
        this->skt = skt;
    }

    //dtor
    httpresponse::~httpresponse( void )
    {

    }

    //run response
    bool httpresponse::run( void )
    {
        return 0;
    }

    //send data
    void httpresponse::sendBytes( char *b, unsigned int sz )
    {
        send( this->skt, b, sz, 0 );
    }

    //send data
    void httpresponse::sendBytes( const char *b )
    {
        this->sendBytes( (char *)b, strlen( b ) );
    }

    //send sucess
    void httpresponse::sendSucess( char *b, unsigned int sz )
    {
        this->sendBytes( "HTTP/1.0 200 OK\r\n" );
        this->sendBytes( "Content-Type: text/html; charset=UTF-8\r\n" );
        this->sendBytes( "Content-Encoding: UTF-8\r\n" );

        this->sendBytes( "Content-Length: " );
        this->sendInt( sz );
        this->sendBytes( "\r\n" );

        this->sendBytes( "Connection: close\r\n" );
        this->sendBytes( "\r\n" );

        this->sendBytes( b, sz );
        this->sendBytes( "\r\n\r\n" );
    }

    //send icon
    void httpresponse::sendIcon( char *b, unsigned int sz )
    {
        this->sendBytes( "HTTP/1.0 200 OK\r\n" );
        this->sendBytes( "Content-Type: image/x-icon\r\n" );

        this->sendBytes( "Content-Length: " );
        this->sendInt( sz );
        this->sendBytes( "\r\n" );

        this->sendBytes( "Connection: close\r\n" );
        this->sendBytes( "\r\n" );

        this->sendBytes( b, sz );
    }

    //send 404
    void httpresponse::send404( char *b, unsigned int sz )
    {
        this->sendBytes( "HTTP/1.0 404 Not Found\r\n" );
        this->sendBytes( "Content-Type: text/html; charset=UTF-8\r\n" );
        this->sendBytes( "Content-Encoding: UTF-8\r\n" );

        if( b && sz )
        {
            this->sendBytes( "Content-Length: " );
            this->sendInt( sz );
            this->sendBytes( "\r\n" );
        }

        this->sendBytes( "Connection: close\r\n" );
        this->sendBytes( "\r\n" );

        if( !b || !sz )
            this->sendBytes( "404 not found" );
        else
            this->sendBytes( b, sz );

        this->sendBytes( "\r\n\r\n" );
    }

    //send error 500
    void httpresponse::send500( char *b, unsigned int sz )
    {
        this->sendBytes( "HTTP/1.0 500 Internal Server Error\r\n" );
        this->sendBytes( "Content-Type: text/html; charset=UTF-8\r\n" );
        this->sendBytes( "Content-Encoding: UTF-8\r\n" );

        if( b && sz )
        {
            this->sendBytes( "Content-Length: " );
            this->sendInt( sz );
            this->sendBytes( "\r\n" );
        }

        this->sendBytes( "Connection: close\r\n" );
        this->sendBytes( "\r\n" );

        if( !b || !sz )
            this->sendBytes( "500 internal server error" );
        else
            this->sendBytes( b, sz );

        this->sendBytes( "\r\n\r\n" );
    }

    //send int
    void httpresponse::sendInt( unsigned int sz )
    {
        char c;
        unsigned int i, e, d, w;

        for( d = 1, i = sz; i > 0; i /= 10, d *= 10 );

        for( ; d >= 1; d /= 10 )
        {
            w = sz / d;
            sz -= w * d;
            e = 48 + w;
            c = e;

            sendBytes( &c, 1 );
        }
    }

};

