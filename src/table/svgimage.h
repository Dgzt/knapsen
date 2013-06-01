#ifndef SVGIMAGE_H
#define SVGIMAGE_H

#include <QDeclarativeItem>

class QGraphicsSvgItem;

class SvgImage : public QDeclarativeItem
{
    Q_OBJECT
    
    Q_PROPERTY( QString source READ source WRITE setSource NOTIFY sourceChanged );
    Q_PROPERTY( QString elementId READ elementId WRITE setElementId NOTIFY elementIdChanged );
    Q_PROPERTY( qreal scale READ scale WRITE setScale NOTIFY scaleChanged );
    
    QString mSource;
    QString mElementId;
    QGraphicsSvgItem* mSvgItem;
    
//
/*protected:
    virtual void hoverEnterEvent( QGraphicsSceneHoverEvent * event );
    virtual void hoverLeaveEvent( QGraphicsSceneHoverEvent * event );*/
//
    
public:
    SvgImage( QDeclarativeItem *parent = 0 );

    QString source(){ return mSource; }
    QString elementId(){ return mElementId; }
    qreal scale() const;
    
    //
    //bool mouseOnEnter() const;
    //bool mouseOnExit() const;
    //bool isContainsMouse() const;
    //

    void setSource( QString source );
    void setElementId( QString elementId );
    void setScale( qreal scale );
    
signals:
    void sourceChanged();
    void elementIdChanged();
    void scaleChanged();
    
    //
    //void signalMouseEntered();
    //void signalMouseExited();
    //
    //void containsMouseChanged();
    //
};

QML_DECLARE_TYPE( SvgImage )

#endif //SVGIMAGE_H
