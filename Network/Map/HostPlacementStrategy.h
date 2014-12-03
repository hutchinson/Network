//
//  HostPlacementStrategy.h
//  Network
//
//  Created by Dan Hutchinson on 03/12/2014.
//
//

#ifndef Network_HostPlacementStrategy_h
#define Network_HostPlacementStrategy_h

#include "Model/Host.h"
#include <memory>

#include <QRect>
#include <QString>

class NetworkView;

//////////////////////////////////////////////////////////////////////////////


// Subclass HostPlacementStrategy if you'd like to create a new way of placing
// hosts in the host map.
//
// The rules you must obey are simple:
//
// - You'll be called by the NetworkView each time we either get notified a new
// host has been detected in the network, or, the map is being resized and your
// given a chance to change the hosts position based on the new extents of the
// map.
//
// - When you return from positionForHost your method must ensure that
// theMap.isSpaceOccupiedByHost(where) == false
// otherwise we'll crash your app =P
class HostPlacementStrategy
{
public:
  virtual ~HostPlacementStrategy();
  virtual const QString strategyName() = 0;
  virtual void positionForHost(const NetworkView &theMap, const netviz::HostSP host, QRectF &where) = 0;
};
typedef std::shared_ptr<HostPlacementStrategy> HostPlacementStrategySP;

//////////////////////////////////////////////////////////////////////////////

class QuadrantPlacementStrategy : public HostPlacementStrategy
{
public:
  // Create an instance of the QuadrantPlacementStrategy and return it.
  static HostPlacementStrategySP create();

  virtual ~QuadrantPlacementStrategy();
  virtual const QString strategyName();
  virtual void positionForHost(const NetworkView &theMap, const netviz::HostSP host, QRectF &where);
  
private:
  QuadrantPlacementStrategy();

  void _offsetBox(QRectF &box, uint32_t octet, qreal currentXOffset, qreal currentYOffset) const;
};

#endif
