#include "array.h"

Array::Array( QDeclarativeItem* parent ): 
    QDeclarativeItem( parent )
{}

int Array::size()
{
    return mArray.size();
}

void Array::add( QDeclarativeItem* item )
{
    mArray.append( item );
    
    emit sizeChanged();
}

QDeclarativeItem* Array::at( int index ) const
{
    return mArray.at( index );
}

QDeclarativeItem* Array::takeAt( int index )
{
    QDeclarativeItem* item = mArray.takeAt( index );
    
    emit sizeChanged();
    
    return item;
}
