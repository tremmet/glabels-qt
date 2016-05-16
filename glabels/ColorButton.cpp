/*  ColorButton.cpp
 *
 *  Copyright (C) 2014-2016  Jim Evins <evins@snaught.com>
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

#include "ColorButton.h"

#include "ColorSwatch.h"
#include <QIcon>
#include <QHBoxLayout>
#include <QtDebug>


namespace
{
	const int SWATCH_W = 24;
	const int SWATCH_H = 24;
}


ColorButton::ColorButton( QWidget* parent )
	: QPushButton( parent )
{
}


void ColorButton::init( const QString& defaultLabel, const QColor& defaultColor, const QColor&  color )
{
	mDefaultColor = defaultColor;
	mColorNode = ColorNode( color );

	setIcon( QIcon( ColorSwatch( SWATCH_W, SWATCH_H, color ) ) );
	setText( "" );
	setCheckable( true );

	mDialog = new ColorPaletteDialog( defaultLabel, defaultColor, color );

	connect( this, SIGNAL(toggled(bool)), this, SLOT(onButtonToggled(bool)) );
	connect( mDialog, SIGNAL(colorChanged(ColorNode,bool)),
		 this, SLOT(onPaletteDialogChanged(ColorNode,bool)) );
	connect( mDialog, SIGNAL(accepted()), this, SLOT(onPaletteDialogAccepted()) );
	connect( mDialog, SIGNAL(rejected()), this, SLOT(onPaletteDialogRejected()) );
}


void ColorButton::setColorNode( ColorNode colorNode )
{
	mIsDefault = false;

	mColorNode = colorNode;

	if ( colorNode.fieldFlag() )
	{
		setIcon( QIcon() );
		setText( colorNode.key() );
	}
	else
	{
		setIcon( QIcon( ColorSwatch( SWATCH_W, SWATCH_H, colorNode.color() ) ) );
		setText( "" );
	}

	mDialog->setColorNode( colorNode );
}


void ColorButton::setColor( QColor color )
{
	mIsDefault = false;

	mColorNode.setFieldFlag( false );
	mColorNode.setColor( color );
	mColorNode.setKey( "" );

	setIcon( QIcon( ColorSwatch( SWATCH_W, SWATCH_H, color ) ) );
	setText( "" );
}


void ColorButton::setToDefault()
{
	mIsDefault = true;

	mColorNode.setFieldFlag( false );
	mColorNode.setColor( mDefaultColor );
	mColorNode.setKey( "" );

	setIcon( QIcon(ColorSwatch( SWATCH_W, SWATCH_H, mDefaultColor ) ) );
	setText( "" );
}


ColorNode ColorButton::colorNode()
{
	return mColorNode;
}


void ColorButton::setKeys( const QList<QString> keyList )
{
	mDialog->setKeys( keyList );
}


void ColorButton::clearKeys()
{
	mDialog->clearKeys();
}


void ColorButton::onButtonToggled( bool checked )
{
	if ( checked )
	{
		///
		/// @TODO: improve positioning of dialog -- near edges of screen.
		///
		QPoint dialogPos( 0, height() );
		mDialog->move( mapToGlobal(dialogPos) );

		mDialog->show();
	}
}


void ColorButton::onPaletteDialogAccepted()
{
	setChecked( false );
}


void ColorButton::onPaletteDialogRejected()
{
	setChecked( false );
}


void ColorButton::onPaletteDialogChanged( ColorNode colorNode, bool isDefault )
{
	mColorNode = colorNode;
	mIsDefault = isDefault;

	setIcon( QIcon( ColorSwatch( SWATCH_W, SWATCH_H, colorNode.color() ) ) );
	setText( "" );
		
	emit colorChanged();
}
