/* dtkComposerSceneNodeComposite.cpp --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Fri Feb  3 14:01:41 2012 (+0100)
 * Version: $Id$
 * Last-Updated: Tue Feb  7 12:45:06 2012 (+0100)
 *           By: Julien Wintz
 *     Update #: 186
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#include "dtkComposerSceneEdge.h"
#include "dtkComposerSceneNode.h"
#include "dtkComposerSceneNodeComposite.h"
#include "dtkComposerSceneNote.h"
#include "dtkComposerScenePort.h"

class dtkComposerSceneNodeCompositePrivate
{
public:
    dtkComposerSceneNoteList notes;
    dtkComposerSceneNodeList nodes;
    dtkComposerSceneEdgeList edges;

public:
    QPointF pos;
    QRectF rect;

public:
    bool root;
    bool entered;
};

dtkComposerSceneNodeComposite::dtkComposerSceneNodeComposite(void) : dtkComposerSceneNode(), d(new dtkComposerSceneNodeCompositePrivate)
{
    d->rect = QRectF(0, 0, 150, 50);

    d->root = false;
    d->entered = false;
}

dtkComposerSceneNodeComposite::~dtkComposerSceneNodeComposite(void)
{
    delete d;

    d = NULL;
}

void dtkComposerSceneNodeComposite::addNote(dtkComposerSceneNote *note)
{
    d->notes << note;
}

void dtkComposerSceneNodeComposite::removeNote(dtkComposerSceneNote *note)
{
    d->notes.removeAll(note);
}

void dtkComposerSceneNodeComposite::addNode(dtkComposerSceneNode *node)
{
    d->nodes << node;
}

void dtkComposerSceneNodeComposite::removeNode(dtkComposerSceneNode *node)
{
    d->nodes.removeAll(node);
}

void dtkComposerSceneNodeComposite::addEdge(dtkComposerSceneEdge *edge)
{
    d->edges << edge;
}

void dtkComposerSceneNodeComposite::removeEdge(dtkComposerSceneEdge *edge)
{
    d->edges.removeAll(edge);
}

dtkComposerSceneNoteList dtkComposerSceneNodeComposite::notes(void)
{
    return d->notes;
}

dtkComposerSceneNodeList dtkComposerSceneNodeComposite::nodes(void)
{
    return d->nodes;
}

dtkComposerSceneEdgeList dtkComposerSceneNodeComposite::edges(void)
{
    return d->edges;
}

bool dtkComposerSceneNodeComposite::entered(void)
{
    return d->entered;
}

void dtkComposerSceneNodeComposite::enter(void)
{
    d->entered = true;

    d->pos = this->pos();

    this->setFlag(QGraphicsItem::ItemIsMovable, false);

    this->layout();
}

void dtkComposerSceneNodeComposite::leave(void)
{
    d->entered = false;

    this->setFlag(QGraphicsItem::ItemIsMovable, true);

    this->layout();
}

bool dtkComposerSceneNodeComposite::root(void)
{
    return d->root;
}

void dtkComposerSceneNodeComposite::setRoot(bool root)
{
    d->root = root;
}

#include <float.h>

void dtkComposerSceneNodeComposite::layout(void)
{
// /////////////////////////////////////////////////////////////////
// Rect calculation
// /////////////////////////////////////////////////////////////////

    if(!d->entered) {

        d->rect = QRectF(0, 0, 150, 50);

        if(!d->pos.isNull())
            this->setPos(d->pos);

    } else {

        qreal xmin =  FLT_MAX;
        qreal xmax = -FLT_MAX;

        qreal ymin =  FLT_MAX;
        qreal ymax = -FLT_MAX;

        foreach(dtkComposerSceneNode *node, d->nodes) {
            xmin = qMin(xmin, node->scenePos().x());
            xmax = qMax(xmax, node->scenePos().x() + node->boundingRect().width());
            ymin = qMin(ymin, node->scenePos().y());
            ymax = qMax(ymax, node->scenePos().y() + node->boundingRect().height());
        }

        qreal w_offset = 100;
        qreal h_offset = 100;

        qreal w = xmax-xmin; w+= w_offset;
        qreal h = ymax-ymin; h+= h_offset;

        d->rect = QRectF(0, 0, w, h);

        this->setPos(xmin - w_offset/2, ymin - h_offset/2);
    }

    int port_margin_top = 10;
    int port_margin_bottom = 10;
    int port_margin_left = 10;
    int port_spacing = 10;
    
// /////////////////////////////////////////////////////////////////
// Port location
// /////////////////////////////////////////////////////////////////

    for(int i = 0; i < this->inputPorts().count(); i++)
        this->inputPorts().at(i)->setPos(QPointF(port_margin_left, i*this->inputPorts().at(i)->boundingRect().height() + i*port_spacing + port_margin_top));
    
    for(int i = 0; i < this->outputPorts().count(); i++)
        this->outputPorts().at(i)->setPos(QPointF(d->rect.right() - port_margin_left - this->outputPorts().at(i)->boundingRect().width(), i*this->outputPorts().at(i)->boundingRect().height() + i*port_spacing + port_margin_top));

// /////////////////////////////////////////////////////////////////
// Update edges geometry
// /////////////////////////////////////////////////////////////////

    foreach(dtkComposerSceneEdge *edge, d->edges)
        edge->adjust();
    
// /////////////////////////////////////////////////////////////////
// Height calculation
// /////////////////////////////////////////////////////////////////

    if(!d->entered) {

        if(this->inputPorts().count() || this->outputPorts().count())
            if(this->inputPorts().count() >= this->outputPorts().count())
                d->rect = QRectF(d->rect.topLeft(), QSize(d->rect.width(), this->inputPorts().count() * this->inputPorts().at(0)->boundingRect().height() + port_margin_top + port_margin_bottom + (this->inputPorts().count()-1) * port_spacing));
            else
                d->rect = QRectF(d->rect.topLeft(), QSize(d->rect.width(), this->outputPorts().count() * this->outputPorts().at(0)->boundingRect().height() + port_margin_top + port_margin_bottom + (this->outputPorts().count()-1) * port_spacing));        

    }

    this->update();
}

QRectF dtkComposerSceneNodeComposite::boundingRect(void) const
{
    return d->rect.adjusted(-2, -2, 2, 2);
}

void dtkComposerSceneNodeComposite::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    static qreal radius = 5.0;

    if (this->isSelected()) {
        painter->setPen(QPen(Qt::magenta, 2, Qt::SolidLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawRoundedRect(d->rect.adjusted(-2, -2, 2, 2), radius, radius);
    }

    if(d->entered) {
        painter->setPen(QPen(Qt::black, 1, Qt::DashLine));
        painter->setBrush(Qt::NoBrush);
    } else {
        QLinearGradient gradiant(d->rect.left(), d->rect.top(), d->rect.left(), d->rect.bottom());
        gradiant.setColorAt(0.0, QColor(Qt::darkGray));
        gradiant.setColorAt(0.3, QColor(Qt::darkGray).darker().darker());
        gradiant.setColorAt(1.0, QColor(Qt::darkGray).darker().darker().darker());

        painter->setPen(QPen(Qt::black, 1, Qt::SolidLine));
        painter->setBrush(gradiant);
    }

    painter->drawRoundedRect(d->rect, radius, radius);
}
