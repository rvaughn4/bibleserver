
#ifndef httprequest_h
#define httprequest_h

namespace bibleserver
{

    class httpserver;

    class httprequest
    {

    public:

        //ctor
        httprequest( int skt, httpserver *svr );
        //dtor
        virtual ~httprequest( void );
        //run request
        virtual bool run( void );

    protected:

    private:

        //resize buffer
        bool resizeBuff( unsigned int more_sz );

        char *buff;
        unsigned int sz, pos_break, pos_path, pos_path_end, pos_method, pos_method_end, cursor, pos_r, pos_n, last_pos_r, last_pos_n;
        int skt;
        httpserver *svr;

    };

};

#endif // httprequest_
