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
#include "Model/Packet.h"

#include "HostGraphicsItem.qt.h"
#include "Grid.qt.h"
#include "PacketController/PacketFormat.h"

#include "Map/HostPlacementStrategy.h"

#include <iostream>
#include <memory>
#include <list>
#include <vector>

#include <assert.h>

class QRectF;

////////////////////////////////////////////////////////////////////////////////

class NetworkView : public QGraphicsView
{
  Q_OBJECT

public:
  NetworkView(QWidget *parent = 0);
  
  void newHostAdded(netviz::HostSP host);
  
  void newPacket(netviz::HostSP from, netviz::PacketSP packet, netviz::HostSP to);

  bool isSpaceOccupiedByHost(const QRectF &position) const;

  void _newMapSizeStrategy(double &newWidth, double &newHeight);

public slots:
  void settingsDidChange();

protected:
  virtual void wheelEvent(QWheelEvent* event);

private:
  QGraphicsScene *_scene;
  GridGraphicsItem *_grid;

  std::map<std::string, HostGraphicsItem*> _hostMap;
  std::list<netviz::HostSP> _unplacedHosts;

  // This will be moved to a singelton
  HostPlacementStrategySP _placementStrategy;
};

#endif
