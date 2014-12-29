SET(NETWORK_HEADERS Map/HostPlacementStrategy.h
                    Controller.qt.h
                    DrawOptionsDock.qt.h
                    GlobalThings.h
                    Grid.qt.h
                    HostGraphicsItem.qt.h
                    MainWindow.qt.h
                    MetaTypeExtensions.h
                    NetworkStatsDock.qt.h
                    NetworkView.qt.h
                    PcapHelpers.h
                    )

SET(NETWORK_SOURCE Map/HostPlacementStrategy.cpp
                   Controller.cpp
                   DrawOptionsDock.cpp
                   GlobalThings.h
                   Grid.cpp
                   HostGraphicsItem.cpp
                   main.cpp
                   MainWindow.cpp
                   MetaTypeExtensions.cpp
                   NetworkStatsDock.cpp
                   NetworkView.cpp
                   PcapHelpers.cpp
                  )

# Tell CMake to run moc when necessary:
SET(CMAKE_AUTOMOC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Widgets finds its own dependencies.
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5OpenGL REQUIRED)

INCLUDE_DIRECTORIES(${THIRDPARTY_ZMQ_INCLUDE})

ADD_EXECUTABLE(Network ${NETWORK_HEADERS} ${NETWORK_SOURCE})

TARGET_LINK_LIBRARIES(Network Model)
TARGET_LINK_LIBRARIES(Network PacketController)
TARGET_LINK_LIBRARIES(Network pcap)
TARGET_LINK_LIBRARIES(Network ${THIRDPARTY_ZMQ_LIB_DIR}/libzmq.a)

QT5_USE_MODULES(Network Widgets)
QT5_USE_MODULES(Network OpenGL)

