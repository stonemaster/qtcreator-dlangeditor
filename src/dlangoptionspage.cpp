#include "dlangoptionspage.h"

#include "dlangeditorconstants.h"

#include <utils/pathchooser.h>
#include <utils/pathlisteditor.h>
#include <coreplugin/icore.h>

#include <QFormLayout>
#include <QSpinBox>
#include <QCheckBox>

using namespace DlangEditor;

namespace {
const char S_INCLUDE_DIR[] = "includeDir";
const char S_DCD_CLIENT[] = "dcdClientExecutable";
const char S_DCD_SERVER[] = "dcdServerExecutable";
const char S_DCD_SERVER_LOG[] = "dcdServerLog";
const char S_DCD_PORTS_FIRST[] = "dcdServerPortsRangeFirst";
const char S_DCD_PORTS_LAST[] = "dcdServerPortsRangeLast";
const char S_HOVER_ENABLE[] = "hoverEnable";
}

DlangOptionsPage::DlangOptionsPage()
{
    setId(Constants::DLANG_CODE_MODEL_SETTINGS_ID);
    setDisplayName(Constants::DLANG_CODE_MODEL_SETTINGS_NAME);
    setCategory(Constants::DLANG_SETTINGS_CATEGORY_ID);
    setDisplayCategory(Constants::DLANG_SETTINGS_CATEGORY_NAME);
    setCategoryIcon(QLatin1String(Constants::DLANG_SETTINGS_CATEGORY_ICON));
}

DlangOptionsPage::~DlangOptionsPage()
{

}

QWidget *DlangOptionsPage::widget()
{
    if (!m_widget) {
        m_widget = new DlangOptionsPageWidget;
    }
    return m_widget;
}

void DlangOptionsPage::apply()
{
    if (m_widget) {
        QSettings *settings = Core::ICore::settings();
        settings->beginGroup(tr("DlangSettings"));
        settings->setValue(QLatin1String(S_DCD_CLIENT), m_widget->clientExecutable());
        settings->setValue(QLatin1String(S_DCD_SERVER), m_widget->serverExecutable());
        settings->setValue(QLatin1String(S_DCD_SERVER_LOG), m_widget->serverLogPath());
        settings->setValue(QLatin1String(S_INCLUDE_DIR), m_widget->includePaths());
        settings->setValue(QLatin1String(S_DCD_PORTS_FIRST), m_widget->portsRange().first);
        settings->setValue(QLatin1String(S_DCD_PORTS_LAST), m_widget->portsRange().second);
        settings->setValue(QLatin1String(S_HOVER_ENABLE), m_widget->hoverEnable());
        settings->endGroup();
    }
}

void DlangOptionsPage::finish()
{
    delete m_widget;
}

QString DlangOptionsPage::dcdClientExecutable()
{
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(tr("DlangSettings"));
    QString result = settings->value(QLatin1String(S_DCD_CLIENT), QLatin1String("dcd-client")).toString();
    settings->endGroup();
    return result;
}

QString DlangOptionsPage::dcdServerExecutable()
{
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(tr("DlangSettings"));
    QString result = settings->value(QLatin1String(S_DCD_SERVER), QLatin1String("dcd-server")).toString();
    settings->endGroup();
    return result;
}

QString DlangOptionsPage::dcdServerLogPath()
{
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(tr("DlangSettings"));
    QString result = settings->value(QLatin1String(S_DCD_SERVER_LOG), QLatin1String("")).toString();
    settings->endGroup();
    return result;
}

QStringList DlangOptionsPage::includePaths()
{
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(tr("DlangSettings"));
    QStringList result = settings->value(QLatin1String(S_INCLUDE_DIR), QStringList("/usr/include/dlang/dmd/")).toStringList();
    settings->endGroup();
    return result;
}

QPair<int, int> DlangOptionsPage::portsRange()
{
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(tr("DlangSettings"));
    int first = settings->value(QLatin1String(S_DCD_PORTS_FIRST), 9167).toInt();
    int last = settings->value(QLatin1String(S_DCD_PORTS_LAST), 9197).toInt();
    settings->endGroup();
    return qMakePair(first, last);
}

bool DlangOptionsPage::hoverEnable()
{
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(tr("DlangSettings"));
    bool result = settings->value(QLatin1String(S_HOVER_ENABLE), false).toBool();
    settings->endGroup();
    return result;
}


DlangOptionsPageWidget::DlangOptionsPageWidget(QWidget *parent)
    : QWidget(parent)
{
    QFormLayout *formLayout = new QFormLayout(this);
    formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    m_client = new Utils::PathChooser(this);
    m_client->setExpectedKind(Utils::PathChooser::ExistingCommand);
    m_client->setHistoryCompleter(QLatin1String("Dlang.Command.Client.History"));
    m_client->setPath(DlangOptionsPage::dcdClientExecutable());
    formLayout->addRow(tr("DCD client executable:"), m_client);

    m_server = new Utils::PathChooser(this);
    m_server->setExpectedKind(Utils::PathChooser::ExistingCommand);
    m_server->setHistoryCompleter(QLatin1String("Dlang.Command.Server.History"));
    m_server->setPath(DlangOptionsPage::dcdServerExecutable());
    formLayout->addRow(tr("DCD server executable:"), m_server);

    m_serverLog = new Utils::PathChooser(this);
    m_serverLog->setExpectedKind(Utils::PathChooser::SaveFile);
    m_serverLog->setHistoryCompleter(QLatin1String("Dlang.Command.ServerLog.History"));
    m_serverLog->setPath(DlangOptionsPage::dcdServerLogPath());
    formLayout->addRow(tr("DCD server log path:"), m_serverLog);

    m_includes = new Utils::PathListEditor(this);
    m_includes->setPathList(DlangOptionsPage::includePaths());
    formLayout->addRow(tr("Include paths:"), m_includes);

    m_firstPort = new QSpinBox(this);
    m_firstPort->setRange(0, 100000);
    m_firstPort->setValue(DlangOptionsPage::portsRange().first);
    formLayout->addRow(tr("First port"), m_firstPort);

    m_lastPort = new QSpinBox(this);
    m_lastPort->setRange(0, 100000);
    m_lastPort->setValue(DlangOptionsPage::portsRange().second);
    formLayout->addRow(tr("Last port"), m_lastPort);

    m_hoverEnable = new QCheckBox(this);
    m_hoverEnable->setChecked(DlangOptionsPage::hoverEnable());
    formLayout->addRow(tr("Help tooltips"), m_hoverEnable);

}

DlangOptionsPageWidget::~DlangOptionsPageWidget()
{

}

QString DlangOptionsPageWidget::clientExecutable() const
{
    return m_client->path();
}

QString DlangOptionsPageWidget::serverExecutable() const
{
    return m_server->path();
}

QString DlangOptionsPageWidget::serverLogPath() const
{
    return m_serverLog->path();
}

QStringList DlangOptionsPageWidget::includePaths() const
{
    return m_includes->pathList();
}

QPair<int, int> DlangOptionsPageWidget::portsRange() const
{
    return qMakePair(static_cast<int>(m_firstPort->value()), static_cast<int>(m_lastPort->value()));
}

bool DlangOptionsPageWidget::hoverEnable() const
{
    return m_hoverEnable->isChecked();
}
