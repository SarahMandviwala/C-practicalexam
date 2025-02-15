#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_BTN_get_rate_clicked();
private slots:
    void on_data_received(QNetworkReply *reply, const QString &destinationCurrency);

private:
    void prepare_country_list();
    void populate_country_list();
    void fetch_exchange_rate(const QString &sourceCountry, const QString & DestinationCountry);

    Ui::MainWindow *ui;
    QNetworkAccessManager *manager;
    QStringList countryList;
    QString apiKey = "https://v6.exchangerate-api.com/v6/3c3ac8efffd47dcfb970eb4f/latest/USD";
};

#endif // MAINWINDOW_H
