/* dtkComposerTransmitterEmitter.h --- 
 * 
 * Author: tkloczko
 * Copyright (C) 2011 - Thibaud Kloczko, Inria.
 * Created: Tue Feb 14 10:33:49 2012 (+0100)
 * Version: $Id$
 * Last-Updated: Wed Aug  1 12:43:30 2012 (+0200)
 *           By: tkloczko
 *     Update #: 112
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef DTKCOMPOSERTRANSMITTEREMITTER_H
#define DTKCOMPOSERTRANSMITTEREMITTER_H

#include "dtkComposerExport.h"
#include "dtkComposerTransmitter.h"

#include <dtkCore/dtkGlobal>

#include <dtkContainer/dtkContainerVector.h>

// /////////////////////////////////////////////////////////////////
// dtkComposerTransmitterEmitter interface
// /////////////////////////////////////////////////////////////////

template <typename T> class DTKCOMPOSER_EXPORT dtkComposerTransmitterEmitter : public dtkComposerTransmitter
{
public:
     dtkComposerTransmitterEmitter(dtkComposerNode *parent = 0);
    ~dtkComposerTransmitterEmitter(void);

public:
    inline void setData(const T& data);

    inline T& data(void);

public:
    virtual Kind kind(void) const;

    virtual QString kindName(void) const;

public:
    LinkMap leftLinks(dtkComposerTransmitter *transmitter, dtkComposerTransmitterLinkList list);

private:
    T m_data;
};

// /////////////////////////////////////////////////////////////////
// dtkComposerTransmitterEmitterVector interface
// /////////////////////////////////////////////////////////////////

template <typename T> class DTKCOMPOSER_EXPORT dtkComposerTransmitterEmitterVector : public dtkComposerTransmitterEmitter<T>
{
public:
     dtkComposerTransmitterEmitterVector(dtkComposerNode *parent = 0);
    ~dtkComposerTransmitterEmitterVector(void);

public:
    dtkComposerTransmitter::Kind kind(void) const;

    QString kindName(void) const;

public:
    inline void setData(const dtkContainerVector<T>& vector);

    inline dtkContainerVector<T>& data(void);

private:
    dtkContainerVector<T> m_vector;

    using dtkComposerTransmitterEmitter<T>::d;
};

// /////////////////////////////////////////////////////////////////
// dtkComposerTransmitterEmitter implementation
// /////////////////////////////////////////////////////////////////

#include "dtkComposerTransmitterEmitter.tpp"

#endif
