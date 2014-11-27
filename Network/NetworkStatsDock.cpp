//
//  NetworkStatsDock.cpp
//  Network
//
//  Created by Dan Hutchinson on 24/11/2014.
//
//

#include "NetworkStatsDock.qt.h"

#include <QScrollArea>
#include <QVBoxLayout>
#include <QToolBox>

NetworkStatsDock::NetworkStatsDock(QWidget *parent, Qt::WindowFlags flags)
: QDockWidget(parent, flags)
, _rootFrame(NULL)
, _globalStats()
{
  setFeatures(QDockWidget::NoDockWidgetFeatures);

  // Set up the tool box.
  _rootFrame = new QWidget(this);
  
  QToolBox *networkStatsToolBox = new QToolBox(this);

  QWidget *networkByPacketType = new QWidget(this);
  QVBoxLayout *networkByPacketTypeLayout = new QVBoxLayout();

  _globalStats.reserve(netviz::NUM_PACKET_TYPES);
  _globalStats[netviz::TCP] = new PercentageBar(tr("TCP"));
  networkByPacketTypeLayout->addWidget( _globalStats[netviz::TCP]);

  _globalStats[netviz::UDP] = new PercentageBar(tr("UDP"));
  networkByPacketTypeLayout->addWidget(_globalStats[netviz::UDP]);
  
  _globalStats[netviz::ICMP] = new PercentageBar(tr("ICMP"));
  networkByPacketTypeLayout->addWidget(_globalStats[netviz::ICMP]);
  
  _globalStats[netviz::OTHER] = new PercentageBar(tr("Other"));
  networkByPacketTypeLayout->addWidget(_globalStats[netviz::OTHER]);

  networkByPacketType->setLayout(networkByPacketTypeLayout);
  networkStatsToolBox->addItem(networkByPacketType, tr("Packet Type Breakdown"));

  QVBoxLayout *rootLayout = new QVBoxLayout();
  rootLayout->addWidget(networkStatsToolBox);
  _rootFrame->setLayout(rootLayout);

  QScrollArea *scrollArea = new QScrollArea(this);
  scrollArea->setMinimumWidth(MINIMUM_DOCK_WIDTH);
  scrollArea->setWidget(_rootFrame);

  setWidget(scrollArea);
}

void NetworkStatsDock::updatePacketStats(uint64_t totalPackets, const uint64_t* packetTypeBreakdown)
{
  _globalStats[netviz::TCP]->setMaximum((int)totalPackets);
  _globalStats[netviz::TCP]->setValue((int)packetTypeBreakdown[netviz::TCP]);
  
  _globalStats[netviz::UDP]->setMaximum((int)totalPackets);
  _globalStats[netviz::UDP]->setValue((int)packetTypeBreakdown[netviz::UDP]);
  
  _globalStats[netviz::ICMP]->setMaximum((int)totalPackets);
  _globalStats[netviz::ICMP]->setValue((int)packetTypeBreakdown[netviz::ICMP]);
  
  _globalStats[netviz::OTHER]->setMaximum((int)totalPackets);
  _globalStats[netviz::OTHER]->setValue((int)packetTypeBreakdown[netviz::OTHER]);
}
