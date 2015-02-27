#include "dlangautocompleter.h"

#include <QTextCursor>
#include <QTextDocument>
#include <QTextBlock>

using namespace DlangEditor;

DlangAutoCompleter::DlangAutoCompleter()
{
}

bool DlangAutoCompleter::contextAllowsAutoParentheses(const QTextCursor &cursor, const QString &textToInsert) const
{
    Q_UNUSED(cursor);
    Q_UNUSED(textToInsert);
    return true;
}

QString DlangAutoCompleter::insertMatchingBrace(const QTextCursor &cursor, const QString &text, QChar la, int *skippedChars) const
{
    Q_UNUSED(skippedChars)

    if (text.isEmpty() || !shouldInsertMatchingText(la))
        return QString();

    const QChar lastChar = text[text.length() - 1];
    QString result;

    if      (lastChar == QLatin1Char('('))  result = QLatin1Char(')');
    else if (lastChar == QLatin1Char('['))  result = QLatin1Char(']');
    else if (lastChar == QLatin1Char('"'))  result = QLatin1Char('"');
    else if (lastChar == QLatin1Char('\'')) result = QLatin1Char('\'');
    else if (lastChar == QLatin1Char('{')) {
        const QString blockText = cursor.block().text().mid(cursor.positionInBlock());
        const QString trimmedBlockText = blockText.trimmed();
        if (!trimmedBlockText.isEmpty() && trimmedBlockText.at(0) == QLatin1Char(')')) {
            result = QLatin1Char('}');
        }
    }

    if (la == lastChar) {
        if (skippedChars) {
            ++(*skippedChars);
        }
        return QString();
    }

    return result;
}

QString DlangAutoCompleter::insertParagraphSeparator(const QTextCursor &cursor) const
{
    QChar c = ' ';
    Q_ASSERT(c.isSpace());
    for (int i = 1; c.isSpace(); ++i) {
        c = cursor.document()->characterAt(cursor.position() - i);
    }
    if (c != QLatin1Char('{')) {
        return QString();
    }
    return "}";
}

bool DlangAutoCompleter::shouldInsertMatchingText(QChar c) const
{
    switch (c.unicode()) {
    case '{': case '}':
    case ']': case ')':
    case ';': case ',':
    case '\"': case '\'':
        return true;
    default:
        if (c.isSpace())
            return true;
    }
    return false;
}

DdocAutoCompleter::DdocState DdocAutoCompleter::isDdocComment(const QTextCursor &cursor)
{
    const QString ddocStart = QLatin1String("/**");
    QTextBlock b = cursor.block();
    do {
        QString blockText = b.text().trimmed();
        if (blockText.startsWith(ddocStart)) {
            return b.blockNumber() == cursor.blockNumber()
                    && !(b.next().isValid() && b.next().text().trimmed().startsWith('*')) ? DDOC_START : DDOC_IN;
        } else if (!blockText.startsWith(QLatin1Char('*')) || blockText.startsWith(QLatin1String("*/"))) {
            return DDOC_OUT;
        }
        b = b.previous();
    } while (b.isValid() && b.length());
    return DDOC_OUT;
}

QString DdocAutoCompleter::insertParagraphSeparator(DdocState state) const
{
    switch (state) {
    case DDOC_START:
        return QLatin1String("\n* \n*/");
    case DDOC_IN:
        return QLatin1String("\n* ");
    case DDOC_OUT:
        break;
    }
    return QString();
}
