#ifndef DLANGTEXTEDITOR_H
#define DLANGTEXTEDITOR_H

#include <texteditor/textdocument.h>
#include <texteditor/texteditor.h>
#include <texteditor/texteditor.h>
#include <coreplugin/editormanager/ieditorfactory.h>
#include <utils/uncommentselection.h>

namespace DCodeModel {
class IModel;
typedef QSharedPointer<IModel> IModelSharedPtr;
}

namespace DlangEditor {

class DlangTextEditorWidget;
class DlangUseSelectionUpdater;
class DdocAutoCompleter;
class DlangOutlineModel;

class DlangTextEditor : public TextEditor::BaseTextEditor
{
    Q_OBJECT
public:
    explicit DlangTextEditor();

    // custom
    virtual QString contextHelpId() const Q_DECL_OVERRIDE;

    // others
    DlangOutlineModel *outline() const;

signals:

public slots:

private:
    DlangTextEditor(TextEditor::TextEditorWidget*);

    DlangOutlineModel *m_outlineModel;

};

class DlangTextEditorWidget : public TextEditor::TextEditorWidget
{
    Q_OBJECT
public:
    explicit DlangTextEditorWidget(QWidget *parent = 0);
    virtual ~DlangTextEditorWidget() Q_DECL_OVERRIDE;

    // virtual TextEditor::TextEditorWidget
    virtual void finalizeInitialization() Q_DECL_OVERRIDE;

    // others

protected:
    virtual void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;

signals:
private:
    virtual void contextMenuEvent(QContextMenuEvent *e) Q_DECL_OVERRIDE;
    virtual Link findLinkAt(const QTextCursor &c, bool resolveTarget, bool inNextSplit) Q_DECL_OVERRIDE;

    DlangUseSelectionUpdater *m_useSelectionsUpdater;
    DCodeModel::IModelSharedPtr m_codeModel;
    DdocAutoCompleter *m_ddocCompleter;
};

class DlangEditorFactory : public TextEditor::TextEditorFactory
{
    Q_OBJECT

public:
    DlangEditorFactory();

    // others

private:
    const QStringList m_mimeTypes;
};

class DlangDocument : public TextEditor::TextDocument
{
    Q_OBJECT

public:
    DlangDocument();
    QString defaultPath() const Q_DECL_OVERRIDE;
    QString suggestedFileName() const Q_DECL_OVERRIDE;
};

bool getFullIdentifier(const TextEditor::TextDocument* doc, int pos, int &begin, int &size);

} // namespace DlangEditor

#endif // DLANGTEXTEDITOR_H
