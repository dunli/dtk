/* dtkDistributor.h --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Sun Mar 21 19:01:19 2010 (+0100)
 * Version: $Id$
 * Last-Updated: Thu Mar 25 11:12:33 2010 (+0100)
 *           By: Julien Wintz
 *     Update #: 9
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef DTKDISTRIBUTOR_H
#define DTKDISTRIBUTOR_H

#include <QtGui>

class dtkDistributorPrivate;

class dtkDistributor : public QWidget
{
    Q_OBJECT

public:
     dtkDistributor(QWidget *parent = 0);
    ~dtkDistributor(void);

protected slots:
    void clear(void);
    void discover(void);
    void update(void);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    dtkDistributorPrivate *d;
};

#endif
