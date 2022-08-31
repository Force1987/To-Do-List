#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <string>
using namespace std;

class ToDo_List {
private:
struct Date
{
	int year;
	int month;
	int day;
	bool operator < (const Date& right) const {
		if (year < right.year) return true;
		if (year == right.year && month < right.month) return true;
		if (year == right.year && month == right.month && day < right.day) return true;
		return false;
	}
	bool operator == (const Date& right) const {
		return year == right.year && month == right.month && day == right.day;
	}
};
friend ostream& operator<< (ostream& out, const Date& point);

struct Task
{
	Date date;
	string time;
	string task;
	bool complete = false;
	bool operator < (const Task& right) const {
		if (date < right.date) return true;
		if (date == right.date&&time < right.time) return true;
		return false;
	}
	bool operator == (const Task& right) const {
		return time == right.time && date == right.date && task == right.task;
	}
};
friend ostream& operator<< (ostream& out, const Task& point);

const string months[13] = { "","January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

	map<int, map<int, map<int, set<Task>>>> list;

public:

	set<Task>* const getDay (const int& year, const int& month, const int& day) {
	if (list.find(year) == list.end())
		return NULL;
	if (list[year].find(month) == list[year].end())
		return NULL;
	if (list[year][month].find(day) == list[year][month].end())
		return NULL;
	return &list[year][month][day];
}

	void addTask(Date date, string time, string task) {
		if (list[date.year][date.month][date.day].insert({ date,time,task }).second)
			cout << "Task is successfully added.\n";
		else
			cout << "Task is already exists.\n";
	}

	void delTask(Task task) {
		set<Task>* tptr;
		tptr = getDay(task.date.year, task.date.month, task.date.day);
		if (tptr != NULL) {
			tptr->erase(task);
			cout << task << " is deleted.\n";
		}
		else
			cout << "No such task exists.\n";
	}
	
	void delTask(set<ToDo_List::Task>::iterator task) {
		list[task->date.year][task->date.month][task->date.day].erase(task);
	}

	void listTasks() {
		for (auto year : list) {
			cout << year.first << " year:\n";
			for (auto month : year.second) {
				cout << "\t" << months[month.first]<<":\n";
				for (auto day : month.second) {
					cout << "\t\t" << day.first << ":\n";
					for (auto task : day.second) {
						cout << task << endl;
					}
				}
			}
		}
		cout << endl;
	}
};

ostream& operator<< (ostream& out, const ToDo_List::Date& date)
{
	out << date.day << "." << date.month << "." << date.year;
	return out;
}

ostream& operator<< (ostream& out, const ToDo_List::Task& task)
{
	out << "Task " << task.date << " " << task.time << " \"" << task.task<<"\"";
	return out;
}

int main()
{
	ToDo_List myList;
	myList.addTask({ 2022,8,29 }, "17:22"s, "Learn C++"s);
	myList.addTask({ 2022,8,29 }, "17:22"s, "Learn C++"s);
	myList.listTasks();
	myList.delTask({ { 2022,8,29 }, "17:22"s, "Learn C++"s });
}