#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "graphsearch.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushItemsAdd_clicked();

    void on_pushItemsRemove_clicked();

    void on_pushTableFill_clicked();

    void on_pushVerticesDFS_clicked();

    void on_pushVerticesBFS_clicked();

    void on_pushTableSave_clicked();

	void on_pushTableLoad_clicked();

	void on_pushMSTPrims_clicked();

	void on_pushMSTKruskals_clicked();

private:
    Ui::MainWindow *ui;
    GraphSearch gs;

    void callSearchFunction(const GraphSearch::SearchMethod& sm);
};

#endif // MAINWINDOW_H
