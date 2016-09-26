
#include "httprequest.h"
#include "../httpserver/httpserver.h"
#include "../httpresponse/httpresponse.h"
#include "../httpresponse_image/httpresponse_image.h"
#include "../httpresponse_home/httpresponse_home.h"

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
        this->resp = 0;
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
        if( this->resp )
            delete this->resp;
        if( this->buff )
            delete[] this->buff;
    }

    //run request
    bool httprequest::run( void )
    {
        char dbuff[ 1024 ];
        int r, szrem;
        unsigned int szn, i;

        if( this->resp )
            return this->resp->run();

        if( this->svr->testSelSocket( this->skt ) )
        {
            r = recv( this->skt, dbuff, 1024, MSG_PEEK );
            if( r <= 0 || r > 1024 )
                return 0;

            if( !this->resizeBuff( r ) )
                return 0;
            szrem = this->sz - this->cursor;
            r = recv( this->skt, &this->buff[ this->cursor ], szrem, 0 );
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
                    if( this->buff[ i ] == 32 )
                    {
                        if( !this->pos_path_end )
                        {
                            this->pos_path = this->pos_method_end + 1;
                            this->pos_path_end = i;
                        }
                    }

                    if( this->buff[ i ] == 10 )
                    {
                        this->last_pos_n = this->pos_n;
                        this->pos_n = i;

                        //handle method
                        if( this->buff[ this->pos_method ] != 71 && this->buff[ this->pos_method ] != 103 )
                            return 0;
                            //std::cout << this->buff << "\r\n";
                        if( this->pos_n - this->last_pos_n <= 2 )
                        {
                            if(
                                !this->resp
                            &&
                            (
                                this->pathContains( ".ico" ) ||
                                this->pathContains( ".jpg" ) ||
                                this->pathContains( ".png" ) ||
                                this->pathContains( ".bmp" ) ||
                                this->pathContains( ".gif" )
                            )
                            )
                                this->resp = new httpresponse_image( this, this->skt );
                            if( !this->resp )
                                this->resp = new httpresponse_home( this, this->skt );

                        }
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

    //return method
    char *httprequest::getMethodString( unsigned int *psz )
    {
        if( this->pos_method_end > this->sz )
            this->pos_method_end = this->sz;
        if( this->pos_method >= this->sz )
            this->pos_method = 0;
        *psz = this->pos_method_end - this->pos_method;
        if( !this->buff )
            return 0;
        return &this->buff[ this->pos_method ];
    }

    //return file/path
    char *httprequest::getPathString( unsigned int *psz )
    {
        if( this->pos_path_end > this->sz )
            this->pos_path_end = this->sz;
        if( this->pos_path >= this->sz )
            this->pos_path = 0;
        *psz = this->pos_path_end - this->pos_path;
        if( !this->buff )
            return 0;
        return &this->buff[ this->pos_path ];
    }

    //search path for string
    bool httprequest::pathContains( char *c, unsigned int sz )
    {
        unsigned int psz, i, j;
        char *pth;

        pth = this->getPathString( &psz );
        if( !pth || !c )
            return 0;

        for( i = 0; i < psz; i++ )
        {
            if( pth[ i ] == c[ 0 ] )
            {
                for( j = 0; j < sz && i + j < psz && pth[ i + j ] == c[ j ]; j++ )
                {
                    if( j == sz - 1 )
                        return 1;
                }
            }
        }

        return 0;
    }

    //search path for string
    bool httprequest::pathContains( const char *c )
    {
        return this->pathContains( (char *)c, strlen( c ) );
    }

};

