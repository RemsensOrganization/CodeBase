#include "mainwindow.h"

#include <QFileDialog>
#include <QFileSystemModel>
#include <QInputDialog>
#include <QMessageBox>

#include "test_template_code_text.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    currentPath = QApplication::applicationDirPath();
    ui->label_directory_for_new_test->setText(currentPath);
    // Создание модели
    model = new QFileSystemModel(this);
    model->setRootPath(
        QApplication::applicationDirPath());  // <-- текущая директория
    model->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);

    // Создание представления
    treeView = ui->treeView;
    treeView->setModel(model);
    treeView->setRootIndex(model->index(QApplication::applicationDirPath()));
    treeView->setColumnHidden(1, true);  // скрыть размер
    treeView->setColumnHidden(2, true);  // скрыть тип
    treeView->setColumnHidden(3, true);  // скрыть дату
    treeView->setHeaderHidden(true);
    treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(treeView, &QTreeView::customContextMenuRequested, this,
            &MainWindow::showContextMenu);

    // Регулярное выражение: разрешает всё, КРОМЕ недопустимых символов
    // \\ / : * ? " < > | и контрольные символы (\x00-\x1F)
    QRegularExpression rx("^[^\\\\/:*?\"<>|\\x00-\\x1F]+$");

    QValidator *validator = new QRegularExpressionValidator(rx, this);
    ui->lineEdit->setValidator(validator);

    // Опционально: максимальная длина (255 символов - лимит Windows)
    ui->lineEdit->setMaxLength(255);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_pushButton_createTest_clicked() {
    QString newName = ui->lineEdit->text().trimmed();

    // 1. Проверка на пустое имя
    if (newName.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Имя не может быть пустым");
        ui->lineEdit->setFocus();
        return;
    }

    // 2. Проверка: если currentPath не установлен
    if (currentPath.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выберите папку из дерева");
        return;
    }

    // 3. Проверка на уникальность (существует ли уже такая директория)
    QString fullPath = QDir(currentPath).filePath(newName);

    if (QFileInfo::exists(fullPath)) {
        QMessageBox::warning(this, "Ошибка",
                             "Директория с именем \"" + newName +
                                 "\" уже существует в выбранной папке");
        ui->lineEdit->setFocus();
        ui->lineEdit->selectAll();
        return;
    }

    // 4. Дополнительная проверка: валидность имени (на случай обхода
    // валидатора)
    static QRegularExpression invalidRx("[\\\\/:*?\"<>|\\x00-\\x1F]");
    if (newName.contains(invalidRx)) {
        QMessageBox::warning(
            this, "Ошибка",
            "Имя содержит недопустимые символы: \\ / : * ? \" < > |");
        return;
    }

    // Всё ок — создаём директорию
    if (QDir().mkdir(fullPath)) {
        QMessageBox::information(this, "Успех",
                                 "Директория создана:\n" + fullPath);

        ui->lineEdit->clear();
    } else {
        QMessageBox::critical(
            this, "Ошибка создания",
            "Не удалось создать директорию. Проверьте права доступа.");
        return;
    }

    // Формируем имена файлов
    QString proFileName = newName + ".pro";
    QString headerFileName = newName + ".h";
    QString cppFileName = newName + ".cpp";

    // Заполняем шаблоны
    QString proContent = QString::fromUtf8(kProTestText)
                             .arg(newName)
                             .arg(headerFileName)
                             .arg(cppFileName);

    // Формируем имена // "MyTest.h"
    QString headerGuard = newName.toUpper() + "_H";  // "MYTEST_H"
    QString className = newName;                     // "MyTest"
    // Заполняем шаблоны
    QString headerContent = QString::fromUtf8(kHeaderTestText)
                                .arg(headerGuard)  // %1 - include guard
                                .arg(className);   // %2 - имя класса

    QString cppContent = QString::fromUtf8(kCppTestText)
                             .arg(headerFileName)  // %1 - имя заголовка
                             .arg(className);      // %2 - имя класса

    // Создаём файлы
    bool ok = true;

    // 1. .pro файл
    QFile proFile(QDir(fullPath).filePath(proFileName));
    if (!proFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        ok = false;
    } else {
        proFile.write(proContent.toUtf8());
        proFile.close();
    }

    // 2. Заголовочный файл
    if (ok) {
        QFile headerFile(QDir(fullPath).filePath(headerFileName));
        if (!headerFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            ok = false;
        } else {
            headerFile.write(headerContent.toUtf8());
            headerFile.close();
        }
    }

    // 3. CPP файл
    if (ok) {
        QFile cppFile(QDir(fullPath).filePath(cppFileName));
        if (!cppFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            ok = false;
        } else {
            cppFile.write(cppContent.toUtf8());
            cppFile.close();
        }
    }

    if (!ok) {
        QMessageBox::critical(this, "Ошибка",
                              "Не удалось создать один из файлов теста");
        // Опционально: удалить частично созданную папку
        QDir(fullPath).removeRecursively();
        return;
    }

    // Успех
    QMessageBox::information(this, "Успех",
                             "Тест создан:\n" + fullPath + "\n\nФайлы:\n- " +
                                 proFileName + "\n- " + headerFileName +
                                 "\n- " + cppFileName);
}

