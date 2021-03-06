/* %ClassName%_p.h ---
 *
 * Author: %Author%
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef %ClassName:u%_P_H
#define %ClassName:u%_P_H

#include "%Layer%Export.h"

#include <dtkCore/dtkAbstractProcess_p.h>

class %ClassName%;

////////////////////////////////////////////////////
// %ClassName%Private interface
////////////////////////////////////////////////////

class DTKCORE_EXPORT %ClassName%Private : public dtkAbstractProcessPrivate
{
public:
    %ClassName%Private(%ClassName% *q = 0) : dtkAbstractProcessPrivate(q) {}
    %ClassName%Private(const %ClassName%Private& other) : dtkAbstractProcessPrivate(other) {}

public:
    virtual ~%ClassName%Private(void) {}

public:
    // private members of the class are put here in the d-pointer, eg
    // int index;
};

////////////////////////////////////////////////////
// %ClassName% protected constructors
////////////////////////////////////////////////////

DTK_IMPLEMENT_PRIVATE(%ClassName%, dtkAbstractProcess);

#endif
