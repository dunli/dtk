// Version: $Id$
// 
// 

// Commentary: 
// 
// 

// Change Log:
// 
// 

// Code:

#include "dtkMetaTypeTest.h"

#include <dtkMeta/dtkMeta>

#include <QtCore>
#include <QtGui>

// ///////////////////////////////////////////////////////////////////
// Data concrete class
// ///////////////////////////////////////////////////////////////////

class Data
{
public:
             Data(void) {;}
             Data(const int& id, const QString& name) : m_id(id), m_name(name) {;}
             Data(const Data& o) : m_id(o.m_id), m_name(o.m_name) {;}
    virtual ~Data(void) {;}

public:
    Data& operator = (const Data& o) { m_id = o.m_id; m_name = o.m_name; return (*this); }

public:
    bool operator == (const Data& o) const { if (m_id != o.m_id || m_name != o.m_name) return false; return true; }

public:
    void   setId(const int& id)       { m_id   = id; }
    void setName(const QString& name) { m_name = name; }

public:
    int       id(void) const { return m_id; }
    QString name(void) const { return m_name; }

private:
    int m_id;
    QString m_name;
};

Q_DECLARE_METATYPE(Data);
Q_DECLARE_METATYPE(Data *);

QDebug operator << (QDebug dbg, const Data &data)
{
    dbg.nospace() << "Data(" << data.id() << ", " << data.name() << ")"; return dbg.space();
}

QDebug operator << (QDebug dbg, Data *data)
{
    dbg.nospace() << "Data(" << data->id() << ", " << data->name() << ")"; return dbg.space();
}

class DeriveData : public Data {};

Q_DECLARE_METATYPE(DeriveData);
Q_DECLARE_METATYPE(DeriveData *);

// ///////////////////////////////////////////////////////////////////
// Abstract class 
// ///////////////////////////////////////////////////////////////////

class MyAbstract
{
public:
             MyAbstract(void) {;}
    virtual ~MyAbstract(void) {;}

public:
    MyAbstract& operator = (const MyAbstract& o) { setId(o.id()); setName(o.name()); return (*this); }

public:
    bool operator == (const MyAbstract& o) const { if (id() != o.id() || name() != o.name()) return false; return true; }

public:
    virtual void   setId(const int& id) = 0;
    virtual void setName(const QString& name) = 0;

public:
    virtual int       id(void) const = 0;
    virtual QString name(void) const = 0;
};

QDebug operator << (QDebug dbg, MyAbstract *o)
{
    dbg.nospace() << "MyAbstract(" << o->id() << ", " << o->name() << ")"; return dbg.space();
}

Q_DECLARE_METATYPE(MyAbstract *);

// ///////////////////////////////////////////////////////////////////

class DeriveMyAbstract : public MyAbstract 
{
public:
     DeriveMyAbstract(void) : MyAbstract(), m_id(-1), m_name(QString()) {;}
     DeriveMyAbstract(const DeriveMyAbstract& other) : MyAbstract(), m_id(other.m_id), m_name(other.m_name) {;}
    ~DeriveMyAbstract(void) {;}

public:
    void setId(const int& id) { m_id = id; }
    void setName(const QString& name) { m_name = name; }

public:
    int       id(void) const { return m_id; }
    QString name(void) const { return m_name; }

private:
    int m_id;
    QString m_name;
};

Q_DECLARE_METATYPE(DeriveMyAbstract);
Q_DECLARE_METATYPE(DeriveMyAbstract *);

// ///////////////////////////////////////////////////////////////////
// Abstract QObject class
// ///////////////////////////////////////////////////////////////////

class VirtualObject : public QObject
{
    Q_OBJECT

public:
             VirtualObject(void) {;}
    virtual ~VirtualObject(void) {;}

public:
    VirtualObject& operator = (const VirtualObject& o) { setId(o.id()); setName(o.name()); return (*this); }

public:
    bool operator == (const VirtualObject& o) const { if (id() != o.id() || name() != o.name()) return false; return true; }

public:
    virtual void   setId(const int& id) = 0;
    virtual void setName(const QString& name) = 0;

public:
    virtual int       id(void) const = 0;
    virtual QString name(void) const = 0;
};

