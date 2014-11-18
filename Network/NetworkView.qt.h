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

private:
  QGraphicsScene *_scene;
  std::map<QGraphicsItem*, netviz::HostSP> _hostMap;
};

#endif
