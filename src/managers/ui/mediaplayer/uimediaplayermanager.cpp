#include "uimediaplayermanager.h"

UIMediaPlayerManager::UIMediaPlayerManager(QObject *parent)
    : UInterface{parent}
{

}

void UIMediaPlayerManager::registrationSubscribe()
{

}

PlayStates::State UIMediaPlayerManager::currentState() const
{
    return m_currentState;
}

void UIMediaPlayerManager::play()
{

}

void UIMediaPlayerManager::pause()
{

}

void UIMediaPlayerManager::setCurrentState(const PlayStates::State &newCurrentState)
{
    if (m_currentState == newCurrentState)
        return;
    m_currentState = newCurrentState;
    emit currentStateChanged();
}

float UIMediaPlayerManager::volume() const
{
    return m_volume;
}

void UIMediaPlayerManager::setVolume(float newVolume)
{
    if (qFuzzyCompare(m_volume, newVolume))
        return;
    m_volume = newVolume;
    emit volumeChanged();
}

QString UIMediaPlayerManager::currentTitle() const
{
    return m_currentTitle;
}

void UIMediaPlayerManager::setCurrentTitle(const QString &newCurrentTitle)
{
    if (m_currentTitle == newCurrentTitle)
        return;
    m_currentTitle = newCurrentTitle;
    emit currentTitleChanged();
}
