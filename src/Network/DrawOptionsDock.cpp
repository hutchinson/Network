//
//  DrawOptionsDock.cpp
//  Network
//
//  Created by Dan Hutchinson on 27/11/2014.
//
//

#include "DrawOptionsDock.h"

#include "GlobalThings.h"

#include <QScrollArea>
#include <QFormLayout>
#include <QCheckBox>
#include <QSettings>

DrawOptionsDock::DrawOptionsDock(QWidget *parent, Qt::WindowFlags flags)
: QDockWidget(parent, flags)
{
  setFeatures(QDockWidget::NoDockWidgetFeatures);

  QWidget *drawOptions = new QWidget(this);
  QFormLayout *drawOptionsLayout = new QFormLayout();

  _showGrid = new QCheckBox();
  _showGrid->setToolTip(tr("Draw grid behind host map."));
  connect(_showGrid, SIGNAL(stateChanged(int)), this, SLOT(settingChanged()));
  drawOptionsLayout->addRow(tr("Show Grid"), _showGrid);

  drawOptions->setLayout(drawOptionsLayout);
  
  QScrollArea *scrollArea = new QScrollArea(this);
  scrollArea->setMinimumWidth(MINIMUM_DOCK_WIDTH);
  scrollArea->setWidget(drawOptions);
  
  setWidget(scrollArea);

  _loadSettings();
}

void DrawOptionsDock::settingChanged()
{
  QSettings settings(ORG_NAME, "Network");

  bool showGrid = _showGrid->isChecked();
  settings.setValue("drawOptions/showgrid", showGrid);
  
  
  settings.sync();
  emit(settingsDidChange());
}

void DrawOptionsDock::_loadSettings()
{
  QSettings settings(ORG_NAME, "Network");
  bool showGrid = settings.value("drawOptions/showgrid", true).toBool();
  _showGrid->setChecked(showGrid);
}
