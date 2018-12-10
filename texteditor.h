#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H
#include <QWidget>
#include <QObject>
#include <QMenu>
#include <QMenuBar>
#include <QMainWindow>
#include <QLayout>
#include <QTabWidget>
#include <QStatusBar>
#include <QTextEdit>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <string>
#include <QToolBar>
#include <QMessageBox>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QDockWidget>
#include <QFileSystemModel>
#include <QStringListModel>
#include <QStringList>
#include <QTreeView>
#include <QListView>
#include <QComboBox>
#include <QAction>
#include <QStyleFactory>
#include "syntaxhighlighter.h"
#include "highlighter.h"

class TextEditor : public QMainWindow{
    Q_OBJECT
private:
    QTabWidget* m_tbw;
    int m_CountFiles;
    QFileSystemModel *m_fileModel;
    struct Tab
        {
            QTextEdit* m_textEdit;
            QFile* m_file;
            bool m_saved = true;
        };
    QComboBox* m_ComboBoxPath;
    QStringListModel* m_StringListModel;
    QStringList m_StringList;

    QDockWidget* Explorer;
    QDockWidget* ExplorerOpenDocs;

public:
    TextEditor(QWidget* pwgt = 0);
private slots:
    void slotNewDoc();
    void slotOpen();
    void slotSave();
    void slotSaveAs( );
    void slotSavePath();
    void slotSaveAll();
    void slotClose();
    void slotCloseAll();
    void slotExit();
    void slotCut();
    void slotCopy();
    void slotPaste();
    void slotDelete();
    void slotSelectAll();
    void slotViewExplorer(bool);
    void slotViewExplorerOpenDocuments(bool);
    void slotSetCurrentDoc(QModelIndex);
    void slotCloseTab(int);
    void slotSelectCPP();
};
#endif // TEXTEDITOR_H