void MainWindow::showContextMenu(const QPoint &pos) {
    QModelIndex index = treeView->indexAt(pos);
    if (!index.isValid()) return;

    // Получаем полный путь к папке из модели
    QString fullPath = model->filePath(index);

    QMenu menu(this);
    QAction *selectAction = menu.addAction("Выбрать эту папку: " + fullPath);
    connect(selectAction, &QAction::triggered, this, [this, fullPath]() {
        // Действие с выбранным путём
        currentPath = fullPath;
        ui->label_directory_for_new_test->setText(currentPath);
        emit folderSelected(fullPath);  // или вызов вашей функции
    });
    menu.exec(treeView->viewport()->mapToGlobal(pos));
}

void MainWindow::createNewFolder() {
    // Проверка: выбрана ли папка
    if (currentPath.isEmpty()) {
        QMessageBox::warning(this, "Ошибка",
                             "Сначала выберите папку через контекстное меню");
        return;
    }

    // Диалог ввода имени
    bool ok;
    QString folderName = QInputDialog::getText(
        this, "Новая папка", "Введите имя папки:", QLineEdit::Normal, "", &ok);

    if (!ok || folderName.trimmed().isEmpty()) return;

    folderName = folderName.trimmed();

    // Проверка на недопустимые символы
    static QRegularExpression invalidRx("[\\\\/:*?\"<>|\\x00-\\x1F]");
    if (folderName.contains(invalidRx)) {
        QMessageBox::warning(
            this, "Ошибка",
            "Имя содержит недопустимые символы: \\ / : * ? \" < > |");
        return;
    }

    // Проверка на существование
    QString newPath = QDir(currentPath).filePath(folderName);
    if (QFileInfo::exists(newPath)) {
        QMessageBox::warning(this, "Ошибка",
                             "Папка с таким именем уже существует");
        return;
    }

    // Создание
    if (QDir().mkdir(newPath)) {
        // Выделить и раскрыть новую папку
        QModelIndex parentIndex = model->index(currentPath);
        treeView->expand(parentIndex);
        QModelIndex newIndex = model->index(newPath);
        treeView->setCurrentIndex(newIndex);
        treeView->scrollTo(newIndex);
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось создать папку");
    }
}

void MainWindow::on_pushButton_create_new_dir_clicked() { createNewFolder(); }

void MainWindow::on_pushButton__chooseRootDir_clicked() {
    currentPath = QApplication::applicationDirPath();
    ui->label_directory_for_new_test->setText(
        QApplication::applicationDirPath());
}
