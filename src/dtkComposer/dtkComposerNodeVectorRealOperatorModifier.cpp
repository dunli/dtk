/* dtkComposerNodeVectorRealOperatorModifier.cpp ---
 *
 * Author: sblekout
 * Copyright (C) 2011 - babette lekouta, Inria.
 * Created: Tue May 15 11:35:09 2012 (+0200)
 * Version: $Id$
 * Last-Updated: Tue Jun 26 16:10:16 2012 (+0200)
 *           By: tkloczko
 *     Update #: 35
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkComposerTransmitterEmitter.h"
#include "dtkComposerTransmitterReceiver.h"
#include "dtkComposerNodeVectorRealOperatorModifier.h"

#include <dtkMath>

#include <dtkLog/dtkLog>

// /////////////////////////////////////////////////////////////////
//  dtkComposerNodearrayScalarOperatorBinary
// /////////////////////////////////////////////////////////////////

class dtkComposerNodeVectorRealOperatorModifierPrivate
{
public:
    dtkComposerTransmitterReceiver<dtkVectorReal> receiver_vector;
    dtkComposerTransmitterVariant receiver_value;
    dtkComposerTransmitterVariant receiver_index;

public:
    dtkComposerTransmitterEmitter<dtkVectorReal>  emitter_vector;
};

dtkComposerNodeVectorRealOperatorModifier::dtkComposerNodeVectorRealOperatorModifier(void) : dtkComposerNodeLeaf(), d(new dtkComposerNodeVectorRealOperatorModifierPrivate)
{
    this->appendReceiver(&d->receiver_vector);

    QList<QVariant::Type> variant_list;

    variant_list << QVariant::Int << QVariant::UInt << QVariant::LongLong << QVariant::ULongLong;
    d->receiver_index.setTypes(variant_list);
    this->appendReceiver(&d->receiver_index);

    variant_list << QVariant::Double;
    d->receiver_value.setTypes(variant_list);
    this->appendReceiver(&d->receiver_value);

    this->appendEmitter(&d->emitter_vector);
}

dtkComposerNodeVectorRealOperatorModifier::~dtkComposerNodeVectorRealOperatorModifier(void)
{
    delete d;

    d = NULL;
}


// /////////////////////////////////////////////////////////////////
//  dtkComposerNodeVectorRealOperatorModifierAll
// /////////////////////////////////////////////////////////////////

class dtkComposerNodeVectorRealOperatorModifierAllPrivate
{
public:
    dtkComposerTransmitterReceiver<dtkVectorReal> receiver_vector;
    dtkComposerTransmitterVariant               receiver_value;

public:
    dtkComposerTransmitterEmitter<dtkVectorReal> emitter_vector;
};


dtkComposerNodeVectorRealOperatorModifierAll::dtkComposerNodeVectorRealOperatorModifierAll(void) : dtkComposerNodeLeaf(), d(new dtkComposerNodeVectorRealOperatorModifierAllPrivate)
{
    this->appendReceiver(&d->receiver_vector);

    QList<QVariant::Type> variant_list;

    variant_list << QVariant::Int << QVariant::UInt << QVariant::LongLong << QVariant::ULongLong << QVariant::Double;
    d->receiver_value.setTypes(variant_list);
    this->appendReceiver(&d->receiver_value);

    this->appendEmitter(&d->emitter_vector);
}

dtkComposerNodeVectorRealOperatorModifierAll::~dtkComposerNodeVectorRealOperatorModifierAll(void)
{
    delete d;

    d = NULL;
}

// /////////////////////////////////////////////////////////////////
// dtkComposerNodeVectorRealOperatorModifier - SET
// /////////////////////////////////////////////////////////////////

void dtkComposerNodeVectorRealOperatorModifierSet::run(void)
{
    if (!d->receiver_vector.isEmpty() && !d->receiver_index.isEmpty() && !d->receiver_value.isEmpty() ) {

        dtkVectorReal& vector(d->receiver_vector.data());
        qlonglong index = qvariant_cast<qlonglong>(d->receiver_index.data());
        qreal value = qvariant_cast<qreal>(d->receiver_value.data());

        if (index < vector.getRows()) {
            vector[index]=value ;
            d->emitter_vector.setData(vector);

        } else {
            dtkWarn() << "index > size of the vector. Nothing is done" ;
            d->emitter_vector.setData(dtkVectorReal());
        }

    } else {
        dtkWarn() << "Inputs not specified. Nothing is done";
        d->emitter_vector.setData(dtkVectorReal());
    }
}

// /////////////////////////////////////////////////////////////////
// dtkComposerNodeVectorRealOperatorModifier - Sum
// /////////////////////////////////////////////////////////////////

void dtkComposerNodeVectorRealOperatorModifierSum::run(void)
{
    if (!d->receiver_vector.isEmpty() && !d->receiver_index.isEmpty() && !d->receiver_value.isEmpty() ) {

        dtkVectorReal& vector(d->receiver_vector.data());
        qlonglong index = qvariant_cast<qlonglong>(d->receiver_index.data());
        qreal value = qvariant_cast<qreal>(d->receiver_value.data());

        if (index < vector.getRows()) {
            vector[index] += value ;
            d->emitter_vector.setData(vector);

        } else {
            dtkWarn() << "index > size of the vector. Nothing is done" ;
            d->emitter_vector.setData(dtkVectorReal());
        }

    } else {
        dtkWarn() << "Inputs not specified. Nothing is done";
        d->emitter_vector.setData(dtkVectorReal());
    }
}
// /////////////////////////////////////////////////////////////////
// dtkComposerNodeArrayScalarOperator - Substract
// /////////////////////////////////////////////////////////////////

void dtkComposerNodeVectorRealOperatorModifierSubstract::run(void)
{
    if (!d->receiver_vector.isEmpty() && !d->receiver_index.isEmpty() && !d->receiver_value.isEmpty() ) {

        dtkVectorReal& vector(d->receiver_vector.data());
        qlonglong index = qvariant_cast<qlonglong>(d->receiver_index.data());
        qreal value = qvariant_cast<qreal>(d->receiver_value.data());

        if (index < vector.getRows()) {
            vector[index] -= value ;
            d->emitter_vector.setData(dtkVectorReal());

        } else {
            dtkWarn() << "index > size of the vector. Nothing is done" ;
            d->emitter_vector.setData(dtkVectorReal());
        }

    } else {
        dtkWarn() << "Inputs not specified. Nothing is done";
        d->emitter_vector.setData(dtkVectorReal());
    }
}

// /////////////////////////////////////////////////////////////////
// dtkComposerNodeArrayScalarOperator - Mult
// /////////////////////////////////////////////////////////////////

void dtkComposerNodeVectorRealOperatorModifierMult::run(void)
{
    if (!d->receiver_vector.isEmpty() && !d->receiver_index.isEmpty() && !d->receiver_value.isEmpty() ) {

        dtkVectorReal& vector(d->receiver_vector.data());
        qlonglong index = qvariant_cast<qlonglong>(d->receiver_index.data());
        qreal value = qvariant_cast<qreal>(d->receiver_value.data());

        if (index < vector.getRows()) {
            vector[index] *= value ;
            d->emitter_vector.setData(vector);

        } else {
            dtkWarn() << "index > size of the vector. Nothing is done" ;
            d->emitter_vector.setData(dtkVectorReal());
        }

    } else {
        dtkWarn() << "Inputs not specified. Nothing is done";
        d->emitter_vector.setData(dtkVectorReal());
    }
}

// /////////////////////////////////////////////////////////////////
// dtkComposerNodeArrayScalarOperator - Divide
// /////////////////////////////////////////////////////////////////

void dtkComposerNodeVectorRealOperatorModifierDivide::run(void)
{
    if (!d->receiver_vector.isEmpty() && !d->receiver_index.isEmpty() && !d->receiver_value.isEmpty() ) {

        dtkVectorReal& vector(d->receiver_vector.data());
        qlonglong index = qvariant_cast<qlonglong>(d->receiver_index.data());
        qreal value = qvariant_cast<qreal>(d->receiver_value.data());

        if (index < vector.getRows()) {
            if (value == 0)
                dtkWarn() << "Value is zero. Nothing is done" ;

            else
                vector[index]/= value ;

            d->emitter_vector.setData(vector);

        } else {
            dtkWarn() << "index > size of the vector. Nothing is done" ;
            dtkVectorReal vec;
            d->emitter_vector.setData(vec);
        }

    } else {
        dtkWarn() << "Inputs not specified. Nothing is done";
        dtkVectorReal vec;
        d->emitter_vector.setData(vec);
    }

}

// /////////////////////////////////////////////////////////////////
// dtkComposerNodeVectorRealOperatorModifierAll - Add
// /////////////////////////////////////////////////////////////////

void dtkComposerNodeVectorRealOperatorModifierAllAdd::run(void)
{
    if (!d->receiver_vector.isEmpty() && !d->receiver_value.isEmpty() ) {
        dtkVectorReal& vector(d->receiver_vector.data());
        qreal value = qvariant_cast<qreal>(d->receiver_value.data());

        for (int i = 0 ; i< vector.getRows(); ++i)
            vector[i] += value ;

        d->emitter_vector.setData( vector);

    } else {

        dtkWarn() << "Inputs not specified. Nothing is done";
        dtkVectorReal vec;
        d->emitter_vector.setData(vec);

    }
}

// /////////////////////////////////////////////////////////////////
// dtkComposerNodeVectorRealOperatorModifierAll - Substract
// /////////////////////////////////////////////////////////////////

void dtkComposerNodeVectorRealOperatorModifierAllSubstract::run(void)
{
    if (!d->receiver_vector.isEmpty() && !d->receiver_value.isEmpty() ) {
        dtkVectorReal& vector(d->receiver_vector.data());
        qreal value = qvariant_cast<qreal>(d->receiver_value.data());

        for (int i = 0 ; i< vector.getRows(); ++i)
            vector[i] -= value ;

        d->emitter_vector.setData( vector);

    } else {

        dtkWarn() << "Inputs not specified. Nothing is done";
        dtkVectorReal vec;
        d->emitter_vector.setData(vec);

    }
}

// /////////////////////////////////////////////////////////////////
// dtkComposerNodeVectorRealOperatorModifierAll - Mult
// /////////////////////////////////////////////////////////////////

void dtkComposerNodeVectorRealOperatorModifierAllMult::run(void)
{
    if (!d->receiver_vector.isEmpty() && !d->receiver_value.isEmpty() ) {
        dtkVectorReal& vector(d->receiver_vector.data());
        qreal value = qvariant_cast<qreal>(d->receiver_value.data());

        for (int i = 0 ; i< vector.getRows(); ++i)
            vector[i] *= value ;

        d->emitter_vector.setData( vector);

    } else {

        dtkWarn() << "Inputs not specified. Nothing is done";
        dtkVectorReal vec;
        d->emitter_vector.setData(vec);

    }
}

// /////////////////////////////////////////////////////////////////
// dtkComposerNodeVectorRealOperatorModifierAll - Divide
// /////////////////////////////////////////////////////////////////

void dtkComposerNodeVectorRealOperatorModifierAllDivide::run(void)
{
    if (!d->receiver_vector.isEmpty() && !d->receiver_value.isEmpty() ) {

        dtkVectorReal& vector(d->receiver_vector.data());
        qreal value = qvariant_cast<qreal>(d->receiver_value.data());

        if (value == 0)
            dtkWarn() << "Value is zero. Nothing is done" ;

        else {
            for (int i = 0 ; i< vector.getRows(); ++i)
                vector[i] /= value ;

            d->emitter_vector.setData(vector);
        }

    } else {

        dtkWarn() << "Inputs not specified. Nothing is done";
        dtkVectorReal vec;
        d->emitter_vector.setData(vec);
    }
}
