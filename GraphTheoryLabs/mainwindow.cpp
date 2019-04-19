#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

#include "graphsearch.h"
#include <vector>

#include <fstream>

using namespace std;

vector< vector<int> > readAdjMatrix(QTableWidget* tableWidget);
vector< vector<int> > convertAdjMatrix(const vector< vector<int> >&);
int readInt(QString title, QString label, int maxVal);

void outputResults(const vector<int> list, QListWidget* listWidget);


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushItemsAdd_clicked()
{
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    int columns = ui->tableWidget->columnCount();
    ui->tableWidget->insertColumn(columns);
    ui->tableWidget->setColumnWidth(columns, 30);
}

void MainWindow::on_pushItemsRemove_clicked()
{
    auto tableWidget = ui->tableWidget;

    if (tableWidget->columnCount() > 0) {
        tableWidget->removeColumn(ui->tableWidget->columnCount() - 1);
    }
    if (tableWidget->rowCount() > 0) {
        tableWidget->removeRow(ui->tableWidget->rowCount() - 1);
    }
}

void MainWindow::on_pushTableFill_clicked()
{
    auto text = ui->lineEditTableItemValue->text();
    auto tableWidget = ui->tableWidget;
    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        for (int column = 0; column < tableWidget->columnCount(); ++column) {
            QTableWidgetItem *item = new QTableWidgetItem();
            if (column != row) {
                item->setText(text);
            } else {
                item->setText(QString::number(0));
            }
            tableWidget->setItem(row, column, item);
        }
    }
}

void MainWindow::on_pushVerticesDFS_clicked()
{
    callSearchFunction(GraphSearch::DFS);
}

void MainWindow::on_pushVerticesBFS_clicked()
{
    callSearchFunction(GraphSearch::BFS);
}

void MainWindow::callSearchFunction(const GraphSearch::SearchMethod &sm)
{
    vector< vector<int> > table = readAdjMatrix(ui->tableWidget);

    if (table.empty()) {
        QMessageBox* mb = new QMessageBox();
        mb->setWindowTitle("Table");
        mb->setText("There are empty cells in the table");
        mb->show();
        return;
    }

    int startPos = readInt("Starting number", "Input starting vertice number", table.size()) - 1;

    vector< vector<int> > graph = convertAdjMatrix(table);
    vector<int> results;
    switch (sm) {
    case GraphSearch::BFS:
         results = gs.bfs(graph, startPos);
        break;
    case GraphSearch::DFS:
         results = gs.dfs(graph, startPos);
        break;
    default:
        QMessageBox* mb = new QMessageBox();
        mb->setWindowTitle("Internal error");
        mb->setText("Failed to call one of the search methods, invalid identifier");
        mb->show();
        return;
    }
    outputResults(results, ui->listWidget);
}


vector< vector<int> > readAdjMatrix(QTableWidget* tableWidget) {
    int rows = tableWidget->rowCount(),
            columns = tableWidget->columnCount();
    vector< vector<int> > results(rows, vector<int>(columns, 0));
    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            QTableWidgetItem *current = tableWidget->item(row, column);
            if (current == nullptr) {
                results = vector< vector<int> >();
                return results;
            }
            results[row][column] = current->text().toInt();
        }
    }
    return results;
}
vector< vector<int> > convertAdjMatrix(const vector< vector<int> >& vec) {
    vector< vector<int> > results(vec.size(), vector<int>());
    for (int i = 0; i < vec.size(); ++i) {
        for (int j = 0; j < vec.size(); ++j) {
            if (vec[i][j] > 0) {
                results[i].push_back(j);
            }/* else if (vec[i][j] < 0) {
                results[j].push_back(i);
            }*/
        }
    }
    return results;
}

int readInt(QString title, QString label, int maxVal) {
    bool ok;
    int val = QInputDialog().getInt(nullptr, title, label, 0, 1, maxVal, 1, &ok);
    if (!ok) {
        return -1;
    }
    return val;
}

void outputResults(const vector<int> list, QListWidget* listWidget) {
    listWidget->clear();
    if (list.empty()) {
        QMessageBox* mb = new QMessageBox();
        mb->setWindowTitle("Starting position");
        mb->setText("Invalid index");
        mb->show();
        return;
    }
    for (const auto& item : list) {
        listWidget->addItem(QString::number(item + 1));
    }
}

void MainWindow::on_pushTableSave_clicked()
{
    vector< vector<int> > matrix = readAdjMatrix(ui->tableWidget);
    QString fileName = QFileDialog::getSaveFileName(this, "Select file to save the matrix");
    if (fileName.length() > 0) {
        ofstream fout(fileName.toStdString());
        fout << matrix.size() << endl;
        for (const auto &vec : matrix) {
            for (const auto &item : vec) {
                fout << item << " ";
            }
            fout << endl;
        }
        fout.close();
    } else {
        QMessageBox* mb = new QMessageBox();
        mb->setWindowTitle("Invalid file name");
        mb->setText("Failed to save to specified file");
        mb->show();
    }
}

void MainWindow::on_pushTableLoad_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select file to save the matrix");
    if (fileName.length() > 0) {
        ifstream fin(fileName.toStdString());
        int N;
        fin >> N;
        if (N < 0) {
            QMessageBox* mb = new QMessageBox();
            mb->setWindowTitle("Invalid file format");
            mb->setText("Invalid matrix size specified");
            mb->show();
            fin.close();
            return;
        }
        if (ui->tableWidget->rowCount() > N) {
            for (int i = ui->tableWidget->rowCount(); i > N; --i) {
                on_pushItemsRemove_clicked();
            }
        } else {
            for (int i = ui->tableWidget->rowCount(); i < N; ++i) {
                on_pushItemsAdd_clicked();
            }
        }
        for (int row = 0; row < N; ++row) {
            for (int column = 0; column < N; ++column) {
                QTableWidgetItem *item = new QTableWidgetItem();
                string text;
                fin >> text;
                item->setText(QString(text.c_str()));
                ui->tableWidget->setItem(row, column, item);
            }
        }
        fin.close();
    } else {
        QMessageBox* mb = new QMessageBox();
        mb->setWindowTitle("Invalid file name");
        mb->setText("Failed to load specified file");
        mb->show();
    }
}
