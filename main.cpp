#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <filesystem>
#include <cstdlib>
#include <fstream>
#include <QString>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCommandLineParser parser;
    parser.addHelpOption();

    parser.addOption({"split", "splits in two automations (programs|non programs)"});
    parser.addOption({{"directory", "dir"}, "directory wich the automations will be touched", "directory"});
    parser.addOption({"mng", "Package manager to use.", "manager"});
    parser.process(app);

    bool splitted = parser.isSet("split");
    QString manager = parser.value("mng");
    string managerstr = manager.toStdString();
    QString path = parser.value("dir");
    string pathstr = path.toStdString();

    if (manager.isEmpty() && splitted == true) {
        if (std::filesystem::exists("/usr/bin/apt")) {
            #include "aptsplit.inc"
        } else {
            qDebug() << "apt not found!";
        }

        if (std::filesystem::exists("/usr/bin/snap")) {
            #include "snapsplit.inc"
        } else {
            qDebug() << "snap not found";
        }
        //ADD MORE PACKAGE MANAGERS HERE!!
    }

    if (manager.isEmpty() && splitted == false) {
        std::filesystem::path allScript = std::filesystem::path(pathstr) / "installEverything.sh";
        std::ofstream output(allScript);
        if (!output.is_open()) {
            qDebug() << "was not possible make installEverything.sh";
            return 1;
        }
        output << "#!/bin/bash\n";

        if (std::filesystem::exists("/usr/bin/apt")) {
            #include "aptnonsplit.inc"
        }

        if (std::filesystem::exists("/usr/bin/snap")) {
            #include "snapnonsplit.inc"
        } else {
            qDebug() << "snap does not exists";
        }

        output.close();
        std::filesystem::permissions(allScript,
            std::filesystem::perms::owner_exec | std::filesystem::perms::group_exec | std::filesystem::perms::others_exec,
            std::filesystem::perm_options::add);
        qDebug() << "finished writing installEverything.sh";
    }

    //ADD PACKAGE MANAGERS HERE!!!
    if (manager == "apt") {
        if (std::filesystem::exists("/usr/bin/apt")) {
            #include "aptsplit.inc"
        }
    } else if (manager == "snap") {
        if (std::filesystem::exists("/usr/bin/snap")) {
            #include "snapsplit.inc"
        }
    } else if (!manager.isEmpty()) {
        qDebug() << "unknown package manager:" << manager;
    }

    return 0;
}