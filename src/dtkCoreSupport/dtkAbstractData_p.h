/* dtkAbstractData_p.h --- 
 * 
 * Author: tkloczko
 * Copyright (C) 2011 - Thibaud Kloczko, Inria.
 * Created: Mon Apr 23 12:12:08 2012 (+0200)
 * Version: $Id$
 * Last-Updated: lun. févr.  3 13:41:15 2014 (+0100)
 *           By: Nicolas Niclausse
 *     Update #: 20
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef DTKABSTRACTDATA_P_H
#define DTKABSTRACTDATA_P_H

#include "dtkCoreSupportExport.h"
#include "dtkAbstractObject_p.h"

class dtkAbstractData;

////////////////////////////////////////////////////
// dtkAbstractDataPrivate interface
////////////////////////////////////////////////////

class DTKCORESUPPORT_EXPORT dtkAbstractDataPrivate : public dtkAbstractObjectPrivate
{
public:
    dtkAbstractDataPrivate(dtkAbstractData *q = 0) : dtkAbstractObjectPrivate(q) {}
    dtkAbstractDataPrivate(const dtkAbstractDataPrivate& other) : dtkAbstractObjectPrivate(other), 
                                                                  readers(other.readers),
                                                                  writers(other.writers),
                                                                  converters(other.converters),
                                                                  serializers(other.serializers),
                                                                  deserializers(other.deserializers),
                                                                  path(other.path),
                                                                  paths(other.paths),
                                                                  numberOfChannels(other.numberOfChannels),
                                                                  thumbnails(other.thumbnails) {}

public:
    virtual ~dtkAbstractDataPrivate(void) {}

public:
    QMap<QString, bool> readers;
    QMap<QString, bool> writers;
    QMap<QString, bool> converters;
    QMap<QString, bool> serializers;
    QMap<QString, bool> deserializers;

public:
    QString     path;
    QStringList paths;

public:
    int numberOfChannels;

public:
    QList<QImage> thumbnails;
};

////////////////////////////////////////////////////
// dtkAbstractData protected constructors
////////////////////////////////////////////////////

DTK_IMPLEMENT_PRIVATE(dtkAbstractData, dtkAbstractObject);

#endif