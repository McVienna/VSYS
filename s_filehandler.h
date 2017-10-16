#ifndef S_FILEHANDLER_H
#define S_FILEHANDLER_H

class filehandler{
private:
    std::string path;
    std::string user;

public:
    filehandler(std::string);
    ~filehandler();
    void create_sub_usr(std::string, std::string);

};


#endif /* S_FILEHANDLER.H*/