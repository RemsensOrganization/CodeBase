#ifndef DEVICES_GPS_EXAMPLES_GUI_GPS_WIDGET_H_
#define DEVICES_GPS_EXAMPLES_GUI_GPS_WIDGET_H_

#include <QPlainTextEdit>
#include <QWidget>

struct GpsData;

class GpsWidget : public QWidget {
public:
    GpsWidget(QWidget *parent = nullptr);

public slots:
    void showGpsData(GpsData data);
    void showGpsStatus(const QString &msg);

private:
    QPlainTextEdit *status;
};

#endif  // DEVICES_GPS_EXAMPLES_GUI_GPS_WIDGET_H_
