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

class QAction;
class QComboBox;
class QPushButton;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow();

protected:
  void closeEvent(QCloseEvent *event);

private slots:
  void startListening();
  void stopListening();
  
private:
  void _createActions();
  void _createToolBar();
  void _readSettings();

  void _populateAvailableInterfaces();
  
  QAction *_startListeningAction;
  QAction *_stopListeningAction;

  QToolBar *_interfaceSelectToolBar;
  QComboBox *_interfaceComboBox;
  QPushButton *_startListeningButton;
  QPushButton *_stopListeningButton;
};


#endif
