/* @(#)dtkComposerGraphNodeBegin.cpp ---
 *
 * Author: Nicolas Niclausse
 * Copyright (C) 2012 - Nicolas Niclausse, Inria.
 * Created: 2012/02/14 13:59:57
 * Version: $Id$
 * Last-Updated: Thu Apr 11 10:25:50 2013 (+0200)
 *           By: Thibaud Kloczko
 *     Update #: 347
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkComposerGraphNodeBegin.h"
#include "dtkComposerGraphNode.h"
#include "dtkComposerNode.h"
#include "dtkComposerNodeControl.h"
#include "dtkComposerNodeComposite.h"
#if defined(DTK_BUILD_DISTRIBUTED)
#include "dtkComposerNodeRemote.h"
#endif

#include <dtkLog/dtkLogger.h>

class dtkComposerGraphNodeBeginPrivate
{
public:
    dtkComposerNodeControl *control_node;
    dtkComposerNodeComposite *composite;

public:
    bool is_remote;

#if defined(DTK_BUILD_DISTRIBUTED)
public:
    dtkComposerNodeRemote *remote;
#endif

public:
    dtkComposerGraphNode *end;
};


dtkComposerGraphNodeBegin::dtkComposerGraphNodeBegin(dtkComposerNode *cnode, const QString& title) : dtkComposerGraphNode(),d(new dtkComposerGraphNodeBeginPrivate)
{
    d->is_remote = false;

    if (!dynamic_cast<dtkComposerNodeControl *>(cnode)) {
#if defined(DTK_BUILD_DISTRIBUTED)
        if (dtkComposerNodeRemote *remote = dynamic_cast<dtkComposerNodeRemote *>(cnode)) {
            d->is_remote = true;
            d->remote = remote ;
            //We can't call isSlave() now
        }
#endif
        d->composite = dynamic_cast<dtkComposerNodeComposite *>(cnode);
        d->control_node = NULL;
    } else {
        d->control_node = dynamic_cast<dtkComposerNodeControl *>(cnode);
        d->composite = NULL;
    }

    this->setTitle(title);
}

dtkComposerGraphNode::Kind dtkComposerGraphNodeBegin::kind(void)
{
    return dtkComposerGraphNode::Begin;
}

dtkComposerNode *dtkComposerGraphNodeBegin::wrapee(void)
{
    if (!d->control_node)
        return d->composite;
    else
        return d->control_node;
}

void dtkComposerGraphNodeBegin::eval(void)
{

    if (!d->control_node  ) {
        if (d->composite)// may be NULL for root node
            d->composite->begin();
    } else
        d->control_node->begin();

    this->setStatus(dtkComposerGraphNode::Done);

    dtkComposerGraphNodeList childs = this->childs();
    dtkComposerGraphNodeList::const_iterator it;

    for (it = childs.constBegin(); it != childs.constEnd(); ++it)
        (*it)->clean();

}

void dtkComposerGraphNodeBegin::setEnd(dtkComposerGraphNode *end)
{
    d->end = end;
}

dtkComposerGraphNodeList dtkComposerGraphNodeBegin::successors(void)
{
#if defined(DTK_BUILD_DISTRIBUTED)
    if (d->is_remote && !d->remote->isSlave()) {
        dtkDebug() << "we are running the begin statement of a remote node on a controller, successor is only the end statement";
        dtkComposerGraphNodeList list;
        list << d->end;
        return list;
    } else {
        return dtkComposerGraphNode::successors();
    }
#else
    return dtkComposerGraphNode::successors();
#endif
}
