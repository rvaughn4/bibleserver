
#include "httprequest.h"
#include "../httpserver/httpserver.h"
#include "../httpresponse/httpresponse.h"

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
    httprequest::httprequest( int skt, httpserver *svr )
    {
        this->skt = skt;
        this->svr = svr;
        this->buff = 0;
        this->sz = 0;
        this->pos_break = 0;
        this->pos_path = 0;
        this->pos_path_end = 0;
        this->pos_method = 0;
        this->pos_method_end = 0;
        this->cursor = 0;
        this->pos_r = 0;
        this->pos_n = 0;
        this->last_pos_r = 0;
        this->last_pos_n = 0;
    }

    //dtor
    httprequest::~httprequest( void )
    {
        if( this->buff )
            delete[] this->buff;
    }

    //run request
    bool httprequest::run( void )
    {
        char dbuff[ 1024 ];
        int r, szr, i;
        unsigned int szn;

        if( this->svr->testSelSocket( this->skt ) )
        {
            r = recv( this->skt, dbuff, 1024, MSG_PEEK );
            if( r <= 0 || r > 1024 )
                return 0;

            if( !this->resizeBuff( r ) )
                return 0;
            szrem = this->sz - this->cursor;
            r = recv( this->skt, this->buff[ this->cursor ], szrem, 0 );
            if( r <= 0 || r > szrem )
                return 0;

            szn = r + this->cursor;
            for( i = this->cursor; i < szn; i++ )
            {

                if( !this->pos_method_end )
                {
                    if( this->buff[ i ] == 32 )
                        this->pos_method_end = i;
                }
                else
                {
                    if( this->buff[ i ] == 10 )
                    {
                        if( !this->pos_path_end )
                        {
                            this->pos_path = this->pos_method_end + 1;
                            this->pos_path_end = i;
                        }

                        this->last_pos_n = this->pos_n;
                        this->pos_n = i;
                    }
                }
            }

        }



        return 1;
    }

    //resize buffer
    bool httprequest::resizeBuff( unsigned int more_sz )
    {
        char *ob, *nb;
        unsigned int osz, nsz;

        ob = this->buff;
        osz = this->sz;

        nsz = osz + more_sz;
        if( !nsz )
            nb = 0;
        else
            nb = new char[ nsz ];
        if( !nb && more_sz )
            return 0;

        this->buff = nb;
        this->sz = nsz;

        if( ob && osz && nb && nsz )
            memcpy( nb, ob, osz );

        if( ob )
            delete[] ob;

        return 1;
    }

};

