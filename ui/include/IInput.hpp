#pragma once
#include <utility>
struct IInput {
    virtual std::pair<int,int> getSelection() = 0;
    virtual ~IInput() = default;
};
