#include "texteditor.h"

TextEditor::TextEditor(QWidget* pwgt){
    setStyle(QStyleFactory::create("Fusion"));
    setWindowTitle("TextEditor");
    setWindowIcon(QIcon(":new/prefix1/text-editor.png"));
    QToolBar* ptb = new QToolBar("Панель инструментов");
    ptb->setStyleSheet("background-color:darkgray;");
    QMenu* pmnuFile = new QMenu("Файл");
    QAction * pactNew = new QAction ("Новый файл", 0) ;
    pactNew -> setText ("&Новый");
    pactNew -> setShortcut ( QKeySequence ("CTRL+N"));
    pactNew -> setToolTip ("Создать новый документ");
    pactNew -> setStatusTip ("Создание нового файла");
    pactNew -> setWhatsThis ("Создание нового файла");
    pactNew -> setIcon (QPixmap(":new/prefix1/New.png"));
    connect (pactNew, SIGNAL(triggered()), SLOT(slotNewDoc()));
    ptb->addAction(pactNew);
    pmnuFile->addAction(pactNew);

    QAction * pactOpen = new QAction ("Открыть файл", 0) ;
    pactOpen -> setText ("&Открыть");
    pactOpen -> setShortcut ( QKeySequence ("CTRL+O"));
    pactOpen -> setToolTip ("Открыть файл");
    pactOpen -> setStatusTip ("Открытие файла");
    pactOpen -> setWhatsThis ("Открытие файла");
    pactOpen -> setIcon (QPixmap(":new/prefix1/Open.png"));
    connect (pactOpen, SIGNAL(triggered()), SLOT(slotOpen()));
    ptb->addAction(pactOpen);
    pmnuFile->addAction(pactOpen);
    //ptb->addSeparator();

    QAction * pactSave = new QAction ("Сохранить файл", 0) ;
    pactSave -> setText ("&Сохранить");
    pactSave -> setShortcut ( QKeySequence ("CTRL+S"));
    pactSave -> setToolTip ("Сохранить файл");
    pactSave -> setStatusTip ("Сохранение файла");
    pactSave -> setWhatsThis ("Сохранение файла");
    pactSave -> setIcon (QPixmap(":new/prefix1/Save.png"));
    connect (pactSave, SIGNAL(triggered()), SLOT(slotSave()));
    ptb->addAction(pactSave);
    pmnuFile->addAction(pactSave);

    pmnuFile->addAction("Сохранить как...", this, SLOT(slotSaveAs()));

    QAction* pactSaveAll = new QAction ("Сохранить все файлы", 0) ;
    pactSaveAll -> setText ("&Сохранить все");
    pactSaveAll -> setToolTip ("Сохранить все файлы");
    pactSaveAll -> setStatusTip ("Сохранение всех файлов");
    pactSaveAll -> setWhatsThis ("Сохранение всех файлов");
    pactSaveAll -> setIcon (QPixmap(":new/prefix1/SaveAll.png"));
    connect (pactSaveAll, SIGNAL(triggered()), SLOT(slotSaveAll()));
    ptb->addAction(pactSaveAll);
    pmnuFile->addAction(pactSaveAll);

    pmnuFile->addAction("Закрыть", this, SLOT(slotClose()));
    pmnuFile->addAction("Закрыть все", this, SLOT(slotCloseAll()));
    pmnuFile->addAction("Выход", this, SLOT(slotExit()));
    QMenu* pmnuEdit = new QMenu("Правка");
    pmnuEdit->addAction("Вырезать", this, SLOT(slotCut()));
    pmnuEdit->addAction("Копировать");
    pmnuEdit->addAction("Вставить");
    pmnuEdit->addAction("Удалить");
    pmnuEdit->addAction("Выделить все");
    QMenu* pmnuView = new QMenu("Вид");
    QAction* pactViewExplorer = new QAction ("Показать проводник", 0);
    pactViewExplorer->setCheckable(true);
    pactViewExplorer->setChecked(true);
    connect (pactViewExplorer, SIGNAL(toggled(bool)), SLOT(slotViewExplorer(bool)));

    pmnuView->addAction(pactViewExplorer);
    QAction* pactViewExplorerOpenDocuments = new QAction ("Показать обозреватель открытых документов", 0);
    pactViewExplorerOpenDocuments->setCheckable(true);
    pactViewExplorerOpenDocuments->setChecked(true);
    connect (pactViewExplorerOpenDocuments, SIGNAL(toggled(bool)), SLOT(slotViewExplorerOpenDocuments(bool)));
    pmnuView->addAction(pactViewExplorerOpenDocuments);

    this->menuBar()->addMenu(pmnuFile);
    this->menuBar()->addMenu(pmnuEdit);
    this->menuBar()->addMenu(pmnuView);

    QMenu* pmnuSyntax = new QMenu("Синтаксисы");
    pmnuSyntax->addAction("C++", this, SLOT(slotSelectCPP()));
    pmnuSyntax->addAction("Добавить свой синтаксис");
    this->menuBar()->addMenu(pmnuSyntax);

    m_tbw = new QTabWidget;
    setCentralWidget(m_tbw);
    statusBar()->showMessage("Готово", 3000);

    addToolBar(Qt::TopToolBarArea, ptb);

    m_fileModel = new QFileSystemModel;
    QTreeView* treeView = new QTreeView();
    treeView->setModel(m_fileModel);
    m_fileModel->setRootPath(QDir::currentPath());
    treeView->setWindowTitle("Проводник");
    Explorer = new QDockWidget;
    Explorer->setWidget(treeView);
    Explorer->resize(500, 500);
    Explorer->setWindowTitle("Проводник");
    addDockWidget(Qt::LeftDockWidgetArea, Explorer);

    m_StringListModel = new QStringListModel;
    m_StringListModel->setStringList(m_StringList);
    QListView* listView = new QListView();
    listView->setWindowTitle("Обозреватель открытых документов");
    listView->setModel(m_StringListModel);
    ExplorerOpenDocs = new QDockWidget;
    //QDockWidget* DW = new QDockWidget("ExplorerOpenDocuments", this);
    ExplorerOpenDocs->setWidget(listView);
    ExplorerOpenDocs->resize(500, 500);
    ExplorerOpenDocs->setWindowTitle("Обозреватель открытых документов");
    addDockWidget(Qt::RightDockWidgetArea, ExplorerOpenDocs);

    m_tbw->setTabsClosable(true);

    connect(m_tbw, SIGNAL(tabCloseRequested(int)), this, SLOT(slotCloseTab(int)));
    connect(listView, SIGNAL(clicked(QModelIndex)), this, SLOT(slotSetCurrentDoc(QModelIndex)));
    connect(ExplorerOpenDocs, SIGNAL(visibilityChanged(bool)), pactViewExplorerOpenDocuments, SLOT(setChecked(bool)));
    connect(Explorer, SIGNAL(visibilityChanged(bool)), pactViewExplorer, SLOT(setChecked(bool)));
}


