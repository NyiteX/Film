#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h>
#include <mutex>
#include <vector>
#include <string>
#include <fstream>
using namespace std;
static bool Keep = true;

mutex m;
class Film
{
	string name;
	string director;
	string year;
	string ganre;
	string LOG;
public:
	string getName()const { return name; }
	string getDirector()const { return director; }
	string getYear()const { return year; }
	string getGanre()const { return ganre; }
	void setName(const string& n) { name = n; }
	void setDirector(const string& n) { director = n; }
	void setYear(const string& n) { year = n; }
	void setGanre(const string& n) { ganre = n; }
	void Vvod()
	{
		cout << "Name: ";
		cin >> name;
		cout << "Director: ";
		cin >> director;
		cout << "Year: ";
		cin >> year;
		cout << "Ganre: ";
		cin >> ganre;
	}
	void Print()const
	{
		m.lock();
		cout << "Name: " << name << endl;
		cout << "Director: " << director << endl;
		cout << "Year: " << year << endl;
		cout << "Ganre: " << ganre << endl;
		m.unlock();
	}
	void PrintALL(const vector<Film>& A)
	{
		for (int i = 0; i < A.size(); i++)
		{
			cout << "~~~~~~~~~ " << i + 1 << " ~~~~~~~~~\n";
			A[i].Print();
			cout << endl;
		}
	}
	int SearchByName(const vector<Film>& A)
	{
		string str;
		cout << "Search by name.\n";
		cout << "Enter name: ";
		cin >> str;
		for (int i = 0; i < A.size(); i++)
		{
			if (!A[i].name.find(str))
			{
				cout <<i+1<<": " << A[i].name << endl;
				return i;
			}			
		}
		cout << "Film is missing.\n";
		return 0;
	}
	void Load()
	{
		string path = "Film.txt";
		ifstream in(path);
		while (!in.is_open())
		{
			this_thread::sleep_for(chrono::milliseconds(1000));
		}
		while (!in.eof())
		{
			string temp;
			getline(in, temp);
			LOG += temp;
			if (!in.eof())
				LOG += '\n';
		}
		in.close();

	}
	void SaveToFile(const vector<Film>& A)
	{
		string path = "Film.txt";
		ofstream out(path);
		out.close();
		while(Keep == true)
		{
			Load();
			out.open(path);
			while (!out.is_open())
			{
				this_thread::sleep_for(chrono::milliseconds(1000));
			}
			if(out.is_open())
			{
				string tmp;			
				bool f = true;
				for (int i = 0; i < A.size(); i++)
				{					
					tmp += "Name: ";
					tmp += A[i].name;
					tmp += "\n";
					tmp += "Director: ";
					tmp += A[i].director;
					tmp += "\n";
					tmp += "Year: ";
					tmp += A[i].year;
					tmp += "\n";
					tmp += "Ganre: ";
					tmp += A[i].ganre;
					tmp += "\n";
				}			
				if(tmp.size()==LOG.size())
				{
					for (int i = 0; i < LOG.size(); i++)
					{
						if (tmp[i] != LOG[i])
						{
							f = false;
							break;
						}
					}
				}
				if (tmp.size() != LOG.size() || f == false /*|| LOG.size()>0*/)
				{
					for (int i = 0; i < A.size(); i++)
					{
						out << "Name: " << A[i].name << endl;
						out << "Director: " << A[i].director << endl;
						out << "Year: " << A[i].year << endl;
						out << "Ganre: " << A[i].ganre << endl;						
					}
				}
				out.close();
				LOG.clear();
				this_thread::sleep_for(chrono::seconds(30));			
			}
		}	
	}
};

int main()
{
	vector<Film>A;
	Film C;
	thread t0(&Film::SaveToFile, C, ref(A));
	char vvod;
	do
	{
		system("cls");
		cout << "1. Add film.\n";
		cout << "2. Del film.\n";
		cout << "3. Edit film.\n";
		cout << "4. Print ALL.\n";
		cout << "5. DELETE ALL.\n";
		cout << "ESC. Exit.\n~~~~~~~~~~~~~~~~~~~\n";
		vvod = _getch();
		switch (vvod)
		{
		case'1':
		{
			thread t1(&Film::Vvod, ref(C));
			t1.join();
			A.push_back(C);
			system("pause");
			break;
		}
		case'2':
		{
			if (A.size() > 0)
			{
				int id;
				auto f = [&]() {
					 id = C.SearchByName(A);
				};
				thread t2(f);
				t2.join();
				A.erase(A.begin() + id);
				cout << "\tDeleted.\n";
			}
			else
				cout << "Film list is empty.\n";
			system("pause");
			break;
		}
		case'3':
		{
			if (A.size() > 0)
			{
				int id;
				auto f = [&]() {
					id = C.SearchByName(A);
				};
				thread t2(f);
				t2.join();
				
				char vvod2;
				do
				{
					system("cls");
					cout << "1. Change Name.\n";
					cout << "2. Change Director.\n";
					cout << "3. Change Year.\n";
					cout << "4. Change Ganre.\n";
					cout << "5. Print.\n";
					cout << "ESC. Exit.\n~~~~~~~~~~~~~~~~~~~\n";
					vvod2 = _getch();
					switch (vvod2)
					{
					case'1':
					{
						string n;
						cout << "Enter new Name: ";
						cin >> n;
						A[id].setName(n);
						system("pause");
						break;
					}
					case'2':
					{
						string n;
						cout << "Enter new Director: ";
						cin >> n;
						A[id].setDirector(n);
						system("pause");
						break;
					}
					case'3':
					{
						string n;
						cout << "Enter new data(year): ";
						cin >> n;
						A[id].setYear(n);
						system("pause");
						break;
					}
					case'4':
					{
						string n;
						cout << "Enter new Ganre: ";
						cin >> n;
						A[id].setGanre(n);
						system("pause");
						break;
					}
					case'5':
					{
						A[id].Print();
						system("pause");
						break;
					}
					}

				} while (vvod2 != 27);
			}
			else
			{				
				cout << "Film list is empty.\n";
				system("pause");
			}
			break;
		}
		case'4':
		{
			if (A.size() > 0)
			{
				C.PrintALL(A);
			}
			else
				cout << "Film list is empty.\n";
			system("pause");
			break;
		}
		case'5':
		{
			if (A.size() > 0)
			{
				A.clear();
			}
			else
				cout << "Film list is empty.\n";
			system("pause");
			break;
		}
		default:
			break;
		}

	} while (vvod != 27);
	Keep = false;
	t0.join();
}