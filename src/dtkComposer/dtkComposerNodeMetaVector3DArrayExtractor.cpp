// dtkComposerNodeMetaVector3DArrayExtractor.cpp --- 
// 
// Author: Jeremie Labroquere
// Copyright (C) 2011 - Jeremie Labroquere, Inria.
// Created: Mon Aug  6 15:38:25 2012 (+0200)
// Version: $Id$
// Last-Updated: 2013 Wed Jan 16 14:00:31 (+0100)
//           By: Thibaud Kloczko
//     Update #: 12
// 

// Commentary: 
// 
// 

// Change log:
// 
// 


#include "dtkComposerNodeMetaVector3DArrayExtractor.h"

#include <dtkComposer/dtkComposerTransmitterEmitter.h>
#include <dtkComposer/dtkComposerTransmitterReceiver.h>

#include <dtkContainer/dtkAbstractContainerWrapper.h>
#include <dtkContainer/dtkContainerVector.h>

#include <dtkMath/dtkVector3D.h>

#include <dtkMath/dtkVector.h>

#include <dtkLog/dtkLog>

// /////////////////////////////////////////////////////////////////
// dtkComposerNodeMetaVector3DArrayExtractorPrivate interface
// /////////////////////////////////////////////////////////////////

class dtkComposerNodeMetaVector3DArrayExtractorPrivate
{
public:
    dtkComposerTransmitterReceiverVector< dtkContainerVector<dtkVector3DReal*> *> receiver_arrays;
    dtkComposerTransmitterReceiver<qlonglong> receiver_index;

    dtkComposerTransmitterEmitterVector<dtkVector3DReal*> emitter_array;

    dtkContainerVector<dtkVector3DReal*> *array;
};

// /////////////////////////////////////////////////////////////////
// dtkComposerNodeMetaVector3DArrayExtractor implementation
// /////////////////////////////////////////////////////////////////

dtkComposerNodeMetaVector3DArrayExtractor::dtkComposerNodeMetaVector3DArrayExtractor(void) : dtkComposerNodeLeaf(), d(new dtkComposerNodeMetaVector3DArrayExtractorPrivate)
{
    this->appendReceiver(&d->receiver_arrays);
    this->appendReceiver(&d->receiver_index);

    this->appendEmitter(&d->emitter_array);
}

dtkComposerNodeMetaVector3DArrayExtractor::~dtkComposerNodeMetaVector3DArrayExtractor(void)
{
    delete d;
    
    d = NULL;
}

void dtkComposerNodeMetaVector3DArrayExtractor::run(void)
{
    if (d->receiver_arrays.isEmpty())
        return;

    if (d->receiver_index.isEmpty())
        return;

    qlonglong index = d->receiver_index.data();

    d->array = (*(d->receiver_arrays.data()))[index];

    d->emitter_array.setData(d->array);
}