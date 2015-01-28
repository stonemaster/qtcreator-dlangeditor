#include "dlangeditorplugin.h"
#include "dlangeditorconstants.h"

#include "dcdsupport.h"
#include "dlangeditor.h"
#include "dlangoptionspage.h"
#include "dlangcompletionassistprovider.h"
#include "dlanghoverhandler.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>

#include <projectexplorer/projectexplorer.h>
#include <projectexplorer/project.h>
#include <cpptools/cppmodelmanager.h>

#include <coreplugin/mimedatabase.h>

#include <QtPlugin>

using namespace DlangEditor::Internal;
using namespace DlangEditor;

DlangEditorPlugin::DlangEditorPlugin()
{
    // Create your members
}

DlangEditorPlugin::~DlangEditorPlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
}

bool DlangEditorPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments)

    if (!Core::MimeDatabase::addMimeTypes(QLatin1String(":/dlangeditor/DlangEditor.mimetypes.xml"), errorString))
        return false;

    setDcdConfiguration();

    addAutoReleasedObject(new DlangOptionsPage);
    addAutoReleasedObject(new DlangEditorFactory);
    addAutoReleasedObject(new DlangCompletionAssistProvider);

    return true;
}

void DlangEditorPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
}

ExtensionSystem::IPlugin::ShutdownFlag DlangEditorPlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}

void DlangEditorPlugin::setDcdConfiguration()
{
    Dcd::Factory::instance().setPortRange(DlangEditor::DlangOptionsPage::portsRange());
    Dcd::Factory::instance().setProcessName(DlangEditor::DlangOptionsPage::dcdServerExecutable());
    Dcd::Factory::instance().setServerLog(DlangEditor::DlangOptionsPage::dcdServerLogPath());

    Dcd::Factory::instance().setNameGetter([]() {
        ProjectExplorer::Project *currentProject = ProjectExplorer::ProjectExplorerPlugin::currentProject();
        return currentProject ? currentProject->displayName() : QLatin1String("defaultProject");
    });

    Dcd::Factory::instance().setServerInitializer([](QSharedPointer<Dcd::Server> server) {
        // append include paths from project settings
        QStringList list;
        CppTools::CppModelManager *modelmanager =
                CppTools::CppModelManager::instance();
        if (modelmanager) {
            ProjectExplorer::Project *currentProject = ProjectExplorer::ProjectExplorerPlugin::currentProject();
            if (currentProject) {
                CppTools::ProjectInfo pinfo = modelmanager->projectInfo(currentProject);
                if (pinfo.isValid()) {
                    foreach (const CppTools::ProjectPart::HeaderPath &header, pinfo.headerPaths()) {
                        if (header.isValid()) {
                            list.push_back(header.path);
                        }
                    }
                }
            }
        }
        list.append(DlangEditor::DlangOptionsPage::includePaths());
        list.removeDuplicates();
        Dcd::Client client(server->port());
        client.appendIncludePaths(list);
    });
}

