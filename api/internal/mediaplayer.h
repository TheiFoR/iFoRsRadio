#ifndef APPMEDIAPLAYER_H
#define APPMEDIAPLAYER_H

#include "api/internal/def.h"

namespace app{
namespace mediaPlayer{

I_API(PlayerPlay){
    API_FIELD(Name)  // M, QString
    API_FIELD(URL)   // M, QUrl
}
I_API(PlayerStop){
}

} // server
} // app

#endif // APPMEDIAPLAYER_H
