//
// Created by Eden on 26/07/2024.
//

#include "TaskManager.h"
#include "SortedList.h"


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


// Default C'tor
TaskManager::TaskManager() {
    employeesAmount = 0;
    employees = new Person[MAX_PERSONS];
}

// Aid functions declarations
void fillListWithTasks(const SortedList<Task> &list, const Person &person);
void
fillListWithTasks(const SortedList<Task> &list, const Person &person,
                  TaskType type);
const Person &lookForEmployee(const string &personName, int employeesAmount,
                              const Person *employees);


// Assigns task to employee
void TaskManager::assignTask(const string &personName, const Task &task) {
    Person employee;

    try {
        employee = lookForEmployee(personName, employeesAmount, employees);
    } catch (EmployeeNotFound &) {
        if (employeesAmount == MAX_PERSONS) {
            throw runtime_error("Surpassed maximum employees amount. (" +
                                to_string(MAX_PERSONS) + ")");
        }

        // add a new employ and assign the task
        Person newEmp(personName);
        SortedList<Task> tasks;
        newEmp.setTasks(tasks);
        newEmp.assignTask(task);
        employees[employeesAmount++] = newEmp;
    }

    employee.assignTask(task);
}

void TaskManager::completeTask(const string &personName) {
    Person employee;
    try {
        employee = lookForEmployee(personName, employeesAmount, employees);
    } catch (EmployeeNotFound &e) {
        cerr << "Error: " << e.what() << endl;
        return;
    }

    employee.completeTask();
}

void TaskManager::bumpPriorityByType(TaskType type, int priority) {

    for (int emp = 0; emp < employeesAmount; ++emp) {
        SortedList<Task> list;
        try {
            list = employees[emp].getTasks();
        } catch (const bad_alloc &a) {
            cerr << "Allocation failed: " << a.what() << endl;
            throw;
        }
        for (SortedList<Task>::ConstIterator it = list.begin();
             it != list.end(); ++it) {
            if ((*it).getType() == type) {
                Task newTask((*it).getPriority() + priority, type,
                             (*it).getDescription());
                newTask.setId((*it).getId());

                list.remove(it);
                list.insert(newTask);
            }
        }
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


/**
 * @brief Searches for an employee in the array.
 *
 * @param personName The name of the person lookup.
 * @param employees The array of the employees.
 * @param employeesAmount The amount of current employees.
 */
const Person &
lookForEmployee(const string &personName, const int employeesAmount,
                const Person *employees) {
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
    SortedList<Task> personTasks = person.getTasks();
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
                  TaskType type) {
    SortedList<Task> personTasks = person.getTasks();
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