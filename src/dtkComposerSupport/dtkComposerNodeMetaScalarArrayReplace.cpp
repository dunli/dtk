// dtkComposerNodeMetaScalarArrayReplace.cpp --- 
// 
// Author: Régis Duvigneau
// Copyright (C) 2008 - Regis Duvigneau, Inria.
// Created: Thu Jul 19 10:28:54 2012 (+0200)
// Version: $Id$
// Last-Updated: mar. févr.  4 17:40:55 2014 (+0100)
//           By: Nicolas Niclausse
//     Update #: 26
// 

// Commentary: 
// 
// 

// Change log:
// 
// 

#include "dtkComposerNodeMetaScalarArrayReplace.h"

#include <dtkComposerTransmitterEmitter.h>
#include <dtkComposerTransmitterReceiver.h>

#include <dtkContainerSupport/dtkAbstractContainerWrapper.h>
#include <dtkContainerSupport/dtkContainerVector.h>

#include <dtkMathSupport/dtkVector.h>

#include <dtkLog/dtkLog>

// /////////////////////////////////////////////////////////////////
// dtkComposerNodeMetaScalarArrayReplacePrivate interface
// /////////////////////////////////////////////////////////////////

class dtkComposerNodeMetaScalarArrayReplacePrivate
{
public:
    dtkComposerTransmitterReceiverVector< dtkContainerVector<qreal> *> receiver_arrays;
    dtkComposerTransmitterReceiver< dtkVector<qreal> > receiver_vector;
    dtkComposerTransmitterReceiver<qlonglong> receiver_index;

    dtkComposerTransmitterEmitterVector< dtkContainerVector<qreal> *> emitter_arrays;

    dtkContainerVector<dtkContainerVector<qreal> *> *arrays;
};

// /////////////////////////////////////////////////////////////////
// dtkComposerNodeMetaScalarArrayReplace implementation
// /////////////////////////////////////////////////////////////////

dtkComposerNodeMetaScalarArrayReplace::dtkComposerNodeMetaScalarArrayReplace(void) : dtkComposerNodeLeaf(), d(new dtkComposerNodeMetaScalarArrayReplacePrivate)
{
    this->appendReceiver(&d->receiver_arrays);
    this->appendReceiver(&d->receiver_vector);
    this->appendReceiver(&d->receiver_index);

    this->appendEmitter(&d->emitter_arrays);
}

dtkComposerNodeMetaScalarArrayReplace::~dtkComposerNodeMetaScalarArrayReplace(void)
{
    delete d;
    
    d = NULL;
}

void dtkComposerNodeMetaScalarArrayReplace::run(void)
{
    if (d->receiver_arrays.isEmpty())
        return;

    if (d->receiver_vector.isEmpty())
        return;

    if (d->receiver_index.isEmpty())
        return;

    d->arrays = d->receiver_arrays.data();

    dtkxarch_int size = d->arrays->count();
    qlonglong index = (*(d->receiver_index.data()));

    for (qlonglong i=0; i<size; i++)
        (*d->arrays->at(i))[index] = (*(d->receiver_vector.data()))[i];
    
    d->emitter_arrays.setData(d->arrays);
}
