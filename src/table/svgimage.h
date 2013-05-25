#ifndef SVGIMAGE_H
#define SVGIMAGE_H

#include <QDeclarativeItem>

class QGraphicsSvgItem;

class SvgImage : public QDeclarativeItem
{
    Q_OBJECT
    
    Q_PROPERTY( int width READ width NOTIFY widthChanged );
    Q_PROPERTY( int height READ height NOTIFY heightChanged );
    Q_PROPERTY( QString source READ source WRITE setSource NOTIFY sourceChanged );
    Q_PROPERTY( QString elementId READ elementId WRITE setElementId NOTIFY elementIdChanged );
    
    QString mSource;
    QString mElementId;
    
    QGraphicsSvgItem* mSvgItem;

public:
    SvgImage( QDeclarativeItem *parent = 0 );
    
    int width();
    int height();
    QString source(){ return mSource; }
    QString elementId(){ return mElementId; }
    
    void setSource( QString source );
    void setElementId( QString elementId );
    
signals:
    void widthChanged();
    void heightChanged();
    void sourceChanged();
    void elementIdChanged();
    
};

QML_DECLARE_TYPE( SvgImage )

#endif //SVGIMAGE_H
