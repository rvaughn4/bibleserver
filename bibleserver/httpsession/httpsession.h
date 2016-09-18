
#ifndef httpsession_h
#define httpsession_h

namespace bibleserver
{

    class httpserver;
    class httprequest;

    class httpsession
    {

    public:

        //ctor
        httpsession( int skt, httpserver *svr, unsigned int ms_time );
        //dtor
        virtual ~httpsession( void );
        //run session, returns true if should continue to run
        virtual bool run( unsigned int ms_time );

    protected:

    private:

        int skt;
        httpserver *svr;
        char *buff;
        httprequest *req;
        unsigned int ms_start, ms_last, cursor;

    };

};

#endif // httpsession_h
