/* dtkPlotViewToolBar.h --- 
 * 
 * Author: Selim Kraria
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Fri Jun  8 12:52:22 2012 (+0200)
 * Version: $Id$
 * Last-Updated: mer. avril  9 13:25:05 2014 (+0200)
 *           By: Nicolas Niclausse
 *     Update #: 81
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef DTKPLOTVIEWTOOLBAR_H
#define DTKPLOTVIEWTOOLBAR_H

#include "dtkPlotSupportExport.h"

#include <QFrame>

class dtkPlotView;
class dtkPlotViewToolBarPrivate;

class DTKPLOTSUPPORT_EXPORT dtkPlotViewToolBar : public QFrame
{
    Q_OBJECT

public:
     dtkPlotViewToolBar(dtkPlotView *parent);
    ~dtkPlotViewToolBar(void);

protected:
    void onExport(const QString& file);

signals:
    void settingsClicked(bool value);

public:
    void setDark(bool dark);

protected slots:
    void onDefaultView(void);
    void onExport(void);
    void onGridActivated(bool value);
    void onLegendActivated(bool value);
    void onPickingActivated(bool value);
    void onZoomActivated(bool value);

private:
    dtkPlotViewToolBarPrivate *d;
};

#endif