QDebug operator << (QDebug dbg, VirtualObject *o)
{
    dbg.nospace() << "VirtualObject(" << o->id() << ", " << o->name() << ")"; return dbg.space();
}

Q_DECLARE_METATYPE(VirtualObject *);

// ///////////////////////////////////////////////////////////////////

class VirtualObject2 : public VirtualObject 
{
    Q_OBJECT

public:
             VirtualObject2(void) {;}
    virtual ~VirtualObject2(void) {;}

public:
    virtual QString identifier(void) { return "VirtualObject2"; }
};

Q_DECLARE_METATYPE(VirtualObject2 *);

// ///////////////////////////////////////////////////////////////////

class DeriveVirtualObject : public VirtualObject2
{
    Q_OBJECT

public:
     DeriveVirtualObject(void) : VirtualObject2(), m_id(-1), m_name(QString()) {;}
     DeriveVirtualObject(const DeriveVirtualObject& other) : VirtualObject2(), m_id(other.m_id), m_name(other.m_name) {;}
    ~DeriveVirtualObject(void) {;}

public:
    void setId(const int& id) { m_id = id; }
    void setName(const QString& name) { m_name = name; }

public:
    int       id(void) const { return m_id; }
    QString name(void) const { return m_name; }

private:
    int m_id;
    QString m_name;
};

Q_DECLARE_METATYPE(DeriveVirtualObject);
Q_DECLARE_METATYPE(DeriveVirtualObject *);

VirtualObject *toto(VirtualObject2* from)
{
    qDebug() << Q_FUNC_INFO;
    return from;
}

// ///////////////////////////////////////////////////////////////////
// dtkMetaTypeTestCasePrivate
// ///////////////////////////////////////////////////////////////////

class dtkMetaTypeTestCasePrivate
{
public:
    dtkMetaTypeTestCasePrivate(void) : count(0), data(0), derive_data(0), abstract(0), virtual_object(0), virtual_object2(0) {;}
    ~dtkMetaTypeTestCasePrivate(void) {;}

public:
    int count;

    Data *data;
    DeriveData *derive_data;
    MyAbstract *abstract;
    VirtualObject *virtual_object;
    VirtualObject2 *virtual_object2;
};

// ///////////////////////////////////////////////////////////////////
// dtkMetaTypeTestCase implementation
// ///////////////////////////////////////////////////////////////////

dtkMetaTypeTestCase::dtkMetaTypeTestCase(void) : QObject(), d(new dtkMetaTypeTestCasePrivate)
{
    // Register debug operators
    QMetaType::registerDebugStreamOperator<Data>();
    QMetaType::registerDebugStreamOperator<Data*>();
    QMetaType::registerDebugStreamOperator<MyAbstract*>();
    QMetaType::registerDebugStreamOperator<VirtualObject*>();

    // Register converters
    QMetaType::registerConverter<DeriveData*, Data*>();
    QMetaType::registerConverter<DeriveMyAbstract*, MyAbstract*>();
}

dtkMetaTypeTestCase::~dtkMetaTypeTestCase(void)
{

}

void dtkMetaTypeTestCase::initTestCase(void)
{
    
}

void dtkMetaTypeTestCase::init(void)
{
    d->count = 0;

    d->data = new Data(d->count ++, "PData");

    d->derive_data = new DeriveData();
    d->derive_data->setId(d->count++);
    d->derive_data->setName("PDeriveData");

    d->abstract = new DeriveMyAbstract();
    d->abstract->setName("PDeriveMyAbstract");
    d->abstract->setId(d->count++);

    d->virtual_object = new DeriveVirtualObject();
    d->virtual_object->setName("PDeriveVirtualObject");
    d->virtual_object->setId(d->count++);

    d->virtual_object2 = new DeriveVirtualObject();
    d->virtual_object2->setName("PDeriveVirtualObject2");
    d->virtual_object2->setId(d->count++);
}

void dtkMetaTypeTestCase::cleanup(void)
{
    delete d->data;
    delete d->derive_data;
    delete d->abstract;
    delete d->virtual_object;
    delete d->virtual_object2;
}

void dtkMetaTypeTestCase::cleanupTestCase(void)
{

}

