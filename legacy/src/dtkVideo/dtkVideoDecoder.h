/* dtkVideoDecoder.h ---
 *
 * Author: Nicolas Niclausse
 * Copyright (C) 2013 - Nicolas Niclausse, Inria.
 * Created: mar. oct. 15 13:53:38 2013 (+0200)
 */

/* Commentary:
 * See credits at EOF.
 */

/* Change log:
 *
 */

#pragma once

#include "dtkVideoExport.h"

#include <QtCore>
#include <QtGui>

class dtkVideoDecoderPrivate;

class DTKVIDEO_EXPORT dtkVideoDecoder
{
public:
             dtkVideoDecoder(void);
             dtkVideoDecoder(QString file);
    virtual ~dtkVideoDecoder(void);

public:
      bool openFile(QString file);

public:
      void close(void);

public:
      bool getFrame(QImage&img,int *effectiveframenumber=0,int *effectiveframetime=0,int *desiredframenumber=0,int *desiredframetime=0);
      bool seekNextFrame();
      bool seekMs(int ts);
      bool seekFrame(int64_t frame);
      int getVideoLengthMs();

private:
      dtkVideoDecoderPrivate *d;
};

// ///////////////////////////////////////////////////////////////////
// Credits
// ///////////////////////////////////////////////////////////////////

/*
  QTFFmpegWrapper - QT FFmpeg Wrapper Class
  Copyright (C) 2009,2010: Daniel Roggen, droggen@gmail.com

  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

  1. Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDERS ``AS IS'' AND ANY
  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE FREEBSD PROJECT OR
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
  OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
  SUCH DAMAGE.
*/