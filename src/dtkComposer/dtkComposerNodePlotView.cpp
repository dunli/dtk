/* dtkComposerNodePlotView.cpp --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Tue May 29 14:40:41 2012 (+0200)
 * Version: $Id$
 * Last-Updated: 2013 Wed Jan  9 08:48:07 (+0100)
 *           By: Thibaud Kloczko
 *     Update #: 81
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#include "dtkComposerNodePlotView.h"
#include "dtkComposerTransmitterEmitter.h"
#include "dtkComposerTransmitterReceiver.h"

#include <dtkPlot/dtkPlotCurve.h>
#include <dtkPlot/dtkPlotView.h>

#include <dtkCore/dtkAbstractView.h>
#include <dtkCore/dtkAbstractViewFactory.h>

class dtkComposerNodePlotViewPrivate
{
public:
    dtkComposerTransmitterReceiver<dtkPlotCurve*> receiver_curve;
    dtkComposerTransmitterReceiver<QString> receiver_x_axis_label;
    dtkComposerTransmitterReceiver<QString> receiver_y_axis_label;

public:
    dtkPlotView *view;
};

dtkComposerNodePlotView::dtkComposerNodePlotView(void) : QObject(), dtkComposerNodeLeafView(), d(new dtkComposerNodePlotViewPrivate)
{
    d->view = NULL;

    this->appendReceiver(&(d->receiver_curve));
    this->appendReceiver(&(d->receiver_x_axis_label));
    this->appendReceiver(&(d->receiver_y_axis_label));

}

dtkComposerNodePlotView::~dtkComposerNodePlotView(void)
{
    if (d->view)
        delete d->view;
    d->view = NULL;

    delete d;

    d = NULL;
}

void dtkComposerNodePlotView::run(void)
{
    if (d->receiver_curve.isEmpty()) {
        dtkWarn() << "no curve speficied!";
        return;
    }

    if (!d->view)
        d->view = reinterpret_cast<dtkPlotView *>(dtkAbstractViewFactory::instance()->create("dtkPlotView"));

    if (!d->view)
        return;

    if (!d->receiver_x_axis_label.isEmpty())
        d->view->setAxisTitleX(d->receiver_x_axis_label.data());

    if (!d->receiver_y_axis_label.isEmpty())
        d->view->setAxisTitleY(d->receiver_y_axis_label.data());

    foreach(dtkPlotCurve *curve, d->receiver_curve.allData())
        (*(d->view)) << curve;

    d->view->update();
}
