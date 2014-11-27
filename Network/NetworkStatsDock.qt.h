//
//  NetworkStatsDock.qt.h
//  Network
//
//  Created by Dan Hutchinson on 24/11/2014.
//
//

#ifndef Network_NetworkStatsDock_qt_h
#define Network_NetworkStatsDock_qt_h

#include "PacketFormat.h"

#include "GlobalThings.h"

#include <QDockWidget>
#include <QWidget>
#include <QProgressBar>
#include <QPaintEvent>
#include <QPainter>
#include <QColor>

#include <vector>

#define BAR_BORDER 2.0f

class PercentageBar : public QProgressBar
{
public:
  PercentageBar(const QString &text, QWidget *parent = 0)
  : QProgressBar(parent)
  , _dataLabel(text)
  , _mouseIsOver(false)
  {
    setMinimumWidth(MINIMUM_DOCK_WIDTH);

    setMinimum(0);
    setValue(0);
    setMaximum(100);

    setTextVisible(true);

  }

  void enterEvent(QEvent *event)
  {
    _mouseIsOver = true;
    this->update();
  }
  
  void leaveEvent(QEvent *event)
  {
    _mouseIsOver = false;
    this->update();
  }
  
  void paintEvent(QPaintEvent *event)
  {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    QRect thisRect = rect();
    
    QRectF borderRect(thisRect.left() + 2, thisRect.top() + 2, thisRect.width() - 4, thisRect.height() - 4);
    QPen border(Qt::SolidLine);
    border.setColor(Qt::gray);
    painter.setPen(border);
    painter.drawRoundedRect(borderRect, thisRect.height() / 2, thisRect.height() / 2);
    
    QRectF insideRect(thisRect.left() + BAR_BORDER, thisRect.top() + BAR_BORDER,
                      thisRect.width() - (2 * BAR_BORDER), thisRect.height() - (2 * BAR_BORDER));

    double value = (1.0f * this->value()) / maximum();
    if(value > 0.01)
    {
      double length = value * width();
      QBrush internal(Qt::SolidPattern);
      internal.setColor(QColor(0, 0, 255, 200));
      painter.setBrush(internal);

      insideRect.setRight(length);
      painter.drawRoundedRect(insideRect, thisRect.height() / 2, thisRect.height() / 2);
    }
    
    if(_mouseIsOver)
    {
      QString text("%1 - %2%");
      text = text.arg(_dataLabel).arg(100.0f * value, 0, 'f', 2);
      painter.setPen(Qt::gray);

      QPointF offset(-10.0f, 1 + BAR_BORDER);
      QPointF textLocation = insideRect.bottomLeft();
      textLocation -= offset;

      painter.drawText(textLocation, text);
    }
  }
  
private:
  QString _dataLabel;
  bool _mouseIsOver;
};

class NetworkStatsDock : public QDockWidget
{
  Q_OBJECT
  
public:
  NetworkStatsDock(QWidget *parent = 0, Qt::WindowFlags flags = 0);
  
  void updatePacketStats(uint64_t totalPackets, const uint64_t* packetTypeBreakdown);

private:
  QWidget *_rootFrame;

  std::vector<PercentageBar*> _globalStats;

};

#endif
