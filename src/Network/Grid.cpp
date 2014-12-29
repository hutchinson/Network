//
//  Grid.cpp
//  Network
//
//  Created by Dan Hutchinson on 19/11/2014.
//
//

#include "Grid.h"
#include "GlobalThings.h"

#include <QSettings>

GridGraphicsItem::GridGraphicsItem(const QRectF &rect, QGraphicsItem *parent)
: QGraphicsObject(parent)
, _drawAt(rect)
{
  setPos(0.0f, 0.0f);
}

void GridGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  QSettings settings(ORG_NAME, "Network");
  if(!settings.value("drawOptions/showgrid", true).toBool())
    return;

  painter->save();
  painter->setOpacity(0.85f);
  
  int worldWidth = _drawAt.width();
  int worldHeight = _drawAt.height();

  // TODO: Remove cell size hard coding.
  // Horizontal grid lines.
  for(int line = 0; line <= worldHeight; line += 25)
  {
    QColor gray(128, 128, 128, 255);
    if(line == (worldHeight / 2) )
      gray = QColor(255, 255,255, 255);

    QPen pen;
    pen.setColor(gray);
    pen.setWidth(1);
    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);
    
    QLineF lineToDraw(0.0f, line, worldWidth, line);
    painter->drawLine(lineToDraw);
  }
  
  // Vertical grid lines.
  for(int line = 0; line <= worldWidth; line += 25)
  {
    QColor gray(128, 128, 128, 255);
    if(line == (worldWidth / 2) )
      gray = QColor(255, 255,255, 255);
    
    QPen pen;
    pen.setColor(gray);
    pen.setWidth(1);
    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);
    
    QLineF lineToDraw(line, 0, line, worldHeight);
    painter->drawLine(lineToDraw);
  }
  
  painter->restore();
}
