        qDebug()<<"apt exists.";
            string list = "apt-mark showmanual > APTLIST.tmp";
            system(list.c_str());
            std::string line;
            std::ifstream listFile("APTLIST.tmp");
            if (!listFile.is_open()) {
                qDebug() << "Was not possible open APTLIST.tmp";
                return 1;
            }

            while(std::getline(listFile, line)){

                output << "sudo apt install " << line << '\n';

            }
            listFile.close();
            output.close();
