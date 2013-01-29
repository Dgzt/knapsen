#ifndef SVGCARD_H
#define SVGCARD_H

#include <QtSvg/QGraphicsSvgItem>

class QSvgRenderer;

class SvgCard : public QGraphicsSvgItem
{
    Q_OBJECT
    
    double mScale;
    
    bool mSelectable;
    
protected:
    void mousePressEvent( QGraphicsSceneMouseEvent * );
    //
    void hoverEnterEvent( QGraphicsSceneHoverEvent * );
    void hoverLeaveEvent( QGraphicsSceneHoverEvent * );
    //
    
public:
    SvgCard( QSvgRenderer*, double, QGraphicsItem* parentItem = 0 );
    
    //
    QSizeF getSizeF();
    //
    
    void setSelectable( bool );
    bool isSelectable(){ return mSelectable; }
    
signals:
    void signalMouseEnter( SvgCard* );
    void signalMouseLeave( SvgCard* );
    
    void signalClick( SvgCard* );
    void signalClick();
    
};

#endif //SVGCARD_H
