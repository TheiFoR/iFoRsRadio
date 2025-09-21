#ifndef APPMEDIAPLAYER_H
#define APPMEDIAPLAYER_H

#include "api/internal/def.h"

namespace app{
namespace mediaPlayer{

I_API(PlayerPlay){
    API_FIELD(Id)  // M, quint64 radio station id
    API_FIELD(Name)  // O, QString
    API_FIELD(URL)   // O, QUrl
}
I_API(PlayerPause){
}
I_API(PlayerStop){
}

I_API(PlayerVolume){
    API_FIELD(Volume) // M, float 0.0f - 1.0f
}
I_API(PlayerPlaybackStateChanged){
    API_FIELD(Id)  // M, quint64 radio station id
    API_FIELD(State)  // M, PlayStates::State
}
I_API(PlayerMediaStatusChanged){
    API_FIELD(Id)  // M, quint64 radio station id
    API_FIELD(State)  // M, PlayStates::State
}
I_API(PlayerCurrentTitleChanged){
    API_FIELD(Title)  // M, QString
}

} // mediaPlayer
} // app

#endif // APPMEDIAPLAYER_H
