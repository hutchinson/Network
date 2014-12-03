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

class NetworkView;

//////////////////////////////////////////////////////////////////////////////

class HostPlacementStrategy
{
public:
  virtual ~HostPlacementStrategy();
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
  virtual void positionForHost(const NetworkView &theMap, const netviz::HostSP host, QRectF &where);
  
private:
  QuadrantPlacementStrategy();

  void _offsetBox(QRectF &box, uint32_t octet, qreal currentXOffset, qreal currentYOffset) const;
};

#endif