void TextEditor::slotNewDoc()
{
    QTextEdit* TE = new QTextEdit;
    TE->setStyleSheet("background-color:snow;");
    m_tbw->addTab(TE, "NewDoc");
    ++m_CountFiles;
    m_StringList.push_back("NewDoc");
    m_StringListModel->setStringList(m_StringList);
}

void TextEditor::slotOpen()
{
     QTextEdit* TE = new QTextEdit;
     QFile file(QFileDialog::getOpenFileName(this,
                                             "Открыть файл",
                                             QDir::current().dirName(),
                                             "Все файлы(*.*);;Текстовые файлы(*.txt)"));
     QString str;
     QString Text;
     QTextStream TS(&file);
     if (file.open(QIODevice::ReadOnly))
         while(!TS.atEnd())
         {
            TS.readLineInto(&str);
            str.append("\n");
            Text.append(str);
         }
     TE->setText(Text);
     QFileInfo fInfo(file);
     m_tbw->addTab(TE, fInfo.baseName() + "." + fInfo.completeSuffix());
     m_StringList.push_back(fInfo.baseName());
     m_StringListModel->setStringList(m_StringList);
     if((fInfo.completeSuffix() == "h") || ((fInfo.completeSuffix() == "cpp")))
     {
         qDebug() << "OK";
         new Highlighter("cppHighlight.xml", TE->document());
     }
     file.close();
}

void TextEditor::slotSave()
{
    QTextEdit* TE = (QTextEdit*)m_tbw->currentWidget();
    QFile file(m_tbw->tabText(0) + ".txt");
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream(&file) << TE->toPlainText();
        file.close();
    }
}

void TextEditor::slotSaveAs()
{
    QTextEdit* TE = (QTextEdit*)m_tbw->currentWidget();
    QString str = QFileDialog::getSaveFileName(0, m_tbw->tabText(0), QString(), tr("TXT file(*.txt);;CPP file(*.cpp);;H file (*.h)"), 0);
    QFile file(str);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream(&file) << TE->toPlainText();
        file.close();
    }
