#ifndef ARRAY_H
#define ARRAY_H

#include <QDeclarativeItem>
#include <QList>

class QGraphicsSvgItem;

class Array : public QDeclarativeItem
{
    Q_OBJECT
    
    Q_PROPERTY( int size READ size NOTIFY sizeChanged );
    
    QList< QDeclarativeItem* > mArray;
    
public:
    Array( QDeclarativeItem *parent = 0 );
    
    int size();

public Q_SLOTS:
    void add( QDeclarativeItem* item );
    
    QDeclarativeItem* at( int index ) const;
    
    QDeclarativeItem* takeAt( int index );
    
signals:
    void sizeChanged();
    
};

QML_DECLARE_TYPE( Array )

#endif //ARRAY_H
