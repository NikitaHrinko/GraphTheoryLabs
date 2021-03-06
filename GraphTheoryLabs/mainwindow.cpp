#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

#include "graphsearch.h"
#include <vector>
#include <algorithm>

#include <fstream>

using namespace std;

vector< vector<int> > readAdjMatrix(QTableWidget* tableWidget);
vector< vector<int> > convertAdjMatrix(const vector< vector<int> >&);

int readInt(QString title, QString label, int minValue, int maxVal);

void outputResults(const vector<int> list, QListWidget* listWidget);
void outputResults(const vector< vector<int> >& matrix, QTableWidget* tableWidget);

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
	int value = readInt("Fill value", "Input values to fill the table with", -100, 100);
    auto tableWidget = ui->tableWidget;
    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        for (int column = 0; column < tableWidget->columnCount(); ++column) {
            QTableWidgetItem *item = new QTableWidgetItem();
//            if (column != row) {
				item->setText(QString::number(value));
//            } else {
//                item->setText(QString::number(0));
//            }
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

	int startPos = readInt("Starting number", "Input starting vertice number", 1, table.size()) - 1;

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
	ui->labelListTitle->setText("Search order");
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

void MainWindow::on_pushMSTPrims_clicked()
{
	vector< vector<int> > matrix = readAdjMatrix(ui->tableWidget);
	int INF = readInt("INF value", "Input edge weight to mark that there's no edge", -100, 100);
	outputResults(gs.mstPrims(matrix, INF), ui->tableWidget);
	ui->labelListTitle->setText("");
}
void MainWindow::on_pushMSTKruskals_clicked()
{
	vector< vector<int> > matrix = readAdjMatrix(ui->tableWidget);
	outputResults(gs.mstKruskals(matrix), ui->tableWidget);
	ui->labelListTitle->setText("");
}

void MainWindow::on_pushSCC_clicked()
{
	ui->listWidget->clear();

	vector< vector<int> > matrix = readAdjMatrix(ui->tableWidget);
	int n = matrix.size();
	vector< vector<int> > gFront(n), gBack(n);

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (matrix[i][j] > 0) {
				gFront[i].push_back(j);
				gBack[j].push_back(i);
			}
		}
	}

	vector< vector<int> > components = gs.stronglyConnectedComponents(gFront, gBack);
	vector< vector<int> > newMatrix(components.size(), vector<int>(components.size()));
	int compIndex = 0;
	for(const auto& component : components) {
		QString componentStr;
		for (int i = 0; i < component.size() - 1; ++i) {
			componentStr += QString::number(component[i] + 1) + ", ";
		}
		componentStr += QString::number(component.back() + 1);
		ui->listWidget->addItem(componentStr);

		for (int i = 0; i < component.size(); ++i) { //i = component item index
			int item = component[i];
			for (int j = 0; j < matrix[item].size(); ++j) { //j = current vertice index
				if (matrix[item][j] > 0 && //j is reachable from this component and j is not in it
						(std::find(component.begin(), component.end(), j) == component.end())) {
					vector<int> connectedTo;
					for (int k = 0; k < components.size(); ++k) { //k = component that contains vertice j
						if (k != compIndex && std::find(components[k].begin(), components[k].end(), j) != components[k].end()) {
							connectedTo.push_back(k); // component k is reachable from current one
						}
					}
					for (const auto& it : connectedTo) {
						newMatrix[compIndex][it] = 1;
					}
				}
			}
		}
		++compIndex;
	}

	int N = newMatrix.size();
	if (ui->tableWidget->rowCount() > N) {
		for (int i = ui->tableWidget->rowCount(); i > N; --i) {
			on_pushItemsRemove_clicked();
		}
	} else {
		for (int i = ui->tableWidget->rowCount(); i < N; ++i) {
			on_pushItemsAdd_clicked();
		}
	}
	outputResults(newMatrix, ui->tableWidget);
	ui->labelListTitle->setText("Components");
}

int readInt(QString title, QString label, int minVal, int maxVal) {
	bool ok;
	int val = QInputDialog().getInt(nullptr, title, label, 0, minVal, maxVal, 1, &ok);
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

void outputResults( const vector< vector<int> >& matrix, QTableWidget* tableWidget) {
	int rows = tableWidget->rowCount(),
			columns = tableWidget->columnCount();

	for (int row = 0; row < rows; ++row) {
		for (int column = 0; column < columns; ++column) {
			QTableWidgetItem *item = new QTableWidgetItem();
			item->setText(QString::number(matrix[row][column]));
			tableWidget->setItem(row, column, item);
		}
	}
}
