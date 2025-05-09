#ifndef APICLIENT_H
#define APICLIENT_H

#include "api/external/def.h"

namespace api{
namespace client{

API(ClientInfo){
    API_FIELD(Ip) // M, quint32
    API_FIELD(Port) // M, quint16
}

} // client
} // api

#endif // APICLIENT_H
