#ifndef SVGCARD_H
#define SVGCARD_H

#include <QtSvg/QGraphicsSvgItem>

class QSvgRenderer;

class SvgCard : public QGraphicsSvgItem
{
    Q_OBJECT
    
    double mScale;
    
    //int mId;
    //
    
    //
    
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
    
    //void setId( int );
    void setSelectable( bool );
    //
    bool isSelectable(){ return mSelectable; }
    //
    
signals:
    //void signalMouseEnter( int );
    void signalMouseEnter( SvgCard* );
    //void signalMouseLeave( int );
    void signalMouseLeave( SvgCard* );
    
    //void signalClick( int );
    void signalClick( SvgCard* );
    void signalClick();
    
};

#endif //SVGCARD_H
