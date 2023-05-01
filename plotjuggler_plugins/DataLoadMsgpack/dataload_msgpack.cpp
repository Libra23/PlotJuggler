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
#include <QPushButton>

DataLoadMsgpack::DataLoadMsgpack() {
  _extensions.push_back("mpac");

  _dialog = new QDialog();
  _ui = new Ui::DialogMsgpack();
  _ui->setupUi(_dialog);

  // prepare ui
  _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
  connect(_ui->radioButtonSelect, &QRadioButton::toggled, this, [this](bool checked) {
    _ui->listWidgetSeries->setEnabled(checked);
    auto selected = _ui->listWidgetSeries->selectionModel()->selectedIndexes();
    bool box_enabled = !checked || selected.size() == 1;
    _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(box_enabled);
  });
  connect(_ui->listWidgetSeries, &QListWidget::itemSelectionChanged, this, [this]() {
    auto selected = _ui->listWidgetSeries->selectionModel()->selectedIndexes();
    bool box_enabled = _ui->radioButtonIndex->isChecked() || selected.size() == 1;
    _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(box_enabled);
  });
  connect(_ui->checkBoxDateFormat, &QCheckBox::toggled, this,
        [this](bool checked) { _ui->lineEditDateFormat->setEnabled(checked); });
}

DataLoadMsgpack::~DataLoadMsgpack() {
  delete _ui;
  delete _dialog;
}

const std::vector<const char*>& DataLoadMsgpack::compatibleFileExtensions() const {
  return _extensions;
}

bool DataLoadMsgpack::readDataFromFile(FileLoadInfo* info, PlotDataMapRef& plot_data) {
  const auto& filename = info->filename;
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly)) {
    throw std::runtime_error("Msgpack: Failed to open file");
  }
  QByteArray file_array = file.readAll();
  MsgpackParser msg_parser(file_array.data(), file_array.size());
  file.close();
  MsgpackParser::Timeseries timeseries = msg_parser.RefTimeseries();

  // select timestamp
  std::string time_series_stamp = "null";
  TimeIndex ret = launchDialog(msg_parser.RefTimeseriesKeys(), time_series_stamp);
  std::cout << " time_series_stamp : " << time_series_stamp << " ret : " << ret << std::endl;
  if (ret == TimeIndex::USE_SELECTED_AS_AXIS) {
    timeseries.timestamps = timeseries.data.at(time_series_stamp);
  }

  // plot
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

TimeIndex DataLoadMsgpack::launchDialog(const std::unordered_set<std::string>& time_series_name_list, std::string& selected_timestamp_name) {
  for(auto itr = time_series_name_list.cbegin(); itr != time_series_name_list.cend(); itr++) {
    _ui->listWidgetSeries->addItem(QString::fromStdString(*itr));
  }
  int res = _dialog->exec();

  // use row number as X axis if click cancel or select use row number
  if (res == QDialog::Rejected) {
    return TimeIndex::USE_ROW_AS_X_AXIS;
  }

  // select row
  QModelIndexList indexes = _ui->listWidgetSeries->selectionModel()->selectedRows();
  if (indexes.size() == 1) {
    selected_timestamp_name = _ui->listWidgetSeries->item(indexes.front().row())->text().toStdString();
    return TimeIndex::USE_SELECTED_AS_AXIS;
  }

  return TimeIndex::USE_ROW_AS_X_AXIS;
}