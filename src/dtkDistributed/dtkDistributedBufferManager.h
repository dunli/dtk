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

#pragma once

#include <dtkCore>

#include "dtkDistributedExport.h"

// ///////////////////////////////////////////////////////////////////
// dtkDistributedBufferOperationManager
// ///////////////////////////////////////////////////////////////////

struct DTKDISTRIBUTED_EXPORT dtkDistributedBufferOperationManager
{
public:
    virtual ~dtkDistributedBufferOperationManager(void) {;}

public:
    virtual void addAssign(char *result, void *source, qlonglong count) = 0;
    virtual void subAssign(char *result, void *source, qlonglong count) = 0;
    virtual void mulAssign(char *result, void *source, qlonglong count) = 0;
    virtual void divAssign(char *result, void *source, qlonglong count) = 0;
};

// ///////////////////////////////////////////////////////////////////
// dtkDistributedBufferOperationManagerTyped
// ///////////////////////////////////////////////////////////////////

template <typename T> struct dtkDistributedBufferOperationManagerTyped : public dtkDistributedBufferOperationManager
{
public:
    void addAssign(char *result, void *source, qlonglong count);
    void subAssign(char *result, void *source, qlonglong count);
    void mulAssign(char *result, void *source, qlonglong count);
    void divAssign(char *result, void *source, qlonglong count);
};

// ///////////////////////////////////////////////////////////////////

template <typename T> inline void dtkDistributedBufferOperationManagerTyped<T>::addAssign(char *result, void *source, qlonglong count)
{
    dtkArray<T> r = dtkArray<T>::fromWritableRawData(reinterpret_cast<T *>(result), count);
    const dtkArray<T> s = dtkArray<T>::fromRawData(reinterpret_cast<T *>(source), count);
    std::transform(r.begin(), r.end(), s.begin(), r.begin(), std::plus<T>());
}

template <typename T> inline void dtkDistributedBufferOperationManagerTyped<T>::subAssign(char *result, void *source, qlonglong count)
{
    dtkArray<T> r = dtkArray<T>::fromWritableRawData(reinterpret_cast<T *>(result), count);
    const dtkArray<T> s = dtkArray<T>::fromRawData(reinterpret_cast<T *>(source), count);
    std::transform(r.begin(), r.end(), s.begin(), r.begin(), std::minus<T>());
}

template <typename T> inline void dtkDistributedBufferOperationManagerTyped<T>::mulAssign(char *result, void *source, qlonglong count)
{
    dtkArray<T> r = dtkArray<T>::fromWritableRawData(reinterpret_cast<T *>(result), count);
    const dtkArray<T> s = dtkArray<T>::fromRawData(reinterpret_cast<T *>(source), count);
    std::transform(r.begin(), r.end(), s.begin(), r.begin(), std::multiplies<T>());
}

template <typename T> inline void dtkDistributedBufferOperationManagerTyped<T>::divAssign(char *result, void *source, qlonglong count)
{
    dtkArray<T> r = dtkArray<T>::fromWritableRawData(reinterpret_cast<T *>(result), count);
    const dtkArray<T> s = dtkArray<T>::fromRawData(reinterpret_cast<T *>(source), count);
    std::transform(r.begin(), r.end(), s.begin(), r.begin(), std::divides<T>());
}

// ///////////////////////////////////////////////////////////////////
// dtkDistributedBufferManager
// ///////////////////////////////////////////////////////////////////

class DTKDISTRIBUTED_EXPORT dtkDistributedBufferManager
{
public:
             dtkDistributedBufferManager(void);
    virtual ~dtkDistributedBufferManager(void);

public:
    template <typename T>    T *allocate(qlonglong capacity);
    template <typename T> void  deallocate(T *& buffer);
    
protected:
    virtual void *allocate(qlonglong objectSize, qlonglong capacity, int metatype_id) = 0;
    virtual void  deallocate(void *buffer, qlonglong objectSize) = 0;

public:
    virtual void rlock(qlonglong wid)  = 0;
    virtual void rlock(void)  = 0;
    virtual void wlock(qlonglong wid)  = 0;
    virtual void wlock(void)  = 0;

    virtual void unlock(qlonglong wid) = 0;
    virtual void unlock(void) = 0;

    virtual bool locked(qlonglong wid) = 0;

public:
    virtual void get(qint32 from, qlonglong position, void *array, qlonglong count = 1) = 0;
    virtual void put(qint32 dest, qlonglong position, void *array, qlonglong count = 1) = 0;
    virtual void addAssign(qint32 dest, qlonglong position, void *array, qlonglong nelements = 1) = 0;
    virtual void mulAssign(qint32 dest, qlonglong position, void *array, qlonglong nelements = 1) = 0;

protected:
    virtual bool canHandleOperationManager(void);

protected:
    dtkDistributedBufferOperationManager *operation_manager;
};

// ///////////////////////////////////////////////////////////////////
// dtkDistributedBufferManager templated member functions
// ///////////////////////////////////////////////////////////////////

inline dtkDistributedBufferManager::dtkDistributedBufferManager(void) : operation_manager(NULL) 
{
}

inline dtkDistributedBufferManager::~dtkDistributedBufferManager(void)
{
}

inline bool dtkDistributedBufferManager::canHandleOperationManager(void)
{
    return true;
}

template <typename T> inline T *dtkDistributedBufferManager::allocate(qlonglong capacity)
{
    if (this->canHandleOperationManager() && !this->operation_manager)
        this->operation_manager = new dtkDistributedBufferOperationManagerTyped<T>();

    return static_cast<T *>(this->allocate(sizeof(T), capacity, qMetaTypeId<T>()));
}

template <typename T> inline void dtkDistributedBufferManager::deallocate(T *& buffer)
{
    if (this->operation_manager && this->canHandleOperationManager()) {
        delete this->operation_manager;
        this->operation_manager = NULL;
    }

    this->deallocate(buffer, sizeof(T));
    buffer = NULL;
}

// 
// dtkDistributedBufferManager.h ends here
