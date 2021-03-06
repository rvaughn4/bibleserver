
#ifndef httpresponse_home_h
#define httpresponse_home_h

#include "../httpresponse/httpresponse.h"

namespace bibleserver
{

    class httpresponse_home : public httpresponse
    {

    public:

        //ctor
        httpresponse_home( httprequest *req, int skt );
        //dtor
        virtual ~httpresponse_home( void );
        //run response
        virtual bool run( void );

    protected:

    private:

        httprequest *req;

    };

};

#endif // httpresponse_h


