/* tstMainWindow.h --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Tue Jun  7 15:30:39 2011 (+0200)
 * Version: $Id$
 * Last-Updated: Sun Jun 10 01:16:43 2012 (+0200)
 *           By: Julien Wintz
 *     Update #: 26
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef TSTMAINWINDOW_H
#define TSTMAINWINDOW_H

#include <QtGui/QMainWindow>

class tstMainWindowPrivate;

class tstMainWindow : public QMainWindow
{
    Q_OBJECT

public:
     tstMainWindow(QWidget *parent = 0);
    ~tstMainWindow(void);

protected slots:
    void onBackgroundColorChanged(const QColor& color);
    void onForegroundColorChanged(const QColor& color);
    void onCurveRenderModeChanged(int mode);
    void onLoad(void);
    void onExport(void);
    void onExport(const QString& file);
    void onActivatePanning(bool);
    void onActivatePicking(bool);
    void onActivateZooming(bool);
    void onZoomForward(void);
    void onZoomBackground(void);

private:
    tstMainWindowPrivate *d;
};

#endif
