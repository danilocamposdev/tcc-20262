#pragma once
#include <string>

class Client {
private:
    int id_;
    std::string name_;
public:
    Client(int id, std::string name)
        : id_{id}, name_{std::move(name)} {}

    int id() const { return id_; }
    std::string name() const { return name_; }
};
