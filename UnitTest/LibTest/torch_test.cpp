#include <torch/torch.h>
#include <torch/script.h>
#include <iostream>

int main() {
    torch::Tensor tensor = torch::rand({2, 3});
    std::cout << tensor << std::endl;
}