/* dtk3DView.cpp ---
 * 
 * Author: Julien Wintz
 * Created: Fri Mar 22 09:41:43 2013 (+0100)
 * Version: 
 * Last-Updated: Fri Mar 22 14:35:39 2013 (+0100)
 *           By: Julien Wintz
 *     Update #: 30
 */

/* Change Log:
 * 
 */

#include "dtk3DView.h"
#include "dtk3DScene.h"

#include <QtGui>

class dtk3DViewPrivate
{
public:
    dtk3DScene *scene;
};

dtk3DView::dtk3DView(QWindow *parent) : QGLView(parent), d(new dtk3DViewPrivate)
{
    d->scene = NULL;

    this->setOption(QGLView::ObjectPicking, true);
}

dtk3DView::~dtk3DView(void)
{
    delete d;

    d = NULL;
}

void dtk3DView::setScene(dtk3DScene *scene)
{
    d->scene = scene;
}

void dtk3DView::initializeGL(QGLPainter *painter)
{
    if (d->scene)
	d->scene->initialize(this, painter);

    QGLView::initializeGL(painter);
}

void dtk3DView::paintGL(QGLPainter *painter)
{
    if (d->scene)
	d->scene->paint(this, painter);

    QGLView::initializeGL(painter);
}

void dtk3DView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Tab) {
        this->setOption(QGLView::ShowPicking, ((this->options() & QGLView::ShowPicking) == 0));
        this->update();
    }

    QGLView::keyPressEvent(event);
}
