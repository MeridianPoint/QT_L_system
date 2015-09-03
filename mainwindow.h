#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "stable.h"
#include <QMainWindow>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void ColorChange(QColor newColor);

private slots:

    void on_foceUpdateButton_pressed();

    void on_actionDocument_triggered();

    void on_actionExit_triggered();

    void on_ColorSelector_clicked();

    void on_BrowseRuleFile_clicked();

private:
    QColor defaultcolor=QColor(Qt::yellow);
    bool initalized=false;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
