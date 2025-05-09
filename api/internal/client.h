#ifndef APPCLIENT_H
#define APPCLIENT_H

#include "api/internal/def.h"

namespace app{
namespace client{

I_API(ClientInfo){
    API_FIELD(Ip) // M, QString
    API_FIELD(Port) // M, quint16
}

} // client
} // app

#endif // APPCLIENT_H
