/* dtkComposerNodeFactoryView.h ---
 * 
 * Author: Thibaud Kloczko
 * Created: Thu Apr 11 10:38:29 2013 (+0200)
 * Version: 
 * Last-Updated: Mon Apr 15 16:06:34 2013 (+0200)
 *           By: Julien Wintz
 *     Update #: 5
 */

/* Change Log:
 * 
 */

#pragma once

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

class dtkComposerNodeFactory;
class dtkComposerNodeFactoryViewPrivate;

class  dtkComposerNodeFactoryView : public QWidget
{
    Q_OBJECT

public:
     dtkComposerNodeFactoryView(QWidget *parent = 0);
    ~dtkComposerNodeFactoryView(void);

public:
    void setFactory(dtkComposerNodeFactory *factory);

public:
    void setDark(void);

private:
    dtkComposerNodeFactoryViewPrivate *d;
};
