#ifndef SVGCARD_H
#define SVGCARD_H

#include <QtSvg/QGraphicsSvgItem>

class QSvgRenderer;
class Animation;

class SvgCard : public QGraphicsSvgItem
{
    Q_OBJECT
    
    Animation* mAnimation;
    
    bool mSelectable;
    
protected:
    void mousePressEvent( QGraphicsSceneMouseEvent * );
    //
    void hoverEnterEvent( QGraphicsSceneHoverEvent * );
    void hoverLeaveEvent( QGraphicsSceneHoverEvent * );
    //
    
public:
    SvgCard( QSvgRenderer*, int/*, QString cardText = "back"*/ );
    ~SvgCard();
    
    Animation* getAnimation(){ return mAnimation; }
    
    void setSelectable( bool );
    bool isSelectable(){ return mSelectable; }
    
    //
    static QString EmptyCard(){ return "back"; }
    //
    
signals:
    void signalMouseEnter( SvgCard* );
    void signalMouseLeave( SvgCard* );
    
    void signalClick( SvgCard* );
    void signalClick();
    
};

#endif //SVGCARD_H
