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

#include "dtkDistributed.h"
#include "dtkDistributedCommunicator.h"
#include "dtkDistributedContainer.h"
#include "dtkDistributedMapper.h"
#include "dtkDistributedWorker.h"

// ///////////////////////////////////////////////////////////////////
// dtkDistributedArray implementation
// ///////////////////////////////////////////////////////////////////

template<typename T> inline dtkDistributedArray<T>::dtkDistributedArray(const qlonglong& size, dtkDistributedWorker *worker) : 
    dtkDistributedContainer(size, worker)
{
    this->initialize();
}

template<typename T> inline dtkDistributedArray<T>::dtkDistributedArray(const qlonglong& size, dtkDistributedWorker *worker, dtkDistributedMapper *mapper) : 
    dtkDistributedContainer(size, worker, mapper)
{
    this->initialize();
}

template<typename T> inline dtkDistributedArray<T>::dtkDistributedArray(const qlonglong& size, const T *array, dtkDistributedWorker *worker) : 
    dtkDistributedContainer(size, worker)
{
    this->initialize();
    
    for (qlonglong i = 0; i < m_mapper->count(this->wid()); ++i) {
        data.setValue(i, array[m_mapper->localToGlobal(i, this->wid())]);
    }
}

template<typename T> inline dtkDistributedArray<T>::~dtkDistributedArray(void)
{
    m_comm->deallocate(this->wid(), data.id());

    delete cache;
}

template<typename T> inline void dtkDistributedArray<T>::initialize(void)
{
    qlonglong size = m_mapper->count(this->wid());
    qlonglong   id = this->cid();
    T *array = static_cast<T*>(m_comm->allocate(size, sizeof(T), this->wid(), id));
    m_comm->barrier();
    data.setData(array, size, id);

    cache = new dtkDistributedArrayCache<T>(this);
}

template<typename T> inline void dtkDistributedArray<T>::setAt(const qlonglong& index, const T& value)
{
    qint32 owner = static_cast<qint32>(m_mapper->owner(index));
    qlonglong pos = m_mapper->globalToLocal(index);
    m_comm->put(owner, pos, &(const_cast<T&>(value)), data.id());
}

template<typename T> inline T dtkDistributedArray<T>::at(const qlonglong& index) const
{
    qint32 owner = static_cast<qint32>(m_mapper->owner(index));

    if (this->wid() == owner) {
        qlonglong pos = m_mapper->globalToLocal(index);
        T temp;
        m_comm->get(owner, pos, &temp, data.id());
        return temp;

    } else {
        return cache->value(index);
    }
}

template<typename T> inline void dtkDistributedArray<T>::rlock(qint32 owner)
{
    m_comm->rlock(owner, data.id());
}

template<typename T> inline void dtkDistributedArray<T>::wlock(qint32 owner)
{
    m_comm->wlock(owner, data.id());
}

template<typename T> inline void dtkDistributedArray<T>::unlock(qint32 owner)
{
    m_comm->unlock(owner, data.id());
}

template<typename T> inline bool dtkDistributedArray<T>::read(const QString& filename)
{
    return false;
}

template<typename T> inline T dtkDistributedArray<T>::first(void) const
{
    return this->at(0);
}

template<typename T> inline T dtkDistributedArray<T>::last(void) const
{
    return this->at(this->size() - 1);
}

template<typename T> inline void dtkDistributedArray<T>::setLocalValue(const qlonglong& index, const T& value)
{
    Q_ASSERT_X(index >= 0 && i < data.size(), "dtkDistributedArray<T>::setLocalValue", "index out of range");
    data.setValue(index, value);
}

template<typename T> inline const T& dtkDistributedArray<T>::localValue(const qlonglong& index) const
{
    Q_ASSERT_X(index >= 0 && i < data.size(), "dtkDistributedArray<T>::localValue", "index out of range");
    return data.value(index);
}

template<typename T> inline qlonglong dtkDistributedArray<T>::dataId(void) const
{
    return data.id();
}

// 
// dtkDistributedArray.tpp ends here
