#ifndef DLANGHOVERHANDLER_H
#define DLANGHOVERHANDLER_H

#include <texteditor/basehoverhandler.h>

namespace DlangEditor {

class DlangHoverHandler : public TextEditor::BaseHoverHandler
{
    Q_OBJECT
public:
    explicit DlangHoverHandler(QObject *parent = 0);

    // pure TextEditor::BaseHoverHandler
    virtual bool acceptEditor(Core::IEditor *editor);
    virtual void identifyMatch(TextEditor::ITextEditor *editor, int pos);

    // overriden
    virtual void decorateToolTip();

signals:

public slots:

private:
    int lastPos;
    QString lastSymbol;
    QString lastTooltip;

};

} // namespace DlangEditor

#endif // DLANGHOVERHANDLER_H
