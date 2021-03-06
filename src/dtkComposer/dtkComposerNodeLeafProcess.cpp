/* dtkComposerNodeLeafProcess.cpp --- 
 * 
 * Author: tkloczko
 * Copyright (C) 2011 - Thibaud Kloczko, Inria.
 * Created: Thu Jun 28 14:08:54 2012 (+0200)
 * Version: $Id$
 * Last-Updated: 2012 Fri Nov 16 16:18:14 (+0100)
 *           By: Thibaud Kloczko, Inria.
 *     Update #: 9
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#include "dtkComposerNodeLeafProcess.h"

#include <dtkCore/dtkAbstractProcess.h>
#include <dtkCore/dtkAbstractProcessFactory.h>

// /////////////////////////////////////////////////////////////////
// dtkComposerNodeLeafProcessPrivate interface
// /////////////////////////////////////////////////////////////////

class dtkComposerNodeLeafProcessPrivate
{
public:
    dtkAbstractProcess *process;

    bool implementation_has_changed;
};

// /////////////////////////////////////////////////////////////////
// dtkComposerNodeLeafProcess implementation
// /////////////////////////////////////////////////////////////////

dtkComposerNodeLeafProcess::dtkComposerNodeLeafProcess(void) : dtkComposerNodeLeaf(), d(new dtkComposerNodeLeafProcessPrivate)
{
    d->process = NULL;
    d->implementation_has_changed = false;
}

dtkComposerNodeLeafProcess::~dtkComposerNodeLeafProcess(void)
{
    if (d->process)
        delete d->process;

    d->process = NULL;

    delete d;

    d = NULL;
}

bool dtkComposerNodeLeafProcess::enableDefaultImplementation(void) const
{
    return false;
}

bool dtkComposerNodeLeafProcess::implementationHasChanged(void) const
{
    return d->implementation_has_changed;
}

QString dtkComposerNodeLeafProcess::currentImplementation(void) const
{
    if (d->process)
        return d->process->identifier();

    return QString();
}

QStringList dtkComposerNodeLeafProcess::implementations(void)
{
    QStringList implementations;
    if (this->enableDefaultImplementation())
        implementations << "default";
    QStringList all_implementations = dtkAbstractProcessFactory::instance()->implementations(this->abstractProcessType());

    for (int i = 0; i < all_implementations.count(); ++i)
        implementations << all_implementations.at(i);

    return implementations;
}

dtkAbstractProcess *dtkComposerNodeLeafProcess::createProcess(const QString& implementation)
{
    d->implementation_has_changed = false;

    if (implementation.isEmpty() || implementation == "Choose implementation")
        return NULL;
    
    if (implementation == "default")
        const_cast<QString&>(implementation) = this->abstractProcessType();
    
    if (!d->process) {

        d->process = dtkAbstractProcessFactory::instance()->create(implementation);

        d->implementation_has_changed = true;

    } else if (d->process->identifier() != implementation) {

        delete d->process;

        d->process = dtkAbstractProcessFactory::instance()->create(implementation);

        d->implementation_has_changed = true;

    }        

    return d->process;
}

dtkAbstractProcess *dtkComposerNodeLeafProcess::process(void)
{
    return d->process;
}
