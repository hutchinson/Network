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

#include <QtGlobal>
#include <QMetaType>

Q_DECLARE_METATYPE(netviz::HostSP);

void declareQtMetaTypeExtensions();

#endif
