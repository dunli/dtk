/* dtkAbstractDataSerializer.cpp ---
 *
 * Author: Nicolas Niclausse
 * Copyright (C) 2011 - Nicolas Niclausse, Inria.
 * Created: lun. oct. 17 13:20:01 2011 (+0200)
 * Version: $Id$
 * Last-Updated: Tue Apr  3 15:56:03 2012 (+0200)
 *           By: tkloczko
 *     Update #: 19
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkAbstractDataSerializer.h"

class dtkAbstractDataSerializerPrivate
{
public:
    bool enabled;

    QByteArray *binary;
};

dtkAbstractDataSerializer::dtkAbstractDataSerializer(void) : dtkAbstractObject(), d(new dtkAbstractDataSerializerPrivate)
{
    d->enabled = false;
}

dtkAbstractDataSerializer::~dtkAbstractDataSerializer(void)
{
    delete d;

    d = NULL;
}

bool dtkAbstractDataSerializer::enabled(void) const
{
    return d->enabled;
}

void dtkAbstractDataSerializer::enable(void)
{
    d->enabled = true;
}

void dtkAbstractDataSerializer::disable(void)
{
    d->enabled = false;
}

QByteArray *dtkAbstractDataSerializer::data(void)
{
    return d->binary;
}

void dtkAbstractDataSerializer::setData(QByteArray *array)
{
    d->binary = array;
}

QByteArray *dtkAbstractDataSerializer::serialize(void)
{
    return NULL;
}

QByteArray *dtkAbstractDataSerializer::serialize(dtkAbstractData *data)
{
    DTK_UNUSED(data);
    return NULL;
}

void dtkAbstractDataSerializer::setProgress(int value)
{
    emit progressed (value);
}

