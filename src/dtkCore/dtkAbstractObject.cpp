/* dtkAbstractObject.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Sat Feb 28 17:54:04 2009 (+0100)
 * Version: $Id$
 * Last-Updated: Wed May 30 13:00:58 2012 (+0200)
 *           By: tkloczko
 *     Update #: 225
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkAbstractObject.h"
#include "dtkAbstractObject_p.h"

#include <dtkLog/dtkLog.h>

// /////////////////////////////////////////////////////////////////
// dtkAbstractObject implementation
// /////////////////////////////////////////////////////////////////

//! Constructs an object with parent \a parent
/*!
 *  The parent of an object may be viewed as the object's owner. The
 *  destructor of a parent object destroys all child objects. Setting
 *  parent to 0 constructs an object with no parent.
 *  The initial reference count is set to 0, and DeferredDeletion is enabled.
 */

dtkAbstractObject::dtkAbstractObject(dtkAbstractObject *parent) : QObject(parent), d_ptr(new dtkAbstractObjectPrivate(this))
{
    d_ptr->count = 0;

    d_ptr->isDeferredDeletionEnabled = true;
}

//! Copy constructor.
/*!
 *  
 */
dtkAbstractObject::dtkAbstractObject(const dtkAbstractObject& other) : QObject(other.parent()), d_ptr(new dtkAbstractObjectPrivate(*this, *other.d_ptr))
{
    d_ptr->count = 0;

    d_ptr->isDeferredDeletionEnabled = true;
}

//! Destroys the object, deleting all its child objects.
/*!
 * The reference count is checked, and a warning message is output if the reference count is not valid.
 * There are two valid reference count values at destruction time :<br>
 *  0 : The object used reference counting, and is no longer referred by any object.<br>
 *  1 : This occurs if the object was never reference counted, and delete(Later) was manually called.<br>
 */

dtkAbstractObject::~dtkAbstractObject(void)
{
    if ( d_ptr->count != 0 ){
        dtkDebug() << "Warning : deleting object of type " << this->metaObject()->className() << " with non-zero reference count";
    }

    delete d_ptr;

    d_ptr = NULL;
}

//! Assignement operator.
/*!
 *  
 */
dtkAbstractObject& dtkAbstractObject::operator = (const dtkAbstractObject& other)
{
    this->setParent(other.parent());

    d_ptr->values = other.d_ptr->values;
    d_ptr->properties = other.d_ptr->properties;
    d_ptr->metadatas = other.d_ptr->metadatas;

    d_ptr->count = 0;
    d_ptr->isDeferredDeletionEnabled = true;

    return *this;
}

//! Comparison operator.
/*!
 *  
 */
bool dtkAbstractObject::operator == (const dtkAbstractObject& other) const
{
    if (d_ptr->values != other.d_ptr->values         ||
        d_ptr->properties != other.d_ptr->properties ||
        d_ptr->metadatas != other.d_ptr->metadatas)
        return false;

    return true;
}

QString dtkAbstractObject::description(void) const
{
    DTK_DEFAULT_IMPLEMENTATION;

    return "";
}

QString dtkAbstractObject::identifier(void) const
{
    DTK_DEFAULT_IMPLEMENTATION;

    return "";
}

QString dtkAbstractObject::name(void) const
{
    return this->objectName();
}

//! Reference count.
/*!
 *  Returns the current reference count.
 */

int dtkAbstractObject::count(void) const
{
    return d_ptr->count;
}

//! Retain reference count.
/*!
 *  This method increments the reference counter once.
 */

int dtkAbstractObject::retain(void) const
{
    return d_ptr->count.fetchAndAddOrdered(1)+1;
}

//! Release reference count.
/*!
 *  This method decrements the reference counter once. Should the
 *  count be null, the object is scheduled for deletion. Note it sends
 *  the destroyed signal just before being actually deleted.
 *  The method of deletion depends on isDeferredDeletionEnabled().
 *  If it is true (default) the object will be deleted using "this->deleteLater();", 
 *  otherwise "delete this;" is used.
 */

int dtkAbstractObject::release(void) const
{
    int newCount = d_ptr->count.fetchAndAddOrdered(-1)-1;

    if(!(newCount)) {

        if (d_ptr->isDeferredDeletionEnabled)
            const_cast<dtkAbstractObject *>(this)->deleteLater();
        else
            delete this;
    }

    return newCount;
}

/*! 
 * \brief Enable / disable use of this->deleteLater() when reference count reaches 0.
 * \param value Changes what happens when the objects reference count reaches 0.
 *  For most objects this should be set to true, as it allows existing events
 *  to be processed which may use this object.
 *  \sa dtkAbstractObject::~dtkAbstractObject();
 *  \sa QObject::deleteLater();
 */
void dtkAbstractObject::enableDeferredDeletion(bool value)
{
    d_ptr->isDeferredDeletionEnabled = value;
}

bool dtkAbstractObject::isDeferredDeletionEnabled(void) const
{
    return d_ptr->isDeferredDeletionEnabled;
}

void dtkAbstractObject::addProperty(const QString& key, const QStringList& values)
{
    d_ptr->values.insert(key, values);
}

void dtkAbstractObject::addProperty(const QString& key, const QString& value)
{
    d_ptr->values.insert(key, QStringList() << value);
}

