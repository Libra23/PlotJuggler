#include "dataload_msgpack.h"
#include "msgpack_parser.h"
#include <QTextStream>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QSettings>
#include <QProgressDialog>
#include <QDateTime>
#include <QInputDialog>

DataLoadMsgpack::DataLoadMsgpack()
{
  _extensions.push_back("mpac");
}

const QRegExp csv_separator("(\\,)");

const std::vector<const char*>& DataLoadMsgpack::compatibleFileExtensions() const
{
  return _extensions;
}

bool DataLoadMsgpack::readDataFromFile(FileLoadInfo* info, PlotDataMapRef& plot_data)
{
  const auto& filename = info->filename;
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly))
  {
    throw std::runtime_error("Msgpack: Failed to open file");
  }
  QByteArray file_array = file.readAll();
  MsgpackParser msg_parser(file_array.data(), file_array.size());
  file.close();

  MsgpackParser::Timeseries timeseries = msg_parser.RefTimeseries();

  for (const auto& data : timeseries.data) {
    auto series = plot_data.addNumeric(data.first);

    for (size_t i = 0; i < data.second.size(); i++) {
      double msg_time = static_cast<double>(timeseries.timestamps[i]);
      PlotData::Point point(msg_time, data.second[i]);
      series->second.pushBack(point);
    }
  }

  return true;
}