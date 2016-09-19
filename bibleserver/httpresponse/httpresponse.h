
#ifndef httpresponse_h
#define httpresponse_h

namespace bibleserver
{

    class httprequest;

    class httpresponse
    {

    public:

        //ctor
        httpresponse( httprequest *req, int skt );
        //dtor
        virtual ~httpresponse( void );
        //run response
        virtual bool run( void );

    protected:

        //send data
        void sendBytes( const char *b );
        //send data
        void sendBytes( char *b, unsigned int sz );
        //send sucess
        void sendSucess( char *b, unsigned int sz );
        //send icon
        void sendIcon( char *b, unsigned int sz );
        //send 404
        void send404( char *b, unsigned int sz );
        //send error 500
        void send500( char *b, unsigned int sz );
        //send int
        void sendInt( unsigned int sz );

    private:

        httprequest *req;
        int skt;

    };

};

#endif // httpresponse_h
