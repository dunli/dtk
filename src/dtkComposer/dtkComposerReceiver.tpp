/* dtkComposerReceiver.tpp --- 
 * 
 * Author: tkloczko
 * Copyright (C) 2011 - Thibaud Kloczko, Inria.
 * Created: Tue Feb 14 12:56:04 2012 (+0100)
 * Version: $Id$
 * Last-Updated: Tue Feb 21 13:47:22 2012 (+0100)
 *           By: tkloczko
 *     Update #: 47
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef DTKCOMPOSERRECEIVER_TPP
#define DTKCOMPOSERRECEIVER_TPP

#include "dtkComposerEmitter.h"

// /////////////////////////////////////////////////////////////////
// dtkComposerReceiver implementation
// /////////////////////////////////////////////////////////////////

//! Constructs an empty receiver.
/*! 
 *  
 */
template <typename T> inline dtkComposerReceiver<T>::dtkComposerReceiver(dtkComposerNode *parent) : dtkComposerTransmitter(parent)
{
    
};

//! Destroys the receiver.
/*! 
 *  
 */
template <typename T> inline dtkComposerReceiver<T>::~dtkComposerReceiver(void)
{

};

//! Returns the data as a modifiable reference.
/*! 
 *  
 */
template <typename T> inline T& dtkComposerReceiver<T>::data(void)
{
    foreach(dtkComposerEmitter<T> *emitter, emitters)
        if (emitter->active())
            return emitter->data();
};

//! Returns the data as a non-modifiable reference.
/*! 
 *  
 */ 
template <typename T> inline const T& dtkComposerReceiver<T>::data(void) const 
{
    foreach(dtkComposerEmitter<T> *emitter, emitters)
        if (emitter->active())
            return emitter->data();
};

//! Returns description of the receiver.
/*! 
 *  
 */
template <typename T> bool dtkComposerReceiver<T>::isEmpty(void) const
{
    if (emitters.isEmpty())
        return true;

    return false;
};

//! Returns description of the receiver.
/*! 
 *  
 */
template <typename T> QString dtkComposerReceiver<T>::identifier(void) const
{
    return "dtkComposerReceiver";
};

//! 
/*! 
 *  
 */
template <typename T> bool dtkComposerReceiver<T>::connect(dtkComposerTransmitter *transmitter)
{
    dtkComposerEmitter<T> *emitter = NULL;

    if (emitter = dynamic_cast<dtkComposerEmitter<T> *>(transmitter)) {
        if (!emitters.contains(emitter)) {
            emitters << emitter;
            return true;
        }
    }

    return false;
};

//! 
/*! 
 *  
 */
template <typename T> bool dtkComposerReceiver<T>::disconnect(dtkComposerTransmitter *transmitter)
{
    dtkComposerEmitter<T> *emitter = NULL;

    if (emitter = dynamic_cast<dtkComposerEmitter<T> *>(transmitter)) {
        return emitters.removeOne(emitter);
    }

    return false;
};

//! 
/*! 
 *  All links from every receiver found to \a transmitter are stored
 *  in a multi-hash table.
 *
 *  By default, an empty multi-hash is returned.
 */
template <typename T> dtkComposerTransmitter::LinkMap dtkComposerReceiver<T>::rightLinks(dtkComposerTransmitter *transmitter, dtkComposerTransmitterLinkList list)
{
    LinkMap link_map;
    foreach(dtkComposerTransmitterLink *l, list)
        link_map.insert(this, l);

    return link_map;
};

#endif
