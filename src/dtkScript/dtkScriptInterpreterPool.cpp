/* dtkScriptInterpreterPool.cpp --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Mon Jan 26 09:48:03 2009 (+0100)
 * Version: $Id$
 * Last-Updated: Thu Oct 14 21:13:17 2010 (+0200)
 *           By: Julien Wintz
 *     Update #: 50
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#include <dtkScript/dtkScriptInterpreter.h>
#if defined(HAVE_SWIG) && defined(HAVE_PYTHON)
#include <dtkScript/dtkScriptInterpreterPython.h>
#endif
#if defined(HAVE_SWIG) && defined(HAVE_TCL)
#include <dtkScript/dtkScriptInterpreterTcl.h>
#endif
#include <dtkScript/dtkScriptInterpreterPool.h>

class dtkScriptInterpreterPoolPrivate
{
public:
    QMap<QString, QPointer<dtkScriptInterpreter> > interpreters;
};

dtkScriptInterpreterPool *dtkScriptInterpreterPool::instance(void)
{
    if(!m_instance)
	m_instance = new dtkScriptInterpreterPool;

    return m_instance;
}

dtkScriptInterpreter *dtkScriptInterpreterPool::console(QString type)
{
#if defined(HAVE_SWIG) && defined(HAVE_PYTHON) && defined(HAVE_TCL)
    if(!d->interpreters.contains("console") || !d->interpreters.value("console"))
        if(type == "tcl")
            d->interpreters.insert("console", new dtkScriptInterpreterTcl);
	else
            d->interpreters.insert("console", new dtkScriptInterpreterPython);

    return d->interpreters.value("console");
#else
    return NULL;
#endif
}

dtkScriptInterpreter *dtkScriptInterpreterPool::python(void)
{
#if defined(HAVE_SWIG) && defined(HAVE_PYTHON)
    if(!d->interpreters.contains("python") || !d->interpreters.value("python"))
	d->interpreters.insert("python", new dtkScriptInterpreterPython);

    return d->interpreters.value("python");
#else
    return NULL;
#endif
}

dtkScriptInterpreter *dtkScriptInterpreterPool::tcl(void)
{
#if defined(HAVE_SWIG) && defined(HAVE_TCL)
    if(!d->interpreters.contains("tcl") || !d->interpreters.value("tcl"))
	d->interpreters.insert("tcl", new dtkScriptInterpreterTcl);

    return d->interpreters.value("tcl");
#else
    return NULL;
#endif
}

dtkScriptInterpreterPool::dtkScriptInterpreterPool(void) : QObject(), d(new dtkScriptInterpreterPoolPrivate)
{

}

dtkScriptInterpreterPool::~dtkScriptInterpreterPool(void)
{
    foreach(dtkScriptInterpreter *interpreter, d->interpreters.values())
	delete interpreter;

    delete d;

    d= NULL;
}

dtkScriptInterpreterPool *dtkScriptInterpreterPool::m_instance = NULL;
