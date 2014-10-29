//
//  MainWindow.qt.h
//  Network
//
//  Created by Dan Hutchinson on 29/10/2014.
//
//

#ifndef Network_MainWindow_qt_h
#define Network_MainWindow_qt_h

#include <QMainWindow>

class QComboBox;
class QPushButton;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow();

protected:
  void closeEvent(QCloseEvent *event);
  
private:
  void _createToolBar();
  void _readSettings();

  void _populateAvailableInterfaces();

  QToolBar *_interfaceSelectToolBar;
  QComboBox *_interfaceComboBox;
  QPushButton *_startListeningButton;
  QPushButton *_stopListeningButton;
};


#endif
