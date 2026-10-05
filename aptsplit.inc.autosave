
std::filesystem::path listPath = std::filesystem::path(pathstr) / "APTLIST.tmp";
std::filesystem::path scriptPath = std::filesystem::path(pathstr) / "apt.sh";

std::string cmd = "apt-mark showmanual > '" + listPath.string() + "'";
if (std::system(cmd.c_str()) != 0) {
    qDebug() << "apt-mark failed";
}
else {
    std::ifstream listFile(listPath);
    std::ofstream aptList(scriptPath);

    if (!listFile.is_open() || !aptList.is_open()) {
        qDebug() << "could not open apt list or apt.sh";
    }
    else {
        aptList << "#!/bin/bash\n";
        std::string line;
        while (std::getline(listFile, line)) {
            aptList << "sudo apt install -y " << line << '\n';
        }
        qDebug() << "finished writing apt.sh";
    }

    listFile.close();
    aptList.close();
    std::filesystem::remove(listPath);
}
