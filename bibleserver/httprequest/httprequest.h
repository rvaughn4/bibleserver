
#ifndef httprequest_h
#define httprequest_h

namespace bibleserver
{

    class httpserver;
    class httpresponse;

    class httprequest
    {

    public:

        //ctor
        httprequest( int skt, httpserver *svr );
        //dtor
        virtual ~httprequest( void );
        //run request
        virtual bool run( void );
        //return method
        char *getMethodString( unsigned int *psz );
        //return file/path
        char *getPathString( unsigned int *psz );
        //search path for string
        bool pathContains( char *c, unsigned int sz );
        //search path for string
        bool pathContains( const char *c );

    protected:

    private:

        //resize buffer
        bool resizeBuff( unsigned int more_sz );

        char *buff;
        unsigned int sz, pos_break, pos_path, pos_path_end, pos_method, pos_method_end, cursor, pos_r, pos_n, last_pos_r, last_pos_n;
        int skt;
        httpserver *svr;
        httpresponse *resp;
    };

};

#endif // httprequest_
