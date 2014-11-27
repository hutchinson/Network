//
//  NetworkView.qt.h
//  Network
//
//  Created by Dan Hutchinson on 30/10/2014.
//
//

#ifndef Network_NetworkView_qt_h
#define Network_NetworkView_qt_h


#include <QGraphicsView>
#include <QGraphicsScene>

#include "Model/Host.h"
#include "HostGraphicsItem.qt.h"
#include "Grid.qt.h"
#include "PacketController/PacketFormat.h"

#include <iostream>
#include <memory>
#include <list>
#include <vector>

#include <assert.h>

class QRect;

////////////////////////////////////////////////////////////////////////////////

class NetworkView : public QGraphicsView
{
  Q_OBJECT

public:
  NetworkView(QWidget *parent = 0);
  
  void newHostAdded(netviz::HostSP host);

  void _offsetBox(QRect &box, uint32_t octet, qreal currentXOffset, qreal currentYOffset) const;
  void _determineCandidatePositionFor(const netviz::HostSP host, QRect &where, qreal initialWidth, qreal initialHeight) const;

  bool _isSpaceOccupiedByHost(const QRect &position);
  void _newMapSizeStrategy(double &newWidth, double &newHeight);
  
public slots:
  void settingsDidChange();

protected:
  virtual void wheelEvent(QWheelEvent* event);

private:
  QGraphicsScene *_scene;
  GridGraphicsItem *_grid;
  std::map<HostGraphicsItem*, netviz::HostSP> _hostMap;
  std::list<netviz::HostSP> _unplacedHosts;
};

#endif
