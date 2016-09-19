
#ifndef httpresponse_favicon_h
#define httpresponse_favicon_h

#include "../httpresponse/httpresponse.h"

namespace bibleserver
{

    class httpresponse_favicon : public httpresponse
    {

    public:

        //ctor
        httpresponse_favicon( httprequest *req, int skt );
        //dtor
        virtual ~httpresponse_favicon( void );
        //run response
        virtual bool run( void );

    protected:

    private:

        httprequest *req;

    };

};

#endif // httpresponse_h

