/* dtkAbstractViewFactory.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Fri Nov  7 15:48:10 2008 (+0100)
 * Version: $Id$
 * Last-Updated: Tue Jul  5 15:24:54 2011 (+0200)
 *           By: Julien Wintz
 *     Update #: 107
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKABSTRACTVIEWERFACTORY_H
#define DTKABSTRACTVIEWERFACTORY_H

#include <dtkCore/dtkAbstractFactory.h>

                  class dtkAbstractView;
                  class dtkAbstractViewAnimator;
                  class dtkAbstractViewNavigator;
                  class dtkAbstractViewInteractor;
                  class dtkAbstractViewFactoryPrivate;
template<class T> class dtkSmartPointer;

class DTKCORE_EXPORT dtkAbstractViewFactory : public dtkAbstractFactory
{
    Q_OBJECT

public:
    typedef dtkAbstractView           *(*dtkAbstractViewCreator)          (void);
    typedef dtkAbstractViewAnimator   *(*dtkAbstractViewAnimatorCreator)  (void);
    typedef dtkAbstractViewNavigator  *(*dtkAbstractViewNavigatorCreator) (void);
    typedef dtkAbstractViewInteractor *(*dtkAbstractViewInteractorCreator)(void);

    typedef QPair<QString, QStringList> dtkAbstractViewTypeHandler;

public:
    static dtkAbstractViewFactory *instance(void);

    bool registerViewType          (const QString& type,                             dtkAbstractViewCreator           func);
    bool registerViewAnimatorType  (const QString& type, const QStringList& handled, dtkAbstractViewAnimatorCreator   func);
    bool registerViewNavigatorType (const QString& type, const QStringList& handled, dtkAbstractViewNavigatorCreator  func);
    bool registerViewInteractorType(const QString& type, const QStringList& handled, dtkAbstractViewInteractorCreator func);

    unsigned int size(const QString& type) const;

    QList<QString>                    creators(void) const;
    QList<dtkAbstractViewTypeHandler> animators(void) const;
    QList<dtkAbstractViewTypeHandler> interactors(void) const;
    QList<dtkAbstractViewTypeHandler> navigators(void) const;

    dtkSmartPointer<dtkAbstractView>               createSmartPointer(const QString& type);
    dtkSmartPointer<dtkAbstractViewAnimator>     animatorSmartPointer(const QString& type);
    dtkSmartPointer<dtkAbstractViewNavigator>   navigatorSmartPointer(const QString& type);
    dtkSmartPointer<dtkAbstractViewInteractor> interactorSmartPointer(const QString& type);

signals:
    void created(dtkAbstractView *view, const QString& type);

public slots:
    dtkAbstractView           *create(const QString& type);
    dtkAbstractViewAnimator   *animator(const QString& type);
    dtkAbstractViewNavigator  *navigator(const QString& type);
    dtkAbstractViewInteractor *interactor(const QString& type);

protected:
     dtkAbstractViewFactory(void);
    ~dtkAbstractViewFactory(void);

private:    
    static dtkAbstractViewFactory *s_instance;

private:
    dtkAbstractViewFactoryPrivate *d;
};

#endif
