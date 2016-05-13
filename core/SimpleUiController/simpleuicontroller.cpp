/*
* Copyright (C) 2016  Integrated Computer Solutions, Inc
* 54 B Middlesex Turnpike, Bedford, MA 01730
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#if ! defined QT_NO_DEBUG_OUTPUT
//#define QT_NO_DEBUG_OUTPUT
#endif
#include <QDebug>

#include "simpleuicontroller.h"
//#include "../QIcsMediaManager/mediaplayerinterface.h"
#include "playlistmodel.h"

SimpleUiController::SimpleUiController()
    : engine(new QQmlEngine()),
      quickView(new QQuickView()),
      m_activeMediaSession("NoType"),
    myAudioModel(new PlayListModel(this)),
    myVideoModel(new PlayListModel(this)),
    myState(mmTypes::Stopped)

{
    connect(myAudioModel,SIGNAL(currentTrackChanged()), this, SIGNAL(currentTrackChanged()));
    connect(myVideoModel,SIGNAL(currentTrackChanged()), this, SIGNAL(currentTrackChanged()));
    qDebug() << Q_FUNC_INFO;
    Q_INIT_RESOURCE(qml);
    qmlRegisterUncreatableType<SimpleUiController>("Model", 1, 0, "Constants","controller defines it");
    quickView->engine()->rootContext()->setContextProperty("controller", this);
    quickView->engine()->addImageProvider("coverArt", myAudioModel->currentCoverArt());
    quickView->setSource(QUrl(QStringLiteral("qrc:/simpleuicontroller/main.qml")));
// FIXME: This needs to become a real configuration option for when we build for a Genivi Device
//    quickView->setProperty("IVI-Surface-ID",3);

    // All views opened should be set to FramelessWindowHint. Doing it in main is not a good idea because
    // some windows might not get created until later. We can create a MediaManager QuickView base class
    // and set the window flags we want there at some point if we think it is needed
    quickView->setFlags(Qt::FramelessWindowHint);

// FIXME: For a quick demo on the ICS 3 screen setup
//    quickView->setX(2480);quickView->setY(200);
    quickView->show();
}

QString SimpleUiController::activeMediaSession() const
{
    return m_activeMediaSession;
}


void SimpleUiController::requestActiveMediaSession(const QString activeSession)
{
    emit requestSetActiveMediaSessionTo(activeSession);
}

void SimpleUiController::setActiveMediaSession(QString activeMediaSession)
{
    qDebug() << Q_FUNC_INFO << "new" << activeMediaSession << "was" << m_activeMediaSession;
    if (m_activeMediaSession == activeMediaSession)
        return;

    m_activeMediaSession = activeMediaSession;
    emit activeMediaSessionChanged(activeMediaSession);
}

void SimpleUiController::setActiveMediaSessionPlaylist(const QJsonArray playList)
{
    qDebug() << Q_FUNC_INFO/* << playList*/;
    // TODO: whatever needs to be done with it...
    // mostly update the Model to show in the UI
    PlayListModel * currentModel = (m_activeMediaSession == "AudioFile")? myAudioModel : myVideoModel;
    currentModel->revert();
    currentModel->insertRows(0, playList.count(), QModelIndex());
    for(int pos = 0; pos < playList.count(); pos++)
    {
        currentModel->setData(currentModel->index(pos,0,QModelIndex()), playList[pos].toVariant(), Qt::UserRole);
    }

    if(m_activeMediaSession == "AudioFile")
        emit audioPlaylistChanged();
    else emit videoPlaylistChanged();
}

void SimpleUiController::setMediaSessions(const QStringList mediaSessions)
{
    qDebug() << Q_FUNC_INFO << mediaSessions;
    if (m_mediaSessions != mediaSessions) {
        m_mediaSessions = mediaSessions;
        emit mediaSessionsChanged();
    }
}

void SimpleUiController::setPlayState(const MediaPlayerInterface::PlayState playState)
{
    qDebug() << Q_FUNC_INFO << playState;
    if(myState != playState){
        myState = playState;
        emit playStateChanged();
    }
}

int SimpleUiController::currentIndex() const
{
    PlayListModel * currentModel = (m_activeMediaSession == "AudioFile")? myAudioModel : myVideoModel;
    return currentModel->currentIndex();
}

void SimpleUiController::setCurrentTrackIndex(const int index)
{
    qDebug() << Q_FUNC_INFO << index;
    PlayListModel * currentModel = (m_activeMediaSession == "AudioFile")? myAudioModel : myVideoModel;
    if(currentModel->currentIndex() != index){
        currentModel->setCurrentIndex(index);
        emit requestTrackAtIndex(index);
    }
}




void SimpleUiController::play() const
{
    qDebug() << Q_FUNC_INFO;
    emit requestPlay();
}

void SimpleUiController::pause() const
{
    qDebug() << Q_FUNC_INFO;
    emit requestPause();
}

void SimpleUiController::stop() const
{
    qDebug() << Q_FUNC_INFO;
    emit requestStop();
}

void SimpleUiController::previous() const
{
    qDebug() << Q_FUNC_INFO;
    emit requestPrevious();
}

void SimpleUiController::next() const
{
    qDebug() << Q_FUNC_INFO;
    emit requestNext();
}

void SimpleUiController::setVideoRectangle(int x, int y, int height, int width)
{
    emit requestChangeVideoRectangle(QRect(quickView->x() + x, quickView->y() + y, width, height));
}

QStringList SimpleUiController::mediaSessions() const
{
    return m_mediaSessions;
}

QJsonObject SimpleUiController::currentTrack() const
{
    PlayListModel * currentModel = (m_activeMediaSession == "AudioFile")? myAudioModel : myVideoModel;
    return currentModel->currentTrack();
}

MediaPlayerInterface::PlayState SimpleUiController::playState() const
{
    return myState;
}

QVariantList SimpleUiController::audioPlaylist() const
{
    QVariantList playList;
    for(int pos = 0; pos < myAudioModel->rowCount(); pos++)
        playList.append(myAudioModel->data(myAudioModel->index(pos,0,QModelIndex()), Qt::UserRole));
   return playList;
}

QVariantList SimpleUiController::videoPlaylist() const
{
    QVariantList playList;
    for(int pos = 0; pos < myVideoModel->rowCount(); pos++)
        playList.append(myVideoModel->data(myVideoModel->index(pos,0,QModelIndex()), Qt::UserRole));
   return playList;
}

QUrl SimpleUiController::qml()
{
    return QUrl("qrc://main.qml");
}
