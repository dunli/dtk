/* dtkScriptInterpreterTcl.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Wed Nov 26 16:18:42 2008 (+0100)
 * Version: $Id$
 * Last-Updated: Thu Jan 10 14:23:36 2013 (+0100)
 *           By: Julien Wintz
 *     Update #: 115
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKSCRIPTINTERPRETERTCL_H
#define DTKSCRIPTINTERPRETERTCL_H

#include <dtkCore/dtkGlobal.h>

#include "dtkScriptInterpreter.h"

// /////////////////////////////////////////////////////////////////
// dtkScriptInterpreterTcl
// /////////////////////////////////////////////////////////////////

class dtkScriptInterpreterTclPrivate;

class DTKSCRIPT_EXPORT dtkScriptInterpreterTcl : public dtkScriptInterpreter
{
    Q_OBJECT

public:
     dtkScriptInterpreterTcl(QObject *parent = 0);
    ~dtkScriptInterpreterTcl(void);

public slots:
    virtual QString interpret(const QString& command, int *stat);

private:
    dtkScriptInterpreterTclPrivate *d;
};

#endif
