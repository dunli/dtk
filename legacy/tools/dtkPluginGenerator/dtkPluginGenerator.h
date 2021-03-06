/* dtkPluginGenerator.h --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Mon Mar  9 21:40:22 2009 (+0100)
 * Version: $Id$
 * Last-Updated: Mon Sep  5 14:53:38 2011 (+0200)
 *           By: Julien Wintz
 *     Update #: 31
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef DTKPLUGINGENERATOR_H
#define DTKPLUGINGENERATOR_H

#include <QtCore>

class dtkPluginGeneratorPrivate
{
public:
    QString plugin;
    QString output;
    QString prefix;
    QString suffix;
    QString type;
    QString description;
    QString license;

    QDir parent;
    QDir target;
};

class dtkPluginGenerator
{
public:
    dtkPluginGenerator(void);
   ~dtkPluginGenerator(void);

   void setOutputDirectory(const QString& directory);
   void setPrefix(const QString& prefix);
   void setSuffix(const QString& suffix);
   void setType(const QString& type);
   void setDescription(const QString& desc);
   void setLicense(const QString& desc);

   bool run(void);

protected:
   bool generateCMakeLists(void);
   bool generateTypeHeaderFile(void);
   bool generateTypeSourceFile(void);
   bool generatePluginConfigFile(void);
   bool generatePluginHeaderFile(void);
   bool generatePluginSourceFile(void);
   bool generateExportHeaderFile(void);
   bool generateHelpCollectionFile(void);
   bool generateHelpConfigurationFile(void);

   bool generateReadmeFile(void);
   bool generateCopyingFile(void);

private:
   dtkPluginGeneratorPrivate *d;
};

#endif
