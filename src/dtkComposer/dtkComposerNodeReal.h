/* dtkComposerNodeReal.h --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Mon Feb 27 12:40:45 2012 (+0100)
 * Version: $Id$
 * Last-Updated: Mon Feb 27 12:41:11 2012 (+0100)
 *           By: Julien Wintz
 *     Update #: 2
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef DTKCOMPOSERNODEREAL_H
#define DTKCOMPOSERNODEREAL_H

#include "dtkComposerExport.h"
#include "dtkComposerNodeLeaf.h"

class dtkComposerNodeRealPrivate;

class DTKCOMPOSER_EXPORT dtkComposerNodeReal : public dtkComposerNodeLeaf
{
public:
     dtkComposerNodeReal(void);
    ~dtkComposerNodeReal(void);

public:
    void run(void);

public:
    inline QString type(void) {
        return "real";
    }

    inline QString titleHint(void) {
        return "Real";
    }

    inline QString labelHint(int port) {
        return "value";
    }

private:
    dtkComposerNodeRealPrivate *d;
};

#endif
