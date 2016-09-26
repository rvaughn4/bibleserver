
#ifndef httpresponse_image_h
#define httpresponse_image_h

#include "../httpresponse/httpresponse.h"

namespace bibleserver
{

    class httpresponse_image : public httpresponse
    {

    public:

        //ctor
        httpresponse_image( httprequest *req, int skt );
        //dtor
        virtual ~httpresponse_image( void );
        //run response
        virtual bool run( void );

    protected:

    private:

        httprequest *req;

    };

};

#endif // httpresponse_h

