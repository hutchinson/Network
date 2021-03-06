//
//  NetworkStatsDock.cpp
//  Network
//
//  Created by Dan Hutchinson on 24/11/2014.
//
//

#include "NetworkStatsDock.h"

#include <QScrollArea>
#include <QVBoxLayout>
#include <QFormLayout>

#include <QDateTime>

NetworkStatsDock::NetworkStatsDock(QWidget *parent, Qt::WindowFlags flags)
: QDockWidget(parent, flags)
, _rootFrame(NULL)
, _networkStatsToolBox(NULL)
, _selectedHostName(NULL)
, _selectedHostIPv4Address(NULL)
, _packetsSent(NULL)
, _packetsReceived(NULL)
, _firstSeen(NULL)
, _globalStats()
{
  setFeatures(QDockWidget::NoDockWidgetFeatures);

  // Set up the tool box.
  _rootFrame = new QWidget(this);
  
  _networkStatsToolBox = new QToolBox(this);

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
  _networkStatsToolBox->addItem(networkByPacketType, tr("Traffic Summary"));

  //////////////////////////////////////////////////////////////////////////////
  
  QWidget *selectedHostDetails = new QWidget(this);
  selectedHostDetails->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  QFormLayout *selectedHostDetailsLayout = new QFormLayout();
  
  _selectedHostName = new QLabel(tr("-"));
  _selectedHostName->setTextFormat(Qt::RichText);
  _selectedHostName->setTextInteractionFlags(Qt::TextBrowserInteraction);
  _selectedHostName->setOpenExternalLinks(true);
  
  _selectedHostIPv4Address = new QLabel(tr("-"));
  
  _packetsSent = new QLabel(tr("-"));
  _packetsReceived = new QLabel(tr("-"));

  _firstSeen = new QLabel(tr("-"));
  
  selectedHostDetailsLayout->addRow(new QLabel(tr("Host Name")), _selectedHostName);
  selectedHostDetailsLayout->addRow(new QLabel(tr("IPv4")), _selectedHostIPv4Address);
  selectedHostDetailsLayout->addRow(new QWidget());
  
  selectedHostDetailsLayout->addRow(new QLabel(tr("Packets Sent")), _packetsSent);
  selectedHostDetailsLayout->addRow(new QLabel(tr("Packets Received")), _packetsReceived);
  selectedHostDetailsLayout->addRow(new QWidget());
  
  selectedHostDetailsLayout->addRow(new QLabel(tr("First Seen")), _firstSeen);
  
  selectedHostDetails->setLayout(selectedHostDetailsLayout);
  _networkStatsToolBox->addItem(selectedHostDetails, tr("Host Detail"));
  
  //////////////////////////////////////////////////////////////////////////////
  
  QVBoxLayout *rootLayout = new QVBoxLayout();
  rootLayout->addWidget(_networkStatsToolBox);
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

void NetworkStatsDock::selectedHostChanged(netviz::HostSP selectedHost)
{
  if(selectedHost)
  {
    _selectedHostName->setText(QString("<a href=\"http://www.whois.com/whois/%1\">%1</a>").arg(selectedHost->hostName().c_str()));
    _selectedHostIPv4Address->setText(QString(selectedHost->hostIP().c_str()));
    _packetsSent->setText(QString("%1").arg(selectedHost->packetsSent()));
    _packetsReceived->setText(QString("%1").arg(selectedHost->packetsReceived()));
    _firstSeen->setText(QString("%1").arg(QDateTime::fromTime_t(static_cast<uint>(selectedHost->firstSeen())).toString()));
//    _networkStatsToolBox->setCurrentIndex(1);
  }
  else
  {
    _selectedHostName->setText(tr("-"));
    _selectedHostIPv4Address->setText(tr("-"));
    _packetsSent->setText(tr("-"));
    _packetsReceived->setText(tr("-"));
    _firstSeen->setText(tr("-"));
  }
}
