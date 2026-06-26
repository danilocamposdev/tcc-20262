#pragma once
#include <string>

class Priority {
public:
    static const Priority Low;
    static const Priority Regular;
    static const Priority High;

    int index()             const { return index_; }  // salvo no banco
    int value()             const { return value_; }  // usado na otimização
    std::string to_string()  const { return title_; }  // usado na UI

    bool operator==(const Priority& other) const { return index_ == other.index_; }

    static Priority fromIndex(int index) {
        switch (index) {
            case 1:  return Low;
            case 2:  return Regular;
            default: return High;
        }
    }

private:
    Priority(int index, int value, std::string title)
        : index_(index), value_(value), title_(title) {}

    int index_;
    int value_;
    std::string title_;
};

inline const Priority Priority::Low     {1, 1,   "Baixa"};
inline const Priority Priority::Regular {2, 5,   "Regular"};
inline const Priority Priority::High    {3, 100, "Alta"};
