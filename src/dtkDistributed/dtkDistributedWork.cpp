/* @(#)dtkDistributedWork.cpp ---
 *
 * Author: Nicolas Niclausse
 * Copyright (C) 2013 - Nicolas Niclausse, Inria.
 * Created: 2013/02/15 14:10:44
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */


#include "dtkDistributedWork.h"
#include <dtkDistributed/dtkDistributedCommunicator.h>
#include <dtkDistributed/dtkDistributedWorker.h>

#include <QtCore>

// /////////////////////////////////////////////////////////////////
// dtkDistributedWorkPrivate
// /////////////////////////////////////////////////////////////////

class dtkDistributedWorkPrivate
{
public:
    dtkDistributedWorker *worker;
};

// /////////////////////////////////////////////////////////////////
// dtkDistributedWork
// /////////////////////////////////////////////////////////////////


dtkDistributedWork::dtkDistributedWork(void) : QObject(), d(new dtkDistributedWorkPrivate)
{
    d->worker = NULL;
}

dtkDistributedWork::~dtkDistributedWork(void)
{
    delete d;

    d = NULL;
}

dtkDistributedWork::dtkDistributedWork(const dtkDistributedWork& other) : QObject(), d(new dtkDistributedWorkPrivate)
{
    d->worker = other.d->worker;

}

dtkDistributedWork& dtkDistributedWork::operator = (const dtkDistributedWork& other)
{
    return *this;

}

dtkDistributedWork *dtkDistributedWork::clone(void)
{
    return new dtkDistributedWork(*this);

}


void dtkDistributedWork::setWorker(dtkDistributedWorker *worker)
{
    d->worker = worker;
}

dtkDistributedWorker *dtkDistributedWork::worker(void)
{
    return d->worker ;
}

void dtkDistributedWork::barrier(void)
{
    d->worker->communicator()->barrier() ;
}

qlonglong dtkDistributedWork::wct(void)
{
    return d->worker->wct() ;
}

qlonglong dtkDistributedWork::wid(void)
{
    return d->worker->wid() ;
}

void dtkDistributedWork::run(void)
{
    qDebug() << "should be implemented in sub classes";
}


