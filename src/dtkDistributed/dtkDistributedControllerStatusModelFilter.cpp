/* dtkDistributedControllerStatusModelFilter.cpp --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Fri Apr  6 15:53:04 2012 (+0200)
 * Version: $Id$
 * Last-Updated: Tue Apr 10 22:16:18 2012 (+0200)
 *           By: Julien Wintz
 *     Update #: 76
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#include "dtkDistributedControllerStatusModelFilter.h"

class dtkDistributedControllerStatusModelFilterPrivate
{
public:
    QFlags<dtkDistributedNode::Network> network_flags;
    QFlags<dtkDistributedNode::State> state_flags;
    QFlags<dtkDistributedNode::Brand> brand_flags;
    QFlags<dtkDistributedCpu::Architecture> arch_flags;
    QFlags<dtkDistributedCpu::Model> model_flags;
};

dtkDistributedControllerStatusModelFilter::dtkDistributedControllerStatusModelFilter(QObject *parent) : QSortFilterProxyModel(parent), d(new dtkDistributedControllerStatusModelFilterPrivate)
{
    this->setDynamicSortFilter(true);
}

dtkDistributedControllerStatusModelFilter::~dtkDistributedControllerStatusModelFilter(void)
{
    delete d;

    d = NULL;
}

void dtkDistributedControllerStatusModelFilter::setNetworkFlags(QFlags<dtkDistributedNode::Network> flags)
{
    d->network_flags = flags;

    this->invalidateFilter();
}

void dtkDistributedControllerStatusModelFilter::setStateFlags(QFlags<dtkDistributedNode::State> flags)
{
    d->state_flags = flags;

    this->invalidateFilter();
}

void dtkDistributedControllerStatusModelFilter::setBrandFlags(QFlags<dtkDistributedNode::Brand> flags)
{
    d->brand_flags = flags;

    this->invalidateFilter();
}

void dtkDistributedControllerStatusModelFilter::setArchFlags(QFlags<dtkDistributedCpu::Architecture> flags)
{
    d->arch_flags = flags;

    this->invalidateFilter();
}

void dtkDistributedControllerStatusModelFilter::setModelFlags(QFlags<dtkDistributedCpu::Model> flags)
{
    d->model_flags = flags;

    this->invalidateFilter();
}

bool dtkDistributedControllerStatusModelFilter::filterAcceptsRow(int row, const QModelIndex& parent) const
{   
    QModelIndex n_index = sourceModel()->index(row, 1, parent);

    if((int)(d->network_flags)) {

        if(sourceModel()->data(n_index).toString() == "Ethernet1G" && !d->network_flags.testFlag(dtkDistributedNode::Ethernet1G))
            return false;

       if(sourceModel()->data(n_index).toString() == "Ethernet10G" && !d->network_flags.testFlag(dtkDistributedNode::Ethernet10G))
            return false;

       if(sourceModel()->data(n_index).toString() == "Myrinet2G" && !d->network_flags.testFlag(dtkDistributedNode::Myrinet2G))
            return false;

       if(sourceModel()->data(n_index).toString() == "Myrinet10G" && !d->network_flags.testFlag(dtkDistributedNode::Myrinet10G))
            return false;

       if(sourceModel()->data(n_index).toString() == "Infiniband10G" && !d->network_flags.testFlag(dtkDistributedNode::Infiniband10G))
            return false;

       if(sourceModel()->data(n_index).toString() == "Infiniband20G" && !d->network_flags.testFlag(dtkDistributedNode::Infiniband20G))
            return false;

        if(sourceModel()->data(n_index).toString() == "Infiniband40G" && !d->network_flags.testFlag(dtkDistributedNode::Infiniband40G))
            return false;
    }

    return true;
}
