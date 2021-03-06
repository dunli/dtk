/* dtkComposerPath.h --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Mon Nov 19 11:40:26 2012 (+0100)
 * Version: $Id$
 * Last-Updated: Thu Apr 11 10:37:36 2013 (+0200)
 *           By: Thibaud Kloczko
 *     Update #: 19
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#pragma once

#include <QtCore>
#include <QtGui>

class dtkComposerPathPrivate;
class dtkComposerScene;

class  dtkComposerPath : public QFrame
{
    Q_OBJECT

public:
     dtkComposerPath(QWidget *parent = 0);
    ~dtkComposerPath(void);

public:
    void setScene(dtkComposerScene *scene);

public:
    QSize sizeHint(void) const;

public slots:
    void update(void);

private:
    dtkComposerPathPrivate *d;
};

