#pragma once
#include <string>

class Buffer
{
private:
    std::string buf;
public:
    Buffer();
    ~Buffer();

    void append(const char *, int);
    void clear();
    
    const char* c_str() const { return buf.c_str(); }
    ssize_t size() const { return buf.size(); }

    void getline();
    void setBuf(const char*);
};
