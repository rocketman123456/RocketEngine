#include <taskflow/taskflow.hpp>  // Taskflow is header-only

int main(){
    
    tf::Executor executor;
    tf::Taskflow taskflow;

    tf::Task A = taskflow.emplace([](){}).name("A");  
    tf::Task C = taskflow.emplace([](){}).name("C");  
    tf::Task D = taskflow.emplace([](){}).name("D");  

    tf::Task B = taskflow.emplace([] (tf::Subflow& subflow) { // subflow task B
        tf::Task B1 = subflow.emplace([](){}).name("B1");  
        tf::Task B2 = subflow.emplace([](){}).name("B2");  
        tf::Task B3 = subflow.emplace([](){}).name("B3");  
        B3.succeed(B1, B2);  // B3 runs after B1 and B2
    }).name("B");                           
    
    A.precede(B, C);  // A runs before B and C
    D.succeed(B, C);  // D runs after  B and C

    executor.run(taskflow).wait(); 

    tf::Taskflow f1, f2;

    // create taskflow f1 of two tasks
    tf::Task f1A = f1.emplace([]() { std::cout << "Task f1A\n"; }).name("f1A");
    tf::Task f1B = f1.emplace([]() { std::cout << "Task f1B\n"; }).name("f1B");

    // create taskflow f2 with one module task composed of f1
    tf::Task f2A = f2.emplace([]() { std::cout << "Task f2A\n"; }).name("f2A");
    tf::Task f2B = f2.emplace([]() { std::cout << "Task f2B\n"; }).name("f2B");
    tf::Task f2C = f2.emplace([]() { std::cout << "Task f2C\n"; }).name("f2C");
    tf::Task f1_module_task = f2.composed_of(f1).name("module");

    f1_module_task.succeed(f2A, f2B).precede(f2C);

    executor.run(f2).wait(); 

    taskflow.dump(std::cout);

    return 0;
}