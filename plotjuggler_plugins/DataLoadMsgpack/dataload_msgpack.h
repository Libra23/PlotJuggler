#pragma once

#include <QObject>
#include <QtPlugin>
#include "PlotJuggler/dataloader_base.h"
#include "ui_dataload_msgpack.h"

using namespace PJ;

enum TimeIndex {
  USE_ROW_AS_X_AXIS,
  USE_SELECTED_AS_AXIS
};

class DataLoadMsgpack : public DataLoader {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "facontidavide.PlotJuggler3.DataLoader")
  Q_INTERFACES(PJ::DataLoader)

public:
  DataLoadMsgpack();
  virtual const std::vector<const char*>& compatibleFileExtensions() const override;

  bool readDataFromFile(PJ::FileLoadInfo* fileload_info,
                        PlotDataMapRef& destination) override;

  virtual ~DataLoadMsgpack();

  virtual const char* name() const override { return "DataLoad Msgpack";}

private:
  TimeIndex launchDialog(const std::unordered_set<std::string>& time_series_name_list, std::string& selected_timestamp_name);

  std::vector<const char*> _extensions;
  QDialog* _dialog;
  Ui::DialogMsgpack* _ui;
};

