/* dtkComposerNodeCase.cpp --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Mon Feb 28 13:03:58 2011 (+0100)
 * Version: $Id$
 * Last-Updated: Wed Apr 27 21:30:53 2011 (+0200)
 *           By: Thibaud Kloczko
 *     Update #: 560
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#include "dtkComposerEdge.h"
#include "dtkComposerNodeCase.h"
#include "dtkComposerNodeCase_p.h"
#include "dtkComposerNodeControlBlock.h"
#include "dtkComposerNodeLoop.h"
#include "dtkComposerNodeProperty.h"
#include "dtkComposerScene.h"

#include <dtkCore/dtkGlobal.h>

// #define DTK_DEBUG_COMPOSER_INTERACTION 1
// #define DTK_DEBUG_COMPOSER_EVALUATION 1

// /////////////////////////////////////////////////////////////////
// dtkComposerNodeCaseButton
// /////////////////////////////////////////////////////////////////

class dtkComposerNodeCaseButton : public QGraphicsItem
{
public:
     dtkComposerNodeCaseButton(dtkComposerNodeCase *parent, dtkComposerNodeCasePrivate *parent_d);
    ~dtkComposerNodeCaseButton(void);

public:
    QRectF boundingRect(void) const;

public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

public:
    dtkComposerNodeCase *parent_node;
    dtkComposerNodeCasePrivate *parent_node_d;

    QPainterPath path;
    QString text;
};

dtkComposerNodeCaseButton::dtkComposerNodeCaseButton(dtkComposerNodeCase *parent, dtkComposerNodeCasePrivate *parent_d) : QGraphicsItem(parent)
{
    int margin = 10;
    int length = 30;
    int height = 10;
    int radius =  5;
    int origin_x = -(length + margin) / 2;
    int origin_y = parent->boundingRect().height() / 2;

    QPainterPath b; b.addRoundedRect(origin_x,              origin_y, margin,          -height,     radius, radius);
    QPainterPath c; c.addRoundedRect(origin_x + margin,     origin_y, length - margin, -height,     radius, radius);
    QPainterPath d; d.addRoundedRect(origin_x + length,     origin_y, margin,          -height,     radius, radius);
    QPainterPath e; e.addRoundedRect(origin_x + margin / 2, origin_y, length,          -height / 2,      0,      0);

    this->path = c.united(e.subtracted(b.united(c.united(d))));

    this->parent_node = parent;
    this->parent_node_d = parent_d;

    this->text = "+";
}

dtkComposerNodeCaseButton::~dtkComposerNodeCaseButton(void)
{    

}

QRectF dtkComposerNodeCaseButton::boundingRect(void) const
{
    return this->path.boundingRect();
}

void dtkComposerNodeCaseButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QLinearGradient gradient(option->rect.left(), option->rect.top(), option->rect.left(), option->rect.bottom());
    gradient.setColorAt(0.0, QColor("#bbbbbb"));
    gradient.setColorAt(0.3, QColor("#333333"));
    gradient.setColorAt(1.0, QColor("#222222"));

    painter->setPen(QPen(Qt::black, 1));
    painter->setBrush(gradient);
    painter->drawPath(path);
    
#if defined(Q_WS_MAC)
    QFont font("Lucida Grande", 8);
#else
    QFont font("Lucida Grande", 6);
#endif
    font.setBold(true);

    QFontMetrics metrics(font);
    
    painter->setFont(font);
    painter->setPen(Qt::white);
    painter->drawText(this->boundingRect(), Qt::AlignCenter, text);
}

void dtkComposerNodeCaseButton::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    QString block_name = QString("case%1").arg(parent_node_d->block_cases.count());
    
    this->parent_node->addBlock(block_name);  
}

// /////////////////////////////////////////////////////////////////
// dtkComposerNodeCase
// /////////////////////////////////////////////////////////////////

dtkComposerNodeCase::dtkComposerNodeCase(dtkComposerNode *parent) : dtkComposerNodeControl(parent), d(new dtkComposerNodeCasePrivate)
{
    d->add_button = new dtkComposerNodeCaseButton(this, d);
    d->add_button->setPos(0, 0);
    d->add_button->setZValue(this->zValue() + 1);

    d->block_default = this->addBlock("default");

    this->setColor(QColor("#922891"));
    this->setInputPropertyName("variable");
    this->setTitle("Case");
    this->setType("dtkComposerCase");
}

dtkComposerNodeCase::~dtkComposerNodeCase(void)
{
    delete d;

    d = NULL;
}

dtkComposerNodeControlBlock *dtkComposerNodeCase::addBlock(const QString& title)
{
    dtkComposerNodeControlBlock *block = dtkComposerNodeControl::addBlock(title);

    if (title == "default") {

        block->setInteractive(true);

        dtkComposerNodeProperty *i_variable = block->addInputProperty("variable", this);
        i_variable->setBlockedFrom(title);
        this->addInputProperty(i_variable);
    
        dtkComposerNodeProperty *o_variable = block->addOutputProperty("variable", this);
        o_variable->setBlockedFrom(title);
        this->addOutputProperty(o_variable);

    } else {

        block->setInteractive(true);

        block->setRemoveButtonVisible(true);

        dtkComposerNodeProperty *i_constant = block->addInputProperty("constant", this);
        i_constant->setBlockedFrom(title);
        this->addInputProperty(i_constant);

        dtkComposerNodeProperty *i_variable = block->addInputProperty("variable", this);
        i_variable->setBlockedFrom(title);
        this->addInputProperty(i_variable);
    
        dtkComposerNodeProperty *o_variable = block->addOutputProperty("variable", this);
        o_variable->setBlockedFrom(title);
        this->addOutputProperty(o_variable);
    
        d->block_cases << block;

    }

    return block;
}

int dtkComposerNodeCase::removeBlock(dtkComposerNodeControlBlock *block, bool clean)
{
    dtkComposerNodeControl::removeBlock(block, false);

    int removed_blocks = 0;

    if (d->block_cases.contains(block)) {

        dtkComposerScene *scene = dynamic_cast<dtkComposerScene *>(this->scene());
        if(!scene)
            return 0;

        foreach(dtkComposerNode *child, block->nodes())
            scene->removeNode(child);

        removed_blocks = d->block_cases.removeAll(block);

        foreach(dtkComposerNodeProperty *property, block->inputProperties()) {
       
            foreach(dtkComposerEdge *edge, this->inputEdges()) {
                if(edge->destination() == property) {
                    this->removeInputEdge(edge);
                    scene->removeEdge(edge);
                }
            }
       
            foreach(dtkComposerEdge *edge, this->inputRelayEdges()) {
                if(edge->source() == property) {
                    this->removeInputRelayEdge(edge);
                    scene->removeEdge(edge);
                }
            }

            this->removeInputProperty(property);
            delete property;
        }
        
        foreach(dtkComposerNodeProperty *property, block->outputProperties()) {
       
            foreach(dtkComposerEdge *edge, this->outputEdges()) {
                if(edge->source() == property) {
                    this->removeOutputEdge(edge);
                    scene->removeEdge(edge);
                }
            }
       
            foreach(dtkComposerEdge *edge, this->outputRelayEdges()) {
                if(edge->destination() == property) {
                    this->removeOutputRelayEdge(edge);
                    scene->removeEdge(edge);
                }
            }
            
            this->removeOutputProperty(property);
            delete property;
        }
        
        if (clean) {
            delete block;
            this->layout();
        }
    }

    return removed_blocks;
}

int dtkComposerNodeCase::removeBlock(const QString& title)
{
    foreach(dtkComposerNodeControlBlock *block, d->block_cases)
        if(block->title() == title)
            return this->removeBlock(block, true);

    return 0;
}

void dtkComposerNodeCase::layout(void)
{
    dtkComposerNodeControl::layout();  

    int i = 0;
    int j;
    
    foreach(dtkComposerNodeControlBlock *block, this->blocks()) {

        block->setRect(QRectF(this->boundingRect().x(),
                              this->boundingRect().y() + 23 + i * ((this->boundingRect().height() - 46) / this->blocks().count()),
                              this->boundingRect().width(),
                              (this->boundingRect().height() - 46) / this->blocks().count()));

        j = 0;
        foreach(dtkComposerNodeProperty *property, block->inputProperties()) {

            property->setRect(QRectF(block->rect().left() + this->nodeRadius(),
                                     block->rect().top()  + this->nodeRadius() * (4*j + 1),
                                     2 * this->nodeRadius(),
                                     2 * this->nodeRadius() ));

            if (property->name() == "variable") {
                property->mirror();
                j++;
            }

            j++;
        }
        if (block->title() == "default")
            j = 0;
        else
            j = 1;
        foreach(dtkComposerNodeProperty *property, block->outputProperties()) {

            property->setRect(QRectF(block->rect().right() - this->nodeRadius() * 3,
                                     block->rect().top()   + this->nodeRadius() * (4*j + 1),
                                     2 * this->nodeRadius(),
                                     2 * this->nodeRadius() ));

            if (property->name() == "variable")
                j++;

            j++;
        }

        i++;
    } 

    d->add_button->setPos(this->boundingRect().width()/2 - 150/2, this->boundingRect().bottom() - 200);
}

void dtkComposerNodeCase::update(void)
{
#if defined(DTK_DEBUG_COMPOSER_EVALUATION)
        qDebug() << DTK_PRETTY_FUNCTION << this;
#endif

    if (!this->isRunning()) {

        if (!this->dirty())
            return;

        // -- Check dirty input value

        if (this->dirtyInputValue())
            return;

#if defined(DTK_DEBUG_COMPOSER_EVALUATION)
        qDebug() << DTK_COLOR_BG_GREEN << DTK_PRETTY_FUNCTION << "Dirty input value OK" << DTK_NO_COLOR;
#endif

        // -- Retrieve input value

        QVariant value = dtkComposerNodeControl::value();

        if (!value.isValid())
            return;

#if defined(DTK_DEBUG_COMPOSER_EVALUATION)
        qDebug() << DTK_COLOR_BG_GREEN << DTK_PRETTY_FUNCTION << "Input value valid" << DTK_NO_COLOR;
#endif

        // -- Block selection
        
        dtkComposerNodeProperty *property = NULL;
        QVariant constant;

        this->setCurrentBlock(NULL);

        foreach(dtkComposerNodeControlBlock *block, d->block_cases) {

            property = this->inputProperty(block->title(), "constant");

            if(!property)
                continue;

            property->setBehavior(dtkComposerNodeProperty::AsInput);

            if(!property->edge())
                continue;
            
            if (property->edge()->source()->node()->dirty())
                return;

            constant = property->edge()->source()->node()->value(property->edge()->source());

            if (constant == value) {
                this->setCurrentBlock(block);
                break;
            }
        }
        if (!this->currentBlock())
            this->setCurrentBlock(d->block_default);

#if defined(DTK_DEBUG_COMPOSER_EVALUATION)
        qDebug() << DTK_COLOR_BG_GREEN << DTK_PRETTY_FUNCTION << "Selected block is" << this->currentBlock()->title() << DTK_NO_COLOR;
#endif

        // -- Check dirty inputs

        if (this->dirtyUpstreamNodes())
            return;

#if defined(DTK_DEBUG_COMPOSER_EVALUATION)
        qDebug() << DTK_COLOR_BG_GREEN << DTK_PRETTY_FUNCTION << "All input nodes are clean" << DTK_NO_COLOR;
#endif

        // -- Mark dirty outputs

        this->markDirtyDownstreamNodes();

#if defined(DTK_DEBUG_COMPOSER_EVALUATION)
        qDebug() << DTK_COLOR_BG_GREEN << DTK_PRETTY_FUNCTION << "All output nodes are set to dirty" << DTK_NO_COLOR;
#endif

        // -- Clean active input routes

        foreach(dtkComposerEdge *active_route, this->inputActiveRoutes()) {
            foreach(dtkComposerEdge *i_route, active_route->destination()->node()->allRoutes()) {
                if (i_route == active_route) {
                    //active_route->destination()->node()->removeInputRoute(i_route);
                    active_route->destination()->node()->removeRoute(i_route);
                    break;
                }
            }
            this->removeInputActiveRoute(active_route);
            delete active_route;
            active_route = NULL;
        }

#if defined(DTK_DEBUG_COMPOSER_EVALUATION)
        qDebug() << DTK_COLOR_BG_YELLOW << DTK_PRETTY_FUNCTION << "Input active routes cleaned" << DTK_NO_COLOR;
#endif

        // -- Pull

        foreach(dtkComposerEdge *i_route, this->inputRoutes())
            if (i_route->destination()->blockedFrom() == this->currentBlock()->title() || i_route->destination() == this->inputProperty())
                this->pull(i_route, i_route->destination());

#if defined(DTK_DEBUG_COMPOSER_EVALUATION)
        qDebug() << DTK_COLOR_BG_YELLOW << DTK_PRETTY_FUNCTION << "Pull done" << DTK_NO_COLOR;
#endif
        
        // -- Running logics

#if defined(DTK_DEBUG_COMPOSER_EVALUATION)
    qDebug() << DTK_COLOR_BG_RED  << "Running node" << this->title() << "'s logics" << DTK_NO_COLOR;
#endif

        this->setRunning(true);
        this->run();

    } else {

        // -- Check Dirty end nodes
        
        foreach(dtkComposerEdge *o_route, this->outputRelayRoutes())
            if (o_route->destination()->blockedFrom() == this->currentBlock()->title())
                if (o_route->source()->node()->dirty())
                    return;

#if defined(DTK_DEBUG_COMPOSER_EVALUATION)
        qDebug() << DTK_COLOR_BG_GREEN << DTK_PRETTY_FUNCTION << "All end block nodes have finished their work" << DTK_NO_COLOR;
#endif

        // -- Clean active output routes

        foreach(dtkComposerEdge *active_route, this->outputActiveRoutes()) {
            foreach(dtkComposerEdge *i_route, active_route->destination()->node()->allRoutes()) {
                if (i_route == active_route) {
                    active_route->destination()->node()->removeRoute(i_route);
                    break;
                }
            }
            this->removeOutputActiveRoute(active_route);
            delete active_route;
            active_route = NULL;
        }

#if defined(DTK_DEBUG_COMPOSER_EVALUATION)
        qDebug() << DTK_COLOR_BG_YELLOW << DTK_PRETTY_FUNCTION << "Output active routes cleaned" << DTK_NO_COLOR;
#endif

        // -- Push

        foreach(dtkComposerEdge *o_route, this->outputRoutes())
            if (o_route->source()->blockedFrom() == this->currentBlock()->title())
                this->push(o_route, o_route->source());

#if defined(DTK_DEBUG_COMPOSER_EVALUATION)
        qDebug() << DTK_COLOR_BG_YELLOW << DTK_PRETTY_FUNCTION << "Push done" << DTK_NO_COLOR;
#endif

        // -- Forward

        this->setDirty(false);
        this->setRunning(false);

#if defined(DTK_DEBUG_COMPOSER_EVALUATION)
        qDebug() << DTK_COLOR_BG_BLUE << DTK_PRETTY_FUNCTION << "Forward done" << DTK_NO_COLOR;
#endif

        foreach(dtkComposerEdge *o_route, this->outputRoutes())
            if (o_route->source()->blockedFrom() == this->currentBlock()->title())
                o_route->destination()->node()->update();

    }
}

void dtkComposerNodeCase::pull(dtkComposerEdge *i_route, dtkComposerNodeProperty *property)
{
    if (property == this->inputProperty()) {
        
        foreach(dtkComposerEdge *relay_route, this->inputRelayRoutes()) {
            if (relay_route->source()->name() == "variable") {

                dtkComposerEdge *route = new dtkComposerEdge;
                route->setSource(i_route->source());
                route->setDestination(relay_route->destination());
                
                relay_route->destination()->node()->addInputRoute(route);
                this->addInputActiveRoute(route);                
            }
        }

    } else {

        foreach(dtkComposerEdge *relay_route, this->inputRelayRoutes()) {
            if (relay_route->source() == property) {
                
                dtkComposerEdge *route = new dtkComposerEdge;
                route->setSource(i_route->source());
                route->setDestination(relay_route->destination());
                
                relay_route->destination()->node()->addInputRoute(route);
                this->addInputActiveRoute(route);
            }
        }
    }
}

void dtkComposerNodeCase::run(void)
{
    foreach(dtkComposerNode *child, this->currentBlock()->nodes())
        child->setDirty(true);

    foreach(dtkComposerNode *child, this->currentBlock()->startNodes())
        child->update();
}

void dtkComposerNodeCase::push(dtkComposerEdge *o_route, dtkComposerNodeProperty *property)
{
    if (property->name() == "variable") {  

        dtkComposerNodeProperty *source = NULL;
        foreach(dtkComposerEdge *i_route, this->inputRoutes())
            if (i_route->destination() == this->inputProperty())
                source = i_route->source();

        dtkComposerEdge *route = new dtkComposerEdge;
        route->setSource(source);
        route->setDestination(o_route->destination());
        this->addOutputActiveRoute(route);
                
        if (o_route->destination()->type() == dtkComposerNodeProperty::HybridOutput || 
            o_route->destination()->type() == dtkComposerNodeProperty::PassThroughOutput || 
            (o_route->destination()->type() == dtkComposerNodeProperty::Input && o_route->destination()->node()->kind() == dtkComposerNode::Control && source->node()->isChildOfControlNode(o_route->destination()->node()))) {

            dtkComposerNodeControl *output_control_node = dynamic_cast<dtkComposerNodeControl *>(o_route->destination()->node());
            output_control_node->addOutputRelayRoute(route);
            
        } else {

            o_route->destination()->node()->addInputRoute(route);

        }

    } else {

        foreach(dtkComposerEdge *relay_route, this->outputRelayRoutes()) {
            if (relay_route->destination() == o_route->source()) {

                dtkComposerEdge *route = new dtkComposerEdge;
                route->setSource(relay_route->source());
                route->setDestination(o_route->destination());

                this->addOutputActiveRoute(route);
                
                if (o_route->destination()->type() == dtkComposerNodeProperty::HybridOutput || 
                    o_route->destination()->type() == dtkComposerNodeProperty::PassThroughOutput || 
                    (o_route->destination()->type() == dtkComposerNodeProperty::Input && o_route->destination()->node()->kind() == dtkComposerNode::Control && relay_route->source()->node()->isChildOfControlNode(o_route->destination()->node()))) {

                    dtkComposerNodeControl *output_control_node = dynamic_cast<dtkComposerNodeControl *>(o_route->destination()->node());
                    output_control_node->addOutputRelayRoute(route);

                } else {

                    o_route->destination()->node()->addInputRoute(route);

                }
            }
        }
    }
}