void dtkAbstractObject::setProperty(const QString& key, const QString& value)
{
    if(!d_ptr->values.contains(key)) {
    	dtkDebug() << this->metaObject()->className() << " has no such property:" << key;
    	return;
    }

    if(!d_ptr->values.value(key).contains(value)) {
    	dtkDebug() << this->metaObject()->className() << " has no such value:" << value << " for key: " << key;
    	return;
    }

    d_ptr->properties.insert(key, value);

    onPropertySet(key, value);

    emit propertySet(key, value);
}

QStringList dtkAbstractObject::propertyList(void) const
{
    return d_ptr->properties.keys();
}

QStringList dtkAbstractObject::propertyValues(const QString& key) const
{
    if(d_ptr->values.contains(key))
	return d_ptr->values[key];

    return QStringList();
}

bool dtkAbstractObject::hasProperty(const QString& key) const
{
    return d_ptr->values.contains(key);
}

QString dtkAbstractObject::property(const QString& key) const
{
    if(!d_ptr->values.contains(key)) {
	dtkDebug() << this->metaObject()->className() << "has no such property:" << key;
	return QString();
    }

    return d_ptr->properties.value(key);
}

void dtkAbstractObject::addMetaData(const QString& key, const QStringList& values)
{
    QStringList currentValues = d_ptr->metadatas.value(key);

    d_ptr->metadatas.insert(key, currentValues + values);

    foreach(QString value, values)
        onMetaDataSet(key, value);

    foreach(QString value, values)
        emit metaDataSet(key, value);
}

void dtkAbstractObject::addMetaData(const QString& key, const QString& value)
{
    QStringList currentValues = d_ptr->metadatas.value(key);

    d_ptr->metadatas.insert(key, currentValues << value);

    onMetaDataSet(key, value);

    emit metaDataSet(key, value);
}

void dtkAbstractObject::setMetaData(const QString& key, const QStringList& values)
{
    d_ptr->metadatas.insert(key, values);

    foreach(QString value, values)
        onMetaDataSet(key, value);

    foreach(QString value, values)
        emit metaDataSet(key, value);
}

void dtkAbstractObject::setMetaData(const QString& key, const QString& value)
{
    d_ptr->metadatas.insert(key, QStringList() << value);

    onMetaDataSet(key, value);

    emit metaDataSet(key, value);
}

QStringList dtkAbstractObject::metaDataList(void) const
{
    return d_ptr->metadatas.keys();
}

QStringList dtkAbstractObject::metaDataValues(const QString& key) const
{
    if(d_ptr->metadatas.contains(key))
	return d_ptr->metadatas[key];

    return QStringList();
}

bool dtkAbstractObject::hasMetaData(const QString& key) const
{
    return d_ptr->metadatas.contains(key);
}

void dtkAbstractObject::onPropertySet(const QString& key, const QString& value)
{
    DTK_UNUSED(key);
    DTK_UNUSED(value);
}

void dtkAbstractObject::onMetaDataSet(const QString& key, const QString& value)
{
    DTK_UNUSED(key);
    DTK_UNUSED(value);
}

QString dtkAbstractObject::metadata(const QString& key) const
{
    if(!d_ptr->metadatas.contains(key)) {
	dtkDebug() << this->metaObject()->className() << "has no such property:" << key;
	return QString();
    }

    return d_ptr->metadatas.value(key).first();
}

QStringList dtkAbstractObject::metadatas(const QString& key) const
{
    if(!d_ptr->metadatas.contains(key)) {
	dtkDebug() << this->metaObject()->className() << "has no such property:" << key;
	return QStringList();
    }

    return d_ptr->metadatas.value(key);
}

// /////////////////////////////////////////////////////////////////
// dtkAbstractObject documentation
// /////////////////////////////////////////////////////////////////

/*! \class dtkAbstractObject
 *
 *  \brief The dtkAbstractObject is the base class of all dtk abstract
 *  concepts.
 *
 *  The dtkAbstractObject is the root of the data, process and view
 *  taxomonies. It provides two main functionalities: a property
 *  system and a meta data system.
 *
 *  A property is a key that can correspond to a predefined set of
 *  values. It is no possible to assign a value that has not been
 *  declared together with the property. A property has a unique
 *  value.
 *
 *  \code
 *  object->addProperty("key", QStringList() << "value1" << "value2");
 *  object->setProperty("key", "value3");   // Wrong, "value3" has not been assigned to "key".
 *  object->setProperty("key", "value1");   // Right, "value1" has been assigned to "key".
 *  \endcode
 *
 *  The previous code snippet emits the signal propertySet and calls
 *  the virtual method onPropertySet so that one can override its
 *  behaviour when specializing any dtk concept.
 *
 *  A meta data is somehow similar to a property except that there is
 *  no constraint for the values one can assign to it. Also, a meta
 *  data is not unique, one can assign many values to it.
 *
 *  \code
 *  object->addMetaData("key", QStringList() << "value1" << "value2");
 *  object->setMetaData("key", "value3");   // Right, "value3" is now mapped to "key".
 *  object->setMetaData("key", "value4");   // Right, "value4" is now mapped to "key".
 *  object->addMetaData("key", QStringList() << "value1" << "value2");
 *
 *  dtkOutput() << object->metaData("key"); // ("value4", "value1", "value2")
 *  \endcode
 *
 *  The previous code snippet emits the signal metaDataSet and calls
 *  the virtual method onMetaDataSet so that one can override its
 *  behaviour when specializing any dtk concept.
 */
