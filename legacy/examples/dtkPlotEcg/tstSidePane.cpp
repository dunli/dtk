/* tstSidePane.cpp --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Wed Jun  8 09:43:47 2011 (+0200)
 * Version: $Id$
 * Last-Updated: Thu Jun 27 12:02:03 2013 (+0200)
 *           By: Selim Kraria
 *     Update #: 145
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#include "tstSidePane.h"
#include <dtkGui/dtkColorButton.h>

#include <QtGui>


class tstSidePanePrivate
{
};

tstSidePane::tstSidePane(QWidget *parent) : QFrame(parent), d(new tstSidePanePrivate)
{
    dtkColorButton *bg_color_button = new dtkColorButton(this);
    bg_color_button->setColor(Qt::white);

    dtkColorButton *fg_color_button = new dtkColorButton(this);
    fg_color_button->setColor(Qt::black);
    
    QComboBox *curve_render_combo = new QComboBox(this);
    curve_render_combo->addItem("Raw");
    curve_render_combo->addItem("Antialiased");
    curve_render_combo->setFocusPolicy(Qt::NoFocus);

    QFormLayout *canvas_layout = new QFormLayout;
    canvas_layout->addRow("Background color", bg_color_button);
    canvas_layout->addRow("Foreground color", fg_color_button);
    
    QGroupBox *canvas_group = new QGroupBox("Canvas", this);
    canvas_group->setLayout(canvas_layout);

    QFormLayout *curve_layout = new QFormLayout;
    curve_layout->addRow("Render hints", curve_render_combo);
    
    QGroupBox *curve_group = new QGroupBox("Curve", this);
    curve_group->setLayout(curve_layout);

    QFormLayout *panning_layout = new QFormLayout;

    QGroupBox *panning_group = new QGroupBox("Panning", this);
    panning_group->setCheckable(true);
    panning_group->setChecked(false);
    panning_group->setLayout(panning_layout);

    QFormLayout *picking_layout = new QFormLayout;

    QGroupBox *picking_group = new QGroupBox("Picking", this);
    picking_group->setCheckable(true);
    picking_group->setChecked(false);
    picking_group->setLayout(picking_layout);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(canvas_group);
    layout->addWidget(curve_group);
    layout->addWidget(panning_group);
    layout->addWidget(picking_group);
    layout->addStretch(1);

    connect(bg_color_button, SIGNAL(colorChanged(const QColor&)), this, SIGNAL(backgroundColorChanged(const QColor&)));
    connect(fg_color_button, SIGNAL(colorChanged(const QColor&)), this, SIGNAL(foregroundColorChanged(const QColor&)));

    connect(curve_render_combo, SIGNAL(currentIndexChanged(int)), this, SIGNAL(curveRenderModeChanged(int)));

    connect(panning_group, SIGNAL(toggled(bool)), this, SIGNAL(activatePanning(bool)));
    connect(picking_group, SIGNAL(toggled(bool)), this, SIGNAL(activatePicking(bool)));
}

tstSidePane::~tstSidePane(void)
{
    delete d;

    d = NULL;
}
