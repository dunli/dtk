/* dtk3DViewTest.h ---
 * 
 * Author: Julien Wintz
 * Created: Fri Mar 22 11:35:20 2013 (+0100)
 * Version: 
 * Last-Updated: Wed Mar 27 13:06:08 2013 (+0100)
 *           By: Julien Wintz
 *     Update #: 7
 */

/* Change Log:
 * 
 */

#pragma once

#include <dtkTest>

class dtk3DViewTestCase : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase(void);
    void init(void);

private slots:
    void testInstanciation(void);
    void testClick(void);

private slots:
    void cleanupTestCase(void);
    void cleanup(void);
};