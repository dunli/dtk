/* dtkComposerNodeFile.cpp --- 
 * 
 * Author: Julien Wintz, INRIA
 * Created: Thu Mar  1 11:45:03 2012 (+0100)
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#include "dtkComposerNodeFile.h"
#include "dtkComposerNodeFile_p.h"
#include "dtkComposerTransmitterEmitter.h"
#include "dtkComposerTransmitterReceiver.h"

#include <QtCore>
#include <QtNetwork>

// /////////////////////////////////////////////////////////////////
// dtkComposerPrivate
// /////////////////////////////////////////////////////////////////

void dtkComposerNodeFilePrivate::download(const QUrl& url)
{
    Q_UNUSED(url);

    // QFileInfo file_template_info = QFileInfo(url.path());

    // QTemporaryFile file;
    // if(!file_template_info.completeSuffix().isEmpty())
    //     file.setFileTemplate(file.fileTemplate() + "." + file_template_info.completeSuffix());
    // file.setAutoRemove(false);
    
    // if (!file.open()) {
    //     qDebug() << Q_FUNC_INFO << "Unable to file for saving";
    //     return;
    // }
        
    // this->dwnl_ok = 0;
    
    // QHttp http;
    
    // connect(&http, SIGNAL(requestFinished(int, bool)), this, SLOT(onRequestFinished(int, bool)));

    // http.setHost(url.host(), url.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp, url.port() == -1 ? 0 : url.port());
        
    // if (!url.userName().isEmpty())
    //     http.setUser(url.userName(), url.password());
        
    // QByteArray path = QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/");
    
    // if (path.isEmpty()) {
    //     qDebug() << Q_FUNC_INFO << "Invalid path" << url.path();
    //     return;
    // }
    
    // this->dwnl_id = http.get(path, &file);
    
    // while(!this->dwnl_ok)
    //     qApp->processEvents();

    // file.close();

    // QFileInfo info(file);
    
    // this->tempName = info.absoluteFilePath();
}

void dtkComposerNodeFilePrivate::onRequestFinished(int id, bool error)
{
    Q_UNUSED(error);

    if(id == this->dwnl_id)
        this->dwnl_ok = 1;
}

// /////////////////////////////////////////////////////////////////
// dtkComposerNodeFile implementation
// /////////////////////////////////////////////////////////////////

dtkComposerNodeFile::dtkComposerNodeFile(void) : dtkComposerNodeLeaf(), d(new dtkComposerNodeFilePrivate)
{
    this->appendReceiver(&(d->receiver));

    d->fileName = QString();
    this->appendEmitter(&(d->emitter));
}

dtkComposerNodeFile::~dtkComposerNodeFile(void)
{
    delete d;
    
    d = NULL;
}

void dtkComposerNodeFile::run(void)
{
    QString path;

    if (!d->receiver.isEmpty())
        path = d->receiver.data();
    else
        path = d->fileName;

    if (path.startsWith("http")) {
        
        d->download(QUrl(path));
        
        if (!d->tempName.isEmpty())
            d->fileName = d->tempName;
        else
            d->fileName = path;
        
    } else {
        
        d->fileName = path;
        
    }

    d->emitter.setData(d->fileName);
}

QString dtkComposerNodeFile::type(void)
{
    return "file";
}

QString dtkComposerNodeFile::titleHint(void)
{
    return "File";
}

QString dtkComposerNodeFile::inputLabelHint(int port)
{
    if(port == 0)
        return "name";

    return dtkComposerNode::inputLabelHint(port);
}

QString dtkComposerNodeFile::outputLabelHint(int port)
{
    if(port == 0)
        return "file";

    return dtkComposerNode::outputLabelHint(port);
}

QString dtkComposerNodeFile::value(void)
{
    return d->fileName;
}

void dtkComposerNodeFile::setValue(QString value)
{
    d->fileName = value;
}
