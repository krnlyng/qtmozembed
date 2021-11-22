/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef QMOZWINDOW_PRIVATE_H
#define QMOZWINDOW_PRIVATE_H

#include <QObject>
#include <QMutex>
#include <QSize>

#include "mozilla/embedlite/EmbedLiteWindow.h"

class QMozWindow;

class QMozWindowPrivate : public mozilla::embedlite::EmbedLiteWindowListener
{
public:
    QMozWindowPrivate(QMozWindow &, const QSize &size);
    virtual ~QMozWindowPrivate();

    void setSize(const QSize &size);
    void setContentOrientation(Qt::ScreenOrientation);
    void setPrimaryOrientation(Qt::ScreenOrientation);

    void timerEvent(QTimerEvent *event);

protected:
    // EmbedLiteWindowListener:
    bool RequestGLContext(void *&context, void *&surface, void *&display);
    void WindowInitialized() override;
    void WindowDestroyed() override;
    void DrawOverlay(const nsIntRect &aRect) override;
    bool PreRender() override;
    void CompositorCreated() override;
    void CompositingFinished() override;

private:
    friend class QMozWindow;
    friend class QMozViewPrivate;

    void getEGLContext(void *&context, void *&surface, void *&display);
#if defined(ENABLE_GLX)
    void getGLXContext(void *&context, void *&surface);
#endif
    bool setReadyToPaint(bool ready);

    QMozWindow &q;
    mozilla::embedlite::EmbedLiteWindow *mWindow;
    bool mCompositorCreated;
    QMutex mReadyToPaintMutex;
    bool mReadyToPaint;
    QSize mSize;
    Qt::ScreenOrientation mOrientation;
    Qt::ScreenOrientation mPrimaryOrientation;
    Qt::ScreenOrientation mPendingOrientation;
    int mOrientationFilterTimer;
    bool mReserved;

    Q_DISABLE_COPY(QMozWindowPrivate)
};

#endif // QMOZWINDOW_PRIVATE_H

