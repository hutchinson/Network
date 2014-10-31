//
//  MetaTypeExtensions.cpp
//  Network
//
//  Created by Dan Hutchinson on 31/10/2014.
//
//

#include "MetaTypeExtensions.h"

void declareQtMetaTypeExtensions()
{
  qRegisterMetaType<netviz::HostSP>("netviz::HostSP");
}
