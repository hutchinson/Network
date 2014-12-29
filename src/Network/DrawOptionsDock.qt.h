//
//  DrawOptionsDock.qt.h
//  Network
//
//  Created by Dan Hutchinson on 27/11/2014.
//
//

#ifndef Network_DrawOptionsDock_qt_h
#define Network_DrawOptionsDock_qt_h

#include <QDockWidget>

class QCheckBox;

class DrawOptionsDock : public QDockWidget
{
  Q_OBJECT
  
public:
  DrawOptionsDock(QWidget *parent = 0, Qt::WindowFlags flags = 0);

public slots:
  void settingChanged();
  
signals:
  void settingsDidChange();

private:
  QCheckBox *_showGrid;
  
  void _loadSettings();
};


#endif
