//
// Created by Eden on 26/07/2024.
//

#include "TaskManager.h"
#include "SortedList.h"
#include "Node.h"
#include "Person.h"
#include "Task.h"


using namespace std;

/**
 * Exception classes
 */
class EmployeeNotFound {
private:
    string message;
public:
    EmployeeNotFound() = default;

    explicit EmployeeNotFound(const string &str) : message("The employee " +
                                                           str +
                                                           " was not found.") {};

    const string &what() { return message; };
};



// Aid functions declarations

/**
 * @brief Searches for an employee in the array.
 *
 * @param personName The name of the person lookup.
 * @param employees The array of the employees.
 * @param employeesAmount The amount of current employees.
 */
Person &
lookForEmployee(const string &personName, const int employeesAmount,
                Person *employees) {
    for (int emp = 0; emp < employeesAmount; ++emp) {
        if (employees[emp].getName() == personName) {
            return employees[emp];
        }
    }

    throw EmployeeNotFound(personName);
}

/**
 * @brief Insert employees tasks to a list by priority.
 *
 * @param list SortedList of tasks to be edited.
 * @param person Person to get task list and add the tasks to list.
 */
void fillListWithTasks(SortedList<Task> &list, const Person &person) {
    SortedList<Task> personTasks;
    try {
        personTasks = person.getTasks();
    } catch (...) {
        throw std::bad_alloc();
    }

    for (SortedList<Task>::ConstIterator i = personTasks.begin();
         i != personTasks.end(); ++i) {
        try {
            list.insert(*i);
        } catch (...) {
            cerr << "Insertion Failed" << endl;
        }
    }
}

/**
 * @brief Insert specific type employees tasks to a list by priority.
 *
 * @param list SortedList of tasks to be edited.
 * @param person Person to get task list and add the tasks to list.
 * @param type TaskType to look for.
 */
void
fillListWithTasks(SortedList<Task> &list, const Person &person,
                  TaskType &type) {
    SortedList<Task> personTasks;
    try {
        personTasks = person.getTasks();
    } catch (const bad_alloc &) {
        throw;
    }

    for (SortedList<Task>::ConstIterator i = personTasks.begin();
         i != personTasks.end(); ++i) {
        if ((*i).getType() == type) {
            try {
                list.insert(*i);
            } catch (...) {
                cerr << "Insertion Failed" << endl;
            }
        }
    }
}

// Default C'tor
TaskManager::TaskManager() {
    employeesAmount = 0;
    currentTaskNum = 0;
    employees = new Person[MAX_PERSONS];
}

TaskManager::~TaskManager() {
    delete[] this->employees;
}

// Assigns task to employee
void TaskManager::assignTask(const string &personName, const Task &task) {
    Task taskCopy(task.getPriority(), task.getType(), task.getDescription());
    taskCopy.setId(this->currentTaskNum++);

    try {
        Person &employee = lookForEmployee(personName, employeesAmount,
                                           employees);
        employee.assignTask(taskCopy);
    } catch (EmployeeNotFound &) {
        if (employeesAmount == MAX_PERSONS) {
            throw runtime_error("Surpassed maximum employees amount. (" +
                                to_string(MAX_PERSONS) + ")");

        }

        // add a new employ and assign the task
        Person newEmp(personName);
        SortedList<Task> tasks;
        newEmp.setTasks(tasks);
        newEmp.assignTask(taskCopy);
        employees[employeesAmount++] = newEmp;
        return;
    }
}

void TaskManager::completeTask(const string &personName) {
    try {
        Person &employee = lookForEmployee(personName, employeesAmount,
                                           employees);
        employee.completeTask();
    } catch (EmployeeNotFound &e) {
        cerr << "Error: " << e.what() << endl;
        return;
    }

}


void TaskManager::bumpPriorityByType(TaskType type, int priority) {
    for (int emp = 0; emp < employeesAmount; ++emp) {
        SortedList<Task> updated_list;
        updated_list = employees[emp].getTasks().apply([type, priority](const Task & task) -> Task{
            int new_priority = task.getPriority();
            if(task.getType() == type)
            {
                new_priority += priority;
            }
            Task new_task(new_priority,task.getType(), task.getDescription());
            new_task.setId(task.getId());
            return new_task;
        });

        /* for (SortedList<Task>::ConstIterator it = list.begin();
             it != list.end(); ++it) {
            if ((*it).getType() == type) {
                Task newTask((*it).getPriority() + priority, type,
                             (*it).getDescription());
                newTask.setId((*it).getId());
                list.remove(it);
                try {
                    list.insert(newTask);
                } catch (const bad_alloc &) {
                    throw;
                }
            }
        } */
        employees[emp].setTasks(updated_list);
    }

}

void TaskManager::printAllEmployees() const {
    for (int emp = 0; emp < employeesAmount; ++emp) {
        cout << employees[emp] << endl;
    }
}

void TaskManager::printAllTasks() const {
    SortedList<Task> allTasks;
    for (int emp = 0; emp < employeesAmount; ++emp) {
        fillListWithTasks(allTasks, employees[emp]);
    }

    for (SortedList<Task>::ConstIterator i = allTasks.begin();
         i != allTasks.end(); ++i) {
        cout << *i << endl;
    }
}

void TaskManager::printTasksByType(TaskType type) const {
    SortedList<Task> allTasks;
    for (int emp = 0; emp < employeesAmount; ++emp) {
        fillListWithTasks(allTasks, employees[emp], type);
    }

    for (SortedList<Task>::ConstIterator i = allTasks.begin();
         i != allTasks.end(); ++i) {
        cout << *i << endl;
    }
}
