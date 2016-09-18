
#ifndef httpserver_h
#define httpserver_h

namespace bibleserver
{

    #define httpserver_MAX_sockets 512
    #define httpserver_MAX_sessions 512
    class httpsession;

    class httpserver
    {

    public:

        //ctor
        httpserver( const char *cdomainname );
        //dtor
        virtual ~httpserver( void );
        //run
        virtual bool run( unsigned int wait_ms_fordata, unsigned int *ms_waited, unsigned int ms_time );
        //add socket to list to test for received data
        void addSelSocket( int s );
        //remove socket from list to test for for received data
        void removeSelSocket( int s );
        //test to see if socket had received data
        bool testSelSocket( int s );

    protected:

    private:

        //run sessions
        void runSessions( void );
        //accept new sessions
        void acceptSessions( void );
        //test session sockets for incoming data
        bool testSockets( void );

        //select socket list
        int sock_cnt, last_sel_num, skts[ httpserver_MAX_sockets ], rdy_skts[ httpserver_MAX_sockets ], listen_skt;
        unsigned int ms_time;
        httpsession *sessions[ httpserver_MAX_sessions ];

    };

};

#endif // httpserver_
