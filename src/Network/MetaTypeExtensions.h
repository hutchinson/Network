//
//  MetaTypeExtensions.qt.h
//  Network
//
//  Created by Dan Hutchinson on 31/10/2014.
//
//

#ifndef Network_MetaTypeExtensions_qt_h
#define Network_MetaTypeExtensions_qt_h

#include "Model/Host.h"
#include "Model/Packet.h"
#include <vector>

#include <QtGlobal>
#include <QMetaType>

Q_DECLARE_METATYPE(netviz::HostSP);
Q_DECLARE_METATYPE(netviz::PacketSP);
Q_DECLARE_METATYPE(uint64_t);

void declareQtMetaTypeExtensions();

#endif