//    QWidget* wdgtSaveAs = new QWidget;
//    wdgtSaveAs->setWindowTitle("Сохранить как");
//    wdgtSaveAs->resize(500, 100);
//    QVBoxLayout* VLayout = new QVBoxLayout;
//    m_ComboBoxPath = new QComboBox;
//    m_ComboBoxPath->addItem("txt");
//    m_ComboBoxPath->addItem("doc");
//    m_ComboBoxPath->addItem("cpp");
//    m_ComboBoxPath->addItem("h");
//    QPushButton* pbtn = new QPushButton("OK");
//    VLayout->addWidget(m_ComboBoxPath);
//    VLayout->addWidget(pbtn);
//    wdgtSaveAs->setLayout(VLayout);
//    wdgtSaveAs->show();
//    connect(pbtn, SIGNAL(clicked()), this, SLOT(slotSavePath()));
//    connect(pbtn, SIGNAL(clicked()), wdgtSaveAs, SLOT(close()));
}

void TextEditor::slotSavePath()
{
    QTextEdit* TE = (QTextEdit*)m_tbw->currentWidget();
    QFile file(m_tbw->tabText(0) + "." + m_ComboBoxPath->currentText());
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream(&file) << TE->toPlainText();
        file.close();
    }
}

void TextEditor::slotSaveAll()
{
    for(int i = 0; i < m_tbw->count(); ++i)
    {
        QTextEdit* TE = (QTextEdit*)m_tbw->widget(i);
        QFile file(m_tbw->tabText(i));
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream(&file) << TE->toPlainText();
            file.close();
        }
    }
}

void TextEditor::slotClose()
{

    QMessageBox* pmbx =
     new QMessageBox(QMessageBox::Information,
     "MessageBox",
     "Сохранить изменения файла?",
     QMessageBox::Yes | QMessageBox::No |
     QMessageBox::Cancel);
    int n = pmbx->exec();
    m_StringList.removeAt(m_tbw->currentIndex());
    m_StringListModel->setStringList(m_StringList);
    if (n == QMessageBox::Yes)
    {
        slotSave();
        m_tbw->removeTab(m_tbw->currentIndex());
    }
    if (n == QMessageBox::No)
    {
        m_tbw->removeTab(m_tbw->currentIndex());
    }
}

void TextEditor::slotCloseAll()
{
    //...//
    QMessageBox* pmbx =
     new QMessageBox(QMessageBox::Information,
     "MessageBox",
     "Сохранить изменения файла?",
     QMessageBox::Yes | QMessageBox::No |
     QMessageBox::Cancel);
    int n = pmbx->exec();
    if (n == QMessageBox::Yes)
    {
        slotSave();
        m_tbw->removeTab(m_tbw->currentIndex());
    }
    if (n == QMessageBox::No)
    {
        m_tbw->removeTab(m_tbw->currentIndex());
    }
    m_StringList.clear();
    m_StringListModel->setStringList(m_StringList);
}

void TextEditor::slotExit()
{

    close();
}

void TextEditor::slotCut()
{
    QTextEdit* TE = (QTextEdit*)m_tbw->currentWidget();
    emit TE->cut();
}

void TextEditor::slotCopy()
{
    QTextEdit* TE = (QTextEdit*)m_tbw->currentWidget();
    emit TE->copy();
}
void TextEditor::slotPaste()
{
    QTextEdit* TE = (QTextEdit*)m_tbw->currentWidget();
    emit TE->paste();
}
void TextEditor::slotDelete()
{
    QTextEdit* TE = (QTextEdit*)m_tbw->currentWidget();
    //emit TE->delete();
}
void TextEditor::slotSelectAll()
{
    QTextEdit* TE = (QTextEdit*)m_tbw->currentWidget();
    emit TE->selectAll();
}

void TextEditor::slotViewExplorer(bool flag)
{
    Explorer->setVisible(flag);
}
void TextEditor::slotViewExplorerOpenDocuments(bool flag)
{
    ExplorerOpenDocs->setVisible(flag);
}


void TextEditor::slotSetCurrentDoc(QModelIndex p_Index)
{
    m_tbw->setCurrentIndex(p_Index.row());
}

void TextEditor::slotCloseTab(int p_Ind)
{
    m_StringList.removeAt(p_Ind);
    m_StringListModel->setStringList(m_StringList);
    if (p_Ind == -1)
        return;
    QWidget* tabItem = m_tbw->widget(p_Ind);
    m_tbw->removeTab(p_Ind);
    delete(tabItem);
    tabItem = nullptr;
}

void TextEditor::slotSelectCPP()
{
    QTextEdit* TE = (QTextEdit*)(m_tbw->currentWidget());
    new Highlighter("cppHighlight.xml", TE->document());
}