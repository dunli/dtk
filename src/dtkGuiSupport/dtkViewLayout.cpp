/* dtkViewLayout.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Wed May 16 09:38:22 2012 (+0200)
 * Version: $Id$
 * Last-Updated: Thu Jan 16 16:39:20 2014 (+0100)
 *           By: Selim Kraria
 *     Update #: 22
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkViewLayout.h"
#include "dtkViewLayoutItem.h"

#include <dtkCoreSupport/dtkAbstractViewFactory.h>

class dtkViewLayoutPrivate
{
public:
    dtkViewLayoutItem *root;
    dtkViewLayoutItem *current;
};

dtkViewLayout::dtkViewLayout(QWidget *parent) : QFrame(parent), d(new dtkViewLayoutPrivate)
{
    d->root = new dtkViewLayoutItem(0);
    d->root->setParent(this);
    d->root->setLayout(this);

    d->current = d->root;

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->root);

    connect(d->root, SIGNAL(focused(dtkAbstractView *)), this, SIGNAL(focused(dtkAbstractView *)));
    connect(d->root, SIGNAL(unfocused(dtkAbstractView *)), this, SIGNAL(unfocused(dtkAbstractView *)));

    connect(dtkAbstractViewFactory::instance(), SIGNAL(cleared()), this, SLOT(clear()));
}

dtkViewLayout::~dtkViewLayout(void)
{
    delete d;

    d = NULL;
}

dtkViewLayoutItem *dtkViewLayout::root(void)
{
    return d->root;
}

dtkViewLayoutItem *dtkViewLayout::current(void)
{
    return d->current;
}

void dtkViewLayout::setCurrent(dtkViewLayoutItem *item)
{
    d->current = item;
}

void dtkViewLayout::clear(void)
{
    d->root->clear();
}
