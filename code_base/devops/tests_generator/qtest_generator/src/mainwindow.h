#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileSystemModel>
#include <QMainWindow>
#include <QTreeView>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_createTest_clicked();
    void on_pushButton_create_new_dir_clicked();
    void on_pushButton__chooseRootDir_clicked();

private:
    Ui::MainWindow *ui;
    QTreeView *treeView;
    QFileSystemModel *model;
    QString currentPath;
    void showContextMenu(const QPoint &pos);
    void createNewFolder();

signals:
    void folderSelected(QString path);
};
#endif  // MAINWINDOW_H
