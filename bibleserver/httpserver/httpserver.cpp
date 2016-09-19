
#include "httpserver.h"
#include "../httpsession/httpsession.h"

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

namespace bibleserver
{

    //ctor
    httpserver::httpserver( const char *cdomainname )
    {
        int i;

        for( i = 0; i < httpserver_MAX_sockets; i++ )
            this->skts[ i ] = 0;
        this->listen_skt = 0;
        for( i = 0; i < httpserver_MAX_sessions; i++ )
            this->sessions[ i ] = 0;

        this->last_sel_num = 0;
        this->sock_cnt = 0;
        this->ms_time = 0;
    }

    //dtor
    httpserver::~httpserver( void )
    {
        int i;

        for( i = 0; i < httpserver_MAX_sessions; i++ )
        {
            if( this->sessions[ i ] )
                delete this->sessions[ i ];
        }

        if( this->listen_skt > 0 )
            close( this->listen_skt );
    }

    //run
    bool httpserver::run( unsigned int wait_ms_fordata, unsigned int *ms_waited, unsigned int ms_time )
    {
        struct sockaddr_in saddr;
        bool r;

        this->ms_time = ms_time;

        if( this->listen_skt <= 0 )
        {
            this->listen_skt = socket( AF_INET, SOCK_STREAM, 0 );
            if( this->listen_skt > 0 )
            {
                memset( &saddr, '0', sizeof( saddr ) );
                saddr.sin_family = AF_INET;
                saddr.sin_addr.s_addr = htonl(INADDR_ANY);
                saddr.sin_port = htons( 5000 );

                if( bind( this->listen_skt, ( struct sockaddr* )&saddr, sizeof( saddr ) ) != 0 )
                {
                    close( this->listen_skt );
                    this->listen_skt = 0;
                }
                else
                {
                    if( listen( this->listen_skt, 100 ) != 0 )
                    {
                        close( this->listen_skt );
                        this->listen_skt = 0;
                    }
                    else
                        this->addSelSocket( this->listen_skt );
                }
            }
        }

        r = this->testSockets();
        if( r )
            this->acceptSessions();
        this->runSessions();

        return r;
    }

    //add socket to list to test for received data
    void httpserver::addSelSocket( int s )
    {
        int i;

        for( i = 0; i < httpserver_MAX_sockets; i++ )
        {
            if( this->skts[ i ] <= 0 )
            {
                this->skts[ i ] = s;
                return;
            }
        }
    }

    //remove socket from list to test for for received data
    void httpserver::removeSelSocket( int s )
    {
        int i;

        for( i = 0; i < httpserver_MAX_sockets; i++ )
        {
            if( this->skts[ i ] == s )
                this->skts[ i ] = 0;
        }
    }

    //test to see if socket had received data
    bool httpserver::testSelSocket( int s )
    {
        int i;

        for( i = 0; i < httpserver_MAX_sockets; i++ )
        {
            if( this->rdy_skts[ i ] == s )
                return 1;
        }

        return 0;
    }

    //run sessions
    void httpserver::runSessions( void )
    {
        httpsession *s;
        int i;

        for( i = 0; i < httpserver_MAX_sessions; i++ )
        {
            s = this->sessions[ i ];
            if( !s )
                continue;
            if( !s->run( this->ms_time ) )
            {
                delete s;
                this->sessions[ i ] = 0;
            }
        }
    }

    //accept new sessions
    void httpserver::acceptSessions( void )
    {
        int s, i;
        socklen_t sz;
        struct sockaddr_in saddr;
        httpsession **ses;

        if( this->listen_skt <= 0 )
            return;

        if( !this->testSelSocket( this->listen_skt ) )
            return;

        memset( &saddr, 0, sizeof( saddr ) );
        saddr.sin_family = AF_INET;

        sz = sizeof( saddr );
        s = accept( this->listen_skt, (sockaddr *)&saddr, &sz );

        if( s <= 0 )
            return;

        ses = &this->sessions[ 0 ];
        for( i = 0; i < httpserver_MAX_sessions && *ses; i++ )
            ses = &this->sessions[ i ];

        if( *ses )
        {
            close( s );
            return;
        }

        *ses = new httpsession( s, this, this->ms_time );
    }

    //test session sockets for incoming data
    bool httpserver::testSockets( void )
    {
        fd_set fd;
        struct timeval ts;
        int i, j, s, hs;

        FD_ZERO( &fd );
        for( i = 0; i < httpserver_MAX_sockets; i++ )
            this->rdy_skts[ i ] = 0;

        hs = 0;
        for( i = 0; i < httpserver_MAX_sockets; i++ )
        {
            s = this->skts[ i ];
            if( s <= 0 )
                continue;
            if( s >= hs )
                hs = s + 1;
            FD_SET( s, &fd );
        }
        if( hs <= 0 )
            return 0;

        ts.tv_sec = 0;
        ts.tv_usec = 0;
        hs = select( hs, &fd, 0, 0, &ts );

        if( hs <= 0 )
            return 0;

        for( j = i = 0; i < httpserver_MAX_sockets; i++ )
        {
            s = this->skts[ i ];
            if( FD_ISSET( s, &fd ) )
            {
                if( j < httpserver_MAX_sockets )
                {
                    this->rdy_skts[ j ] = s;
                    j++;
                }
            }
        }

        return 1;
    }

};

