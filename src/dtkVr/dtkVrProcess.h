/* dtkVrProcess.h --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Wed Feb 10 21:08:39 2010 (+0100)
 * Version: $Id$
 * Last-Updated: Thu Feb 18 11:16:13 2010 (+0100)
 *           By: Julien Wintz
 *     Update #: 58
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef DTKVRPROCESS_H
#define DTKVRPROCESS_H

class dtkDistributedCommunicator;
class dtkVrDevice;
class dtkVrProcessPrivate;

class dtkVrProcess
{
public:
             dtkVrProcess(dtkDistributedCommunicator *communicator);
    virtual ~dtkVrProcess(void);

    virtual void   initialize(void) = 0;
    virtual void uninitialize(void) = 0;

    void show(bool fullscreen = false);

    void start(void);
    void stop(void);

    int rank(void) const;
    int size(void) const;

    bool running(void);
    
protected:
    virtual void process(void) = 0;

    void broadcast(void);
    void synchronize(void);

private:
    dtkVrProcessPrivate *d;
};

#endif
