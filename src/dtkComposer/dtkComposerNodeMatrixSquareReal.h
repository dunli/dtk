/* dtkComposerNodeMatrixSquareReal.h ---
 *
 * Author: sblekout
 * Copyright (C) 2011 - babette lekouta, Inria.
 * Created: Thu Apr 26 10:10:43 2012 (+0200)
 * Version: $Id$
 * Last-Updated: Tue May 29 15:15:05 2012 (+0200)
 *           By: tkloczko
 *     Update #: 10
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKCOMPOSERNODEMATRIXSQUAREREAL_H
#define DTKCOMPOSERNODEMATRIXSQUAREREAL_H

#include "dtkComposerExport.h"
#include "dtkComposerNodeLeaf.h"

class dtkComposerNodeMatrixSquareRealPrivate;

class DTKCOMPOSER_EXPORT dtkComposerNodeMatrixSquareReal : public dtkComposerNodeLeaf
{
public:
     dtkComposerNodeMatrixSquareReal(void);
    ~dtkComposerNodeMatrixSquareReal(void);

public:
    void run(void);

public:
    inline QString type(void) {
        return "matrix_square_real";
    }

    inline QString titleHint(void) {
        return "Matrix Square Real";
    }

public:
    QString  inputLabelHint(int port);
    QString outputLabelHint(int port);

private:
    dtkComposerNodeMatrixSquareRealPrivate *d;
};

#endif