void dtkMetaTypeTestCase::testVariantFromValue(void)
{
    // Non QObject Data
    {
        Data d0 = *d->data;
        QVariant v_d0 = dtkMetaType::variantFromValue(d0);
        QVERIFY(v_d0.isValid());
        QCOMPARE(v_d0.typeName(), "Data");
        QCOMPARE(v_d0.value<Data>(), d0);
    }

    // Non QObject Data pointer
    {
        Data *d0 = d->data;
        QVariant v_d0 = dtkMetaType::variantFromValue(d0);
        QVERIFY(v_d0.isValid());
        QCOMPARE(v_d0.typeName(), "Data*");
        QCOMPARE(*(v_d0.value<Data*>()), *d0);

        DeriveData *dd0 = d->derive_data;
        QVariant v_dd0 = dtkMetaType::variantFromValue(dd0);
        QVERIFY(v_dd0.isValid());
        QCOMPARE(v_dd0.typeName(), "DeriveData*");
        QCOMPARE(*(v_dd0.value<DeriveData*>()), *dd0);
        QVERIFY(v_dd0.canConvert(v_d0.userType()));
        
        Data *d1 = dd0;
        QVariant v_d1 = dtkMetaType::variantFromValue(d1);
        QVERIFY(v_d1.isValid());
        QVERIFY(v_d1.userType() != v_dd0.userType());
        QCOMPARE(v_d1.typeName(), "Data*");

        Data *d2 = v_dd0.value<Data*>();
        QCOMPARE(*d1, *d2);
    }

    // Non QObject Abstract class
    {
        MyAbstract *a0 = d->abstract;

        QVariant v_a0 = dtkMetaType::variantFromValue(a0);
        QVERIFY(v_a0.isValid());
        QCOMPARE(v_a0.typeName(), "MyAbstract*");
        QCOMPARE(*(v_a0.value<MyAbstract*>()), *a0);

        DeriveMyAbstract *b0 = dynamic_cast<DeriveMyAbstract*>(a0);
        QVariant v_b0 = dtkMetaType::variantFromValue(b0);
        QVERIFY(v_b0.isValid());
        QCOMPARE(v_b0.typeName(), "DeriveMyAbstract*");
        QCOMPARE(*(v_b0.value<DeriveMyAbstract*>()), *b0);

        QVERIFY(v_b0.canConvert(v_a0.userType()));
        QVERIFY(v_b0.convert(v_a0.userType()));

    }

    // QObject Abstract class
    {
        VirtualObject *a0 = d->virtual_object;

        // Since DeriveVirtualObject has not been registered at
        // runtime, the resulting variant is of type VirtualObject*.
        QVariant v_a0 = dtkMetaType::variantFromValue(a0);
        QVERIFY(v_a0.isValid());
        QCOMPARE(v_a0.typeName(), "VirtualObject*");
        QCOMPARE(*(v_a0.value<VirtualObject*>()), *a0);

        // Now, let us register DeriveVirtualObject class
        qRegisterMetaType<DeriveVirtualObject*>();

        // It follows that the resulting variant is of the truly type
        // DeriveVirtualObject*.
        QVariant w_a0 = dtkMetaType::variantFromValue(a0);
        QVERIFY(w_a0.isValid());
        QCOMPARE(w_a0.typeName(), "DeriveVirtualObject*");
        QCOMPARE(*(w_a0.value<VirtualObject*>()), *a0);


        // QMetaType::registerConverter<VirtualObject2*, VirtualObject*>(toto);

        // VirtualObject2 *b1 = d->virtual_object2;
        // QVariant vv_b1 = QVariant::fromValue(b1);
        // QVERIFY(vv_b1.canConvert(qMetaTypeId<VirtualObject*>()));
        // QVERIFY(vv_b1.convert(qMetaTypeId<VirtualObject*>()));
    }
}

void dtkMetaTypeTestCase::testClone(void)
{
    
}

DTKTEST_MAIN_NOGUI(dtkMetaTypeTest, dtkMetaTypeTestCase)

#include "dtkMetaTypeTest.moc"

// 
// dtkMetaTypeTest.cpp ends here
