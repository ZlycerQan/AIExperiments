// mainwindow.cpp
#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindowTitle("MC-Problem Solver");
}

MainWindow::~MainWindow() {
    delete ui;
}

QString MainWindow::format(const state& x, const int& n) {
    state y = {n - x.x1, n - x.x2, x.x3 ^ 1};
    const static QString blue = "<span style=\"color: blue\">%1</span>";
    const static QString red = "<span style=\"color: red\">%1</span>";
    const static QString plain = "<span>%1</span>";
    QString result, tmp;
    result += plain.arg("___");
    result += blue.arg(tmp.fill('+', y.x1));
    result += red.arg(tmp.fill('#', y.x2));
    result += plain.arg(tmp.fill('_', x.x1 + x.x2));
    result += plain.arg('_');
    for (int i = 0; i < std::max(n, 8); ++ i) {
        result += plain.arg("-_");
    }
    result += plain.arg("-");
    result += plain.arg('_');
    result += plain.arg(tmp.fill('_', y.x1 + y.x2));
    result += blue.arg(tmp.fill('+', x.x1));
    result += red.arg(tmp.fill('#', x.x2));
    result += plain.arg("___");
    return result;
}

QString MainWindow::formatBoat(const state& last, const state& now, const int& n) {
    int bol = std::max(8, n) * 2 + 1;
    int length = n * 4 + bol + 8;
    int x = abs(last.x1 - now.x1);
    int y = abs(last.x2 - now.x2);
    QString boat, tmp, result;
    const static QString blue = "<span style=\"color: blue\">%1</span>";
    const static QString red = "<span style=\"color: red\">%1</span>";
    const static QString plain = "<span>%1</span>";
    boat += plain.arg("\\_");
    boat += blue.arg(tmp.fill('+', x));
    boat += red.arg(tmp.fill('#', y));
    boat += plain.arg("_/");
    int siz = x + y + 4;
    tmp = "";
    for (int i = 0; i < (length - siz) / 2; ++ i) {
        tmp += "&nbsp;";
    }
    result += plain.arg(tmp);
    if (last.x3 == 0) {
        result += boat + plain.arg("-&gt;");
    } else {
        result += plain.arg("&lt;-") + boat;
    }
    tmp = "";
    for (int i = 0; i < (length - siz) / 2; ++ i) {
        tmp += "&nbsp;";
    }
    result += plain.arg(tmp);
    return result;
}

void MainWindow::on_calculateButton_clicked() {
    ui->displayBrowser->clear();
    int n = ui->nLineEdit->text().toInt();
    int c = ui->cLineEdit2->text().toInt();
    auto result = solve(n, c);
    if (result.size() == 0) {
        QMessageBox::warning(this, "tips", "No Solution.");
        return ;
    }
    ui->displayBrowser->append(QString("Total %1 steps.\n").arg(result.size() - 1));
    ui->displayBrowser->append("Use '+' to replace the priest, '#' to replace the savage.\n");
    for (int i = 0; i < (int)result.size(); ++ i) {
        ui->displayBrowser->append(QString::number(i));
        ui->displayBrowser->append(format(result[i], n));
        ui->displayBrowser->append("");
        if (i != (int)result.size() - 1) {
            ui->displayBrowser->append(formatBoat(result[i], result[i + 1], n));
        }
    }
    ui->displayBrowser->moveCursor(QTextCursor::Start);
}
