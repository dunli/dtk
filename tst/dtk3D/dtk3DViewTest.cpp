/* dtk3DViewTest.cpp ---
 * 
 * Author: Julien Wintz
 * Created: Fri Mar 22 11:35:55 2013 (+0100)
 * Version: 
 * Last-Updated: Fri Apr  5 19:27:24 2013 (+0200)
 *           By: Julien Wintz
 *     Update #: 167
 */

/* Change Log:
 * 
 */

#include "dtk3DViewTest.h"

#include <dtk3D>

#include <Qt3D/QGLBuilder>
#include <Qt3D/QGLCube>
#include <Qt3D/QGLSphere>
#include <Qt3D/QGLTeapot>

void dtk3DViewTestCase::initTestCase(void)
{

}

void dtk3DViewTestCase::init(void)
{

}

void dtk3DViewTestCase::testInstanciation(void)
{
    dtk3DView view;
    view.setTitle("Hit 'Q' key to validate test");
    view.resize(800, 600);
    view.show();
    view.raise();

    QEventLoop loop;
    connect(&view, SIGNAL(quit()), &loop, SLOT(quit()));
    loop.exec();
}

void dtk3DViewTestCase::testClick(void)
{
    dtk3DItem *teapot = new dtk3DItem; {
	QGLBuilder builder;
	builder << QGL::Smooth;
	builder << QGLTeapot();
	teapot->addNode(builder.finalizedSceneNode());
	teapot->setEffect(QGL::LitMaterial);
	teapot->setColor(Qt::red);
    }

    dtk3DItem *sphere = new dtk3DItem; {
	QGLBuilder builder;
	builder << QGL::Faceted;
	builder << QGLSphere();
	sphere->addNode(builder.finalizedSceneNode());
        sphere->translate(QVector3D(-2.5, 0.0, 0.0));
	sphere->setEffect(QGL::LitMaterial);
	sphere->setColor(Qt::green);
    }

    dtk3DItem *cube = new dtk3DItem; {
        QGLBuilder builder;
        builder << QGL::Faceted;
        builder << QGLCube();
        cube->addNode(builder.finalizedSceneNode());
        cube->setFlag(dtk3DItem::Interactive, true);
        cube->translate(QVector3D(2.5, 0.0, 0.0));
	cube->setEffect(QGL::LitMaterial);
	cube->setColor(Qt::blue);
    }

    dtk3DScene scene;
    scene.addItem(teapot);
    scene.addItem(sphere);
    scene.addItem(cube);
    
    dtk3DView view;
    view.setScene(&scene);
    view.setTitle("Click the cube item to validate test");
    view.resize(1600, 800);
    view.show();
    view.raise();

    QEventLoop loop;
    connect(cube, SIGNAL(clicked()), &loop, SLOT(quit()));
    loop.exec();
}

void dtk3DViewTestCase::cleanupTestCase(void)
{

}

void dtk3DViewTestCase::cleanup(void)
{

}

DTKTEST_MAIN_GUI(dtk3DViewTest, dtk3DViewTestCase)