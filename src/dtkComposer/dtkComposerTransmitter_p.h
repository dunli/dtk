/* dtkComposerTransmitter_p.h --- 
 * 
 * Author: tkloczko
 * Copyright (C) 2011 - Thibaud Kloczko, Inria.
 * Created: Mon Mar 12 11:43:28 2012 (+0100)
 * Version: $Id$
 * Last-Updated: Wed Aug  1 09:16:57 2012 (+0200)
 *           By: tkloczko
 *     Update #: 35
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef DTKCOMPOSERTRANSMITTER_P_H
#define DTKCOMPOSERTRANSMITTER_P_H

#include "dtkComposerTransmitter.h"

#include <QtCore>

#include <dtkContainer/dtkAbstractContainerWrapper.h>

class dtkComposerNode;

// /////////////////////////////////////////////////////////////////
// dtkComposerTransmitterPrivate declaration
// /////////////////////////////////////////////////////////////////

class dtkComposerTransmitterPrivate
{
public:
    bool active;
    bool required;

public:
    dtkComposerNode *parent;

public:
    QList<dtkComposerTransmitter *>     next_list;
    QList<dtkComposerTransmitter *> previous_list;

public:
    QList<dtkComposerTransmitter *> receivers;

public:
    QVariant variant;

public:
    dtkAbstractContainerWrapper container;

public:
    dtkComposerTransmitter::DataTransmission data_transmission;
};

#endif
