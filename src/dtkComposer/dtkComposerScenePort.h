/* dtkComposerScenePort.h --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Fri Feb  3 12:28:22 2012 (+0100)
 * Version: $Id$
 * Last-Updated: jeu. mars 22 15:11:16 2012 (+0100)
 *           By: Nicolas Niclausse
 *     Update #: 47
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef DTKCOMPOSERSCENEPORT_H
#define DTKCOMPOSERSCENEPORT_H

#include <QtGui>

class dtkComposerScenePortPrivate;
class dtkComposerSceneNode;

class dtkComposerScenePort : public QGraphicsItem
{
public:
    enum Type {
        Input,
        Output
    };

public:
     dtkComposerScenePort(Type type, dtkComposerSceneNode *parent);
     dtkComposerScenePort(Type type, const QString& label, dtkComposerSceneNode *parent);
    ~dtkComposerScenePort(void);

public:
    Type type(void);

public:
    dtkComposerSceneNode *node(void);
    dtkComposerSceneNode *owner(void);

public:
    int loop(void);

public:
    QString label(void);

public:
    void setLoop(int loop);

public:
    void setLabel(const QString& label);

public:
    virtual QRectF boundingRect(void) const;

public:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
    dtkComposerScenePortPrivate *d;
};

// /////////////////////////////////////////////////////////////////
// dtkComposerScenePortList
// /////////////////////////////////////////////////////////////////

class dtkComposerScenePortList : public QList<dtkComposerScenePort *> {};

#endif
