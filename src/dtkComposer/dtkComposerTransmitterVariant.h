/* dtkComposerTransmitterVariant.h --- 
 * 
 * Author: tkloczko
 * Copyright (C) 2011 - Thibaud Kloczko, Inria.
 * Created: Fri Mar  2 16:19:20 2012 (+0100)
 * Version: $Id$
 * Last-Updated: Wed Jun 27 15:57:07 2012 (+0200)
 *           By: tkloczko
 *     Update #: 101
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef DTKCOMPOSERTRANSMITTERVARIANT_H
#define DTKCOMPOSERTRANSMITTERVARIANT_H

#include "dtkComposerExport.h"
#include "dtkComposerTransmitter.h"

#include <dtkCore/dtkGlobal>

// /////////////////////////////////////////////////////////////////
// dtkComposerTransmitterVariant declaration
// /////////////////////////////////////////////////////////////////

class dtkComposerTransmitterVariantPrivate;
class dtkDistributedMessage;

class DTKCOMPOSER_EXPORT dtkComposerTransmitterVariant : public dtkComposerTransmitter
{
public:
     dtkComposerTransmitterVariant(dtkComposerNode *parent = 0);
    ~dtkComposerTransmitterVariant(void);

public:
    void setData(const QVariant& data);
    void setData(const dtkAbstractContainerWrapper& data);
    void setDataFromMsg(dtkDistributedMessage *msg);

          QVariant& data(void);
    const QVariant& data(void) const;

          dtkAbstractContainerWrapper& container(void);
    const dtkAbstractContainerWrapper& container(void) const;

    QVariantList allData(void);

public:
    bool isEmpty(void) const;

public:
    void setTwin(dtkComposerTransmitterVariant *twin);

    dtkComposerTransmitterVariant *twin(void);
    
    void setTwinned(bool twinned);

public:
    virtual Kind kind(void) const;

    virtual QString kindName(void) const;

public:
    QVariant::Type type(void) const;

public:
    void setTypes(QList<QVariant::Type> types);

    QList<QVariant::Type> types(void);

public:
    bool    connect(dtkComposerTransmitter *transmitter);
    bool disconnect(dtkComposerTransmitter *transmitter);

public:
    void setActiveEmitter(dtkComposerTransmitter *emitter);

public:
    LinkMap  leftLinks(dtkComposerTransmitter *transmitter, dtkComposerTransmitterLinkList list);
    LinkMap rightLinks(dtkComposerTransmitter *transmitter, dtkComposerTransmitterLinkList list);

protected:
    dtkComposerTransmitterVariantPrivate *e;
};

#endif
