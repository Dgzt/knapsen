#ifndef SVGIMAGE_H
#define SVGIMAGE_H

#include <QDeclarativeItem>

class QGraphicsSvgItem;

class SvgImage : public QDeclarativeItem
{
    Q_OBJECT
    
    Q_PROPERTY( QString source READ source WRITE setSource NOTIFY sourceChanged );
    Q_PROPERTY( QString elementId READ elementId WRITE setElementId NOTIFY elementIdChanged );
    
    QString mSource;
    QString mElementId;
    
    QGraphicsSvgItem* mSvgItem;
    
public:
    SvgImage( QDeclarativeItem *parent = 0 );
    
    QString source(){ return mSource; }
    QString elementId(){ return mElementId; }
    
    void setSource( QString source );
    void setElementId( QString elementId );
    
signals:
    void sourceChanged();
    void elementIdChanged();
    
};

QML_DECLARE_TYPE( SvgImage )

#endif //SVGIMAGE_H
