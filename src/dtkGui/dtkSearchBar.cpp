/* dtkSearchBar.cpp --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Thu Feb  4 11:03:21 2010 (+0100)
 * Version: $Id$
 * Last-Updated: Thu Feb  4 16:45:43 2010 (+0100)
 *           By: Julien Wintz
 *     Update #: 63
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#include "dtkSearchBar.h"

// /////////////////////////////////////////////////////////////////
// dtkSearchBarButton
// /////////////////////////////////////////////////////////////////

class dtkSearchBarButtonPrivate
{
public:
    void drawRoundRect(QPainter *painter, const QRectF& rect, qreal radius)
    {
        painter->drawPath(roundRectangle(rect, radius, radius, radius, radius));
    }
    
    void drawRoundRect(QPainter *painter, const QRectF& rect, qreal leftRadius, qreal rightRadius) 
    {
        painter->drawPath(roundRectangle(rect, leftRadius, leftRadius, rightRadius, rightRadius));
    }
    
    void drawRoundRect(QPainter *painter, const QRectF& rect, qreal leftTopRadius, qreal leftBottomRadius, qreal rightTopRadius, qreal rightBottomRadius)
    {
        painter->drawPath(roundRectangle(rect, leftTopRadius, leftBottomRadius, rightTopRadius, rightBottomRadius));
    }
    
    void fillRoundRect(QPainter *painter, const QRectF& rect, qreal radius, const QBrush& brush)
    {
        painter->fillPath(roundRectangle(rect, radius, radius, radius, radius), brush);
    }
    
    void fillRoundRect(QPainter *painter, const QRectF& rect, qreal leftRadius, qreal rightRadius, const QBrush& brush)
    {
        painter->fillPath(roundRectangle(rect, leftRadius, leftRadius, rightRadius, rightRadius), brush);
    }
    
    void fillRoundRect(QPainter *painter, const QRectF& rect, qreal leftTopRadius, qreal leftBottomRadius, qreal rightTopRadius, qreal rightBottomRadius, const QBrush& brush)
    {
        painter->fillPath(roundRectangle(rect, leftTopRadius, leftBottomRadius, rightTopRadius, rightBottomRadius), brush);
    }
    
    QPainterPath roundRectangle(const QRectF& rect, qreal radius)
    {
	return roundRectangle(rect, radius, radius, radius, radius);
    }
    
    QPainterPath roundRectangle(const QRectF& rect, qreal leftRadius, qreal rightRadius)
    {
        return roundRectangle(rect, leftRadius, leftRadius, rightRadius, rightRadius);
    }
    
    QPainterPath roundRectangle(const QRectF& rect, qreal leftTopRadius, qreal leftBottomRadius, qreal rightTopRadius, qreal rightBottomRadius)
    {
        QPainterPath path(QPoint(rect.left(), rect.top() + leftTopRadius));
        path.quadTo(rect.left(), rect.top(), rect.left() + leftTopRadius, rect.top());
        path.lineTo(rect.right() - rightTopRadius, rect.top());
        path.quadTo(rect.right(), rect.top(), rect.right(), rect.top() + rightTopRadius);
        path.lineTo(rect.right(), rect.bottom() - rightBottomRadius);
        path.quadTo(rect.right(), rect.bottom(), rect.right() - rightBottomRadius, rect.bottom());
        path.lineTo(rect.left() + leftBottomRadius, rect.bottom());
        path.quadTo(rect.left(), rect.bottom(), rect.left(), rect.bottom() - leftBottomRadius);
        path.closeSubpath();
        
        return path;
    }
    
public:
    qreal leftTopRadius;
    qreal leftBottomRadius;
    qreal rightTopRadius;
    qreal rightBottomRadius;
};

dtkSearchBarButton::dtkSearchBarButton(QWidget *parent): QAbstractButton(parent), d(new dtkSearchBarButtonPrivate)
{
    this->setRadius(10);
}

dtkSearchBarButton::dtkSearchBarButton(const QString& text, QWidget *parent) : QAbstractButton(parent), d(new dtkSearchBarButtonPrivate)
{
    this->setRadius(10);
    this->setText(text);
}

dtkSearchBarButton::~dtkSearchBarButton(void)
{
    delete d;
}

void dtkSearchBarButton::setRadius(qreal radius)
{
    d->leftTopRadius = radius;
    d->leftBottomRadius = radius;
    d->rightTopRadius = radius;
    d->rightBottomRadius = radius;
}

void dtkSearchBarButton::setRadius(qreal leftTopRadius, qreal leftBottomRadius, qreal rightTopRadius, qreal rightBottomRadius)
{
    d->leftTopRadius = leftTopRadius;
    d->leftBottomRadius = leftBottomRadius;
    d->rightTopRadius = rightTopRadius;
    d->rightBottomRadius = rightBottomRadius;
}

QSize dtkSearchBarButton::minimumSizeHint(void) const
{
    QFontMetrics fontMetrics(QFont("Arial", 8, QFont::Bold));

    int width = fontMetrics.width(text()) + 48;

    return QSize(width, 22);
}

void dtkSearchBarButton::paintEvent(QPaintEvent *event)
{
    int height = event->rect().height();
    int width = event->rect().width();
    int mh = (height / 2);
        
    QLinearGradient linearGrad;
    QColor color;

    if (isDown()) {
        linearGrad = QLinearGradient(QPointF(0, 0), QPointF(0, mh));
        linearGrad.setColorAt(0, QColor(0x6c, 0x6c, 0x6c));
        linearGrad.setColorAt(1, QColor(0x40, 0x40, 0x40));
        color = QColor(0x35, 0x35, 0x35);
    } else {
        linearGrad = QLinearGradient(QPointF(0, 0), QPointF(0, mh));
        linearGrad.setColorAt(0, QColor(0x8e, 0x8e, 0x8e));
        linearGrad.setColorAt(1, QColor(0x5c, 0x5c, 0x5c));
        color = QColor(0x41, 0x41, 0x41);
    }
    
    QPainter p(this);
    p.setRenderHints(QPainter::Antialiasing);
    p.setPen(QPen(QColor(0x28, 0x28, 0x28), 1));
    p.translate(5, 0);
    d->fillRoundRect(&p, QRect(0, 0, width, mh), d->leftTopRadius, 0, d->rightTopRadius, 0, QBrush(linearGrad));
    d->fillRoundRect(&p, QRect(0, mh, width, mh), 0, d->leftBottomRadius, 0, d->rightBottomRadius, color);
    d->drawRoundRect(&p, QRect(0, 0, width, height), d->leftTopRadius, d->leftBottomRadius, d->rightTopRadius, d->rightBottomRadius);
    p.translate(-5, 0);
    p.setFont(QFont("Arial", 8, QFont::Bold));
    p.setPen(QPen(QColor(0xff, 0xff, 0xff), 1));
    p.drawText(event->rect(), Qt::AlignCenter, text());
    p.end();
}

// /////////////////////////////////////////////////////////////////
// dtkSearchBar
// /////////////////////////////////////////////////////////////////

class dtkSearchBarPrivate
{
public:
    dtkSearchBarButton *button;
    QLineEdit *edit;
};

dtkSearchBar::dtkSearchBar(QWidget *parent) : QWidget(parent), d(new dtkSearchBarPrivate)
{
    d->button = new dtkSearchBarButton("Search:", this);
    d->button->setRadius(10, 10, 0, 0);
    
    d->edit = new QLineEdit(this);
    d->edit->setAttribute(Qt::WA_MacShowFocusRect, false);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->addWidget(d->button);
    layout->addWidget(d->edit);
}

dtkSearchBar::~dtkSearchBar(void)
{
    delete d;

    d = NULL;
}
