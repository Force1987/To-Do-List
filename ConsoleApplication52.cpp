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

const pair<string, int> months[13] = { {"",0},{"January",31}, {"February",28}, {"March",31}, {"April",30}, {"May",31}, {"June",30}, {"July",31}, {"August",31},
											  {"September",30}, {"October",31}, {"November",30}, {"December",31} };

	map<int, map<int, map<int, set<Task>>>> list;

	set<Task>* getDay(const int& year, const int& month, const int& day) {
		if (list.find(year) == list.end())
			return NULL;
		if (list[year].find(month) == list[year].end())
			return NULL;
		if (list[year][month].find(day) == list[year][month].end())
			return NULL;
		return &list[year][month][day];
	}

public:

	void addTask(Date date, string time, string task) {
		if (list[date.year][date.month][date.day].insert({ date,time,task }).second)
			cout << "Task is successfully added.\n";
		else
			cout << "Task is already exists.\n";
	}

	void delTask(Task task, bool transfer=0) {
		set<Task>* tptr;
		tptr = getDay(task.date.year, task.date.month, task.date.day);
		if (tptr != NULL) {
			tptr->erase(task);
			if (transfer == 0) {
				cout << task << " is deleted.\n";
			}
			else
				cout << "No such task exists.\n";
		}
	}

	void listTasks() {
		for (auto year : list) {
			cout << year.first << " year:\n";
			for (auto month : year.second) {
				cout << "\t" << months[month.first].first<<":\n";
				for (auto day : month.second) {
					cout << "\t\t" << day.first << ":\n";
					for (auto task : day.second) {
						cout << task << endl;
					}
				}
			}
		}
	}

	void listTasks(Date date) {
		set<Task>* day = getDay(date.year, date.month, date.day);
		if (day) {
			for (const auto& task : *day)
				cout << task;
		}
		else
			cout << "No scheduled tasks for " << date<<endl;
	}

	void transferIncomplete(Date date) {
		set<Task>* day = getDay(date.year, date.month, date.day);
		int counter = 0;
		if (day) {
			for (auto& task:*day) {
				if (!task.complete) {
					++counter;
					if (((date.year % 400 == 0) || ((date.year % 4 == 0) && (date.year % 100 != 0))) && date.month == 2) {  //високосный год
						if (date.day == 29)
							list[date.year][date.month + 1][1].insert(task);
						else
							list[date.year][date.month][date.day + 1].insert(task);
					}
					else if (months[date.month].second == date.day) {														//последний день месяца
						if (date.month == 12)
							list[date.year + 1][1][1].insert(task);
						else
							list[date.year][date.month + 1][1].insert(task);
					}
					else
						list[date.year][date.month][date.day + 1].insert(task);												// общий случай
					delTask(task,true);
					if (day->empty()) {
						list[date.year][date.month].erase(date.day);
						break;
					}
				}
			}
		}
		if (counter==0)
			cout << "No incomplete tasks for " << date<<endl;
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
	myList.transferIncomplete({ 2022,8,29 });
	myList.listTasks({2022,8,30});
	myList.addTask({ 2022,8,29 }, "17:22"s, "Learn C++"s);
	myList.transferIncomplete({ 2022,8,29 });
	myList.listTasks({2022,8,30});
}