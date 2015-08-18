/*  ColorPaletteItem.cpp
 *
 *  Copyright (C) 2014  Jim Evins <evins@snaught.com>
 *
 *  This file is part of gLabels-qt.
 *
 *  gLabels-qt is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  gLabels-qt is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with gLabels-qt.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ColorPaletteItem.h"


#include <QPainter>


//
// Private Configuration Data
//
namespace
{
	const int     border = 4;
	const int     wSwatch = 25;
	const int     hSwatch = 25;
        const QColor  hoverColor( 170, 200, 255 );
        const QColor  outlineColor( 0, 0, 0 );
        const QColor  emptyOutlineColor( 192, 192, 192 );
	const int     outlineWidthPixels = 1;
}


namespace glabels
{

	///
	/// Constructor From Data
	///
	ColorPaletteItem::ColorPaletteItem( int            id,
					    const QColor&  color,
					    const QString& tip,
					    QWidget*       parent )
		: QWidget(parent), mId(id), mColor(color), mTip(tip), mHover(false)
	{
		setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ) );
		setMinimumSize( wSwatch+2*border+1, hSwatch+2*border+1 );
		setToolTip( tip );
	}


	///
	/// Color Property Setter
	///
	void ColorPaletteItem::setColor( int            id,
					 const QColor&  color,
					 const QString& tip )
	{
		mId    = id;
		mColor = color;
		mTip   = tip;

		setToolTip( tip );
		update();
	}


	///
	/// Paint Event
	///
	void ColorPaletteItem::paintEvent( QPaintEvent* event )
	{
		QPainter painter(this);

		if ( mHover && mColor.alpha() )
		{
			painter.setBrush( QBrush( hoverColor ) );
			painter.setPen( Qt::NoPen );
			painter.drawRect( rect() );
		}

		if ( mColor.alpha() )
		{
			QPen pen( outlineColor );
			pen.setWidth( outlineWidthPixels );
			painter.setPen( pen );
		}
		else
		{
			QPen pen( emptyOutlineColor );
			pen.setWidth( outlineWidthPixels );
			painter.setPen( pen );
		}

		painter.setBrush( QBrush( mColor ) );
		painter.drawRect( border, border, wSwatch, hSwatch );
	}


	///
	/// Enter Event
	///
	void ColorPaletteItem::enterEvent( QEvent* event )
	{
		mHover = true;
		update();
	}


	///
	/// Leave Event
	///
	void ColorPaletteItem::leaveEvent( QEvent* event )
	{
		mHover = false;
		update();
	}

	
	///
	/// Mouse Press Event
	///
	void ColorPaletteItem::mousePressEvent( QMouseEvent* event )
	{
		emit activated( mId );
	}

}
