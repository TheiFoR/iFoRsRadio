#ifndef APPRADIO_H
#define APPRADIO_H

#include "api/internal/def.h"

namespace app{
namespace radio{

I_API(Play){
    API_FIELD(Name)  // M, QString
    API_FIELD(URL)   // M, QString
}
I_API(Stop){
}

} // radio
} // app

#endif // APPRADIO_H
