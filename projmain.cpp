#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <fstream>
#include <windows.h>
#include <limits>
#include <algorithm>
#include <vector>
using namespace std;

string getSavePath () {
	char* userProfile = getenv("USERPROFILE");
	if (userProfile == nullptr) {
		system("cls");
		cout << "Failed to get user profile path." << endl;
		this_thread::sleep_for(chrono::seconds(2));
		cout << "Please try again later." << endl;
		this_thread::sleep_for(chrono::seconds(2));
		cout << "Exiting the program..." << endl;
		this_thread::sleep_for(chrono::seconds(3));
		exit(1);
	}

	string fullPath = string(userProfile) + "\\Documents\\PassGuard\\";
	CreateDirectoryA(fullPath.c_str(), NULL);
	return fullPath;
}

void RegisterUser() {
	string path = getSavePath();
	string filename = path + "user_data.bin";
	
	ifstream checkFile(filename, ios::binary);
	if (checkFile.good()) {
		system("cls");
		cout << "Registration blocked." << endl;
		this_thread::sleep_for(chrono::seconds(2));
		cout << "A user is already registered." << endl;
		this_thread::sleep_for(chrono::seconds(2));
		cout << "Returning to the Sign Up/Login page for the login..." << endl;
		this_thread::sleep_for(chrono::seconds(3));
		system("cls");
		return;
	}
	checkFile.close();
	
	string email, username, pass, confPass;
	system("cls");
	cout << "----- Sign Up Page -----" << endl;
	cout << endl;
	cout << "Enter your email: ";
	cin >> email;
	cout << "Enter your username: ";
	cin >> username;
	cout << "Enter your password: ";
	cin >> pass;
	cout << "Confirm your password: ";
	cin >> confPass;
	
	if (pass != confPass) {
		system("cls");
		cout << "Registration failed." << endl;
		this_thread::sleep_for(chrono::seconds(2));
		cout << "Passwords does not match." << endl;
		this_thread::sleep_for(chrono::seconds(2));
		cout << "Exiting the program..." << endl;
		this_thread::sleep_for(chrono::seconds(3));
		exit(1);
	}
	
	ofstream outFile(filename, ios::binary);
	if (!outFile) {
		system("cls");
		cout << "Failed to create user data file." << endl;
		this_thread::sleep_for(chrono::seconds(2));
		cout << "Please try again later." << endl;
		this_thread::sleep_for(chrono::seconds(2));
		cout << "Exiting the program..." << endl;
		this_thread::sleep_for(chrono::seconds(3));
        exit(1);
	}
	
	outFile << username << '\n' << email << '\n' << pass << '\n';
	outFile.close();
	system("cls");
	cout << "Successful Registration!" << endl;
	this_thread::sleep_for(chrono::seconds(2));
    cout << "Returning to the Sign Up/Login page..." << endl;
    this_thread::sleep_for(chrono::seconds(3));
    system("cls");
}

void changePass(const string& filename, const string& email, const string& username, bool fMM);

void changePass(const string& filename, const string& email, const string& username, bool fMM) {
	string NewPass, confNewPass;
	
	while (true) {
		system("cls");
		cout << "----- Password Reset -----" << endl;
		cout << endl;
		cout << "Enter your new password: ";
		cin >> NewPass;
		cout << "Confirm your new password: ";
		cin >> confNewPass;
		
		if (NewPass == confNewPass) {
			ofstream outFile(filename, ios::binary | ios::trunc);
			if (!outFile) {
				system("cls");
				cout << "Failed to update password." << endl;
				this_thread::sleep_for(chrono::seconds(2));
				if (fMM) {
					cout << "Returning to the Main Menu..." << endl;
				} else {
					cout << "Returning to the Sign Up/Login page..." << endl;
				}
				this_thread::sleep_for(chrono::seconds(3));
				system("cls");
				return;
			}
			outFile << username << '\n' << email << '\n' << NewPass << '\n';
			outFile.close();
			system("cls");
			cout << "Password Updated Successfully!" << endl;
			this_thread::sleep_for(chrono::seconds(2));
			if (fMM) {
				cout << "Returning to the Main Menu page..." << endl;
			} else {
				cout << "Returning to the Sign Up/Login page..." << endl;
			}
			this_thread::sleep_for(chrono::seconds(3));
			system("cls");
			return;
 		} else {
		 	system("cls");
			cout << "Passwords don't match." << endl;
			this_thread::sleep_for(chrono::seconds(2));
			cout << "Please try again." << endl;
			this_thread::sleep_for(chrono::seconds(2));
		}
	}
}

void MainMenu(const string& filename, const string& email, const string& username);

void LoginUser() {
	string path = getSavePath();
	string filename = path + "user_data.bin";
	
	ifstream inFile(filename, ios::binary);
	if (!inFile) {
		system("cls");
		cout << "No user registered." << endl;
		this_thread::sleep_for(chrono::seconds(2));
		cout << "Please register." << endl;
		this_thread::sleep_for(chrono::seconds(2));
		cout << "Returning to the Sign Up/Login page..." << endl;
		this_thread::sleep_for(chrono::seconds(3));
		system("cls");
		return;
	}
	
	string storedUsername, storedEmail, storedPass;
	getline(inFile, storedUsername);
	getline(inFile, storedEmail);
	getline(inFile, storedPass);
	inFile.close();
	
	string inputUsem, inputPass;
	
	while (true) {
		system("cls");
		cout << "----- Login Page -----" << endl;
		cout << endl;
		cout << "Enter your username or email: ";
		cin >> inputUsem;
		cin.ignore();
		if (inputUsem != storedUsername && inputUsem != storedEmail) {
			system("cls");
			cout << "Wrong Username or Email." << endl;
			this_thread::sleep_for(chrono::seconds(2));
			cout << "Please try again." << endl;
			this_thread::sleep_for(chrono::seconds(3));
			system("cls");
			continue;
		}
		
		int attempts = 0;
		while (attempts < 3) {
			cout << "Enter your password (or type 'reset password' to reset it): ";
			getline(cin, inputPass);
			cout << endl;
			if (inputPass == "reset password") {
				while (true) {
					system("cls");
					string RecoveryEmail;
					cout << "----- Password Reset -----" << endl;
					cout << endl;
					cout << "Enter your registered email to reset your password: ";
					cin >> RecoveryEmail;
					cin.ignore();
				
					if (RecoveryEmail == storedEmail) {
						changePass(filename, storedEmail, storedUsername, false);
						return;
					} else {
						system("cls");
						cout << "Invalid Registered Email." << endl;
						this_thread::sleep_for(chrono::seconds(2));
						cout << "Please try again." << endl;
						this_thread::sleep_for(chrono::seconds(3));
						system("cls");
						continue;
					}
				}
			}
		
			if (inputPass == storedPass) {
				system("cls");
				cout << "Successful Login!" << endl;
				this_thread::sleep_for(chrono::seconds(2));
				cout << "Redirecting to the Main Menu page..." << endl;
				this_thread::sleep_for(chrono::seconds(3));
				system("cls");
				MainMenu(filename, storedEmail, storedUsername);
				return;
			} else {
				system("cls");
				attempts = attempts + 1;
				cout << "Incorrect Password." << endl;
				this_thread::sleep_for(chrono::seconds(2));
				cout << "Attempts Left: " << (3 - attempts) << endl;
				this_thread::sleep_for(chrono::seconds(2));
				if (attempts == 2 || attempts == 1) {
					cout << "Please try again." << endl;
					this_thread::sleep_for(chrono::seconds(3));
					system("cls");
				}
			}
		}
		
		cout << "Too many failed attempts." << endl;
		this_thread::sleep_for(chrono::seconds(2));
		cout << endl;
		cout << "Please wait 30 seconds before trying again..." << endl;
		this_thread::sleep_for(chrono::seconds(30));
		system("cls");
	}	
}

struct Entry {
	string AppName;
	string Username;
	string Email;
	string Pass;
};

void AddEntry();

void AddEntry() {
	string AppName, Username, Email, Pass;
	string path = getSavePath();
	string filename = path + "data.bin";
	system("cls");
	cout << "----- Add Entry -----" << endl;
	cout << endl;
	cout << "Enter the name of the website or the name of the app: ";
	getline(cin, AppName);
	
	cout << "Enter your username: ";
	getline(cin, Username);
	
	cout << "Enter your email: ";
	getline(cin, Email);
	
	cout << "Enter your password: ";
	getline(cin, Pass);
	
	Entry newEntry = { AppName, Username, Email, Pass };
	
	ofstream outFile(filename, ios::binary | ios::app);
	
	if (!outFile.is_open()) {
		system("cls");
		cout << "Error opening the file." << endl;
		this_thread::sleep_for(chrono::seconds(2));
		cout << "Exiting the program..." << endl;
		this_thread::sleep_for(chrono::seconds(3));
		exit(1);
	}
	
	size_t len;
	
	len = newEntry.AppName.size();
	outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
	outFile.write(newEntry.AppName.c_str(), len);
	
	len = newEntry.Username.size();
	outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
	outFile.write(newEntry.Username.c_str(), len);
	
	len = newEntry.Email.size();
	outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
	outFile.write(newEntry.Email.c_str(), len);
	
	len = newEntry.Pass.size();
	outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
	outFile.write(newEntry.Pass.c_str(), len);
	
	outFile.close();
	
	if(!outFile.good()) {
		system("cls");
		cout << "Error writing to the file." << endl;
		this_thread::sleep_for(chrono::seconds(2));
		cout << "Exiting the program..." << endl;
		this_thread::sleep_for(chrono::seconds(3));
		system("cls");
		exit(1);
	}
	
	system("cls");
	cout << "Entry Added Successfully!" << endl;
	this_thread::sleep_for(chrono::seconds(2));
	cout << "Returning to the Main Menu page..." << endl;
	this_thread::sleep_for(chrono::seconds(3));
	system("cls");
}

void EditEntry();

void EditEntry() {
	while (true) {
		string path = getSavePath();
		string filename = path + "data.bin";
	
		ifstream inFile(filename, ios::binary);
		if(!inFile) {
			system("cls");
			cout << "Cannot open or missing this file." << endl;
			this_thread::sleep_for(chrono::seconds(2));
			cout << "Returning to the Main Menu page..." << endl;
			this_thread::sleep_for(chrono::seconds(3));
			system("cls");
			return;
		}
	
		vector<Entry> entries;
	
		while (true) {
			Entry e;
			size_t len;
			char* buffer;
		
			if (!inFile.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
			buffer = new char[len + 1];
			if (!inFile.read(buffer, len)) {
				delete[] buffer;
				break;
			}
			buffer[len] = '\0';
			e.AppName = buffer;
			delete[] buffer;
		
			if (!inFile.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
			buffer = new char[len + 1];
			if (!inFile.read(buffer, len)) {
				delete[] buffer;
				break;
			}
			buffer[len] = '\0';
			e.Username = buffer;
			delete[] buffer;
		
			if (!inFile.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
			buffer = new char[len + 1];
			if (!inFile.read(buffer, len)) {
				delete[] buffer;
				break;
			}
			buffer[len] = '\0';
			e.Email = buffer;
			delete[] buffer;
		
			if (!inFile.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
			buffer = new char[len + 1];
			if (!inFile.read(buffer, len)) {
				delete[] buffer;
				break;
			}
			buffer[len] = '\0';
			e.Pass = buffer;
			delete[] buffer;
		
			entries.push_back(e);
		}
		inFile.close();
	
		if (entries.empty()) {
			system("cls");
			cout << "No entries found to edit." << endl;
			this_thread::sleep_for(chrono::seconds(2));
			cout << "Returning to the Main Menu page..." << endl;
			this_thread::sleep_for(chrono::seconds(3));
			system("cls");
			return;
		}
	
		system("cls");
	
		cout << "----- Edit Entry [page 1/3] -----" << endl;
		for (size_t i = 0; i < entries.size(); i++) {
			cout << i + 1 << ". " << entries[i].AppName << endl;
		}
		cout << endl;
		cout << "Your choice: ";
		int choice3;
		cin >> choice3;
	
		if (cin.fail()) {
			cin.clear();
			cin.ignore();
			system("cls");
			cout << "Invalid input." << endl;
			this_thread::sleep_for(chrono::seconds(2));
			cout << "You must enter a number." << endl;
			this_thread::sleep_for(chrono::seconds(2));
			cout << "Returning to the Main Menu page..." << endl;
			this_thread::sleep_for(chrono::seconds(3));
			system("cls");
			return;
		}
	
		if (choice3 < 1 || choice3 > (int)entries.size()) {
			cin.clear();
			cin.ignore();
			system("cls");
			cout << "Invalid choice." << endl;
			this_thread::sleep_for(chrono::seconds(2));
			cout << "Please try again." << endl;
			this_thread::sleep_for(chrono::seconds(2));
			cout << "Returning to the Edit Section [page 1/3]..." << endl;
			this_thread::sleep_for(chrono::seconds(3));
			continue;
		}
	
		cin.ignore();
		Entry& selected = entries[choice3 - 1];
		while (true) {
			system("cls");
			int field;
			cout << "----- Edit Entry [page 2/3] -----" << endl;
			cout << endl;
			cout << "1. App Name" << endl;
			cout << "2. Username" << endl;
			cout << "3. Email" << endl;
			cout << "4. Password" << endl;
			cout << endl;
			cout << "Your choice: ";
			cin >> field;
	
			if (cin.fail()) {
				cin.clear();
				cin.ignore();
				system("cls");
				cout << "Invalid input." << endl;
				this_thread::sleep_for(chrono::seconds(2));
				cout << "You must enter a number." << endl;
				this_thread::sleep_for(chrono::seconds(2));
				cout << "Returning to the Main Menu page..." << endl;
				this_thread::sleep_for(chrono::seconds(3));
				system("cls");
				return;
			}
	
			cin.ignore();
			string newValue;
		
			if (field >= 1 && field <=4) {
				system("cls");
				cout << "----- Edit Entry [page 3/3] -----" << endl;
				cout << endl;
				cout << "Enter the new value: ";
				getline(cin, newValue);
				system("cls"); 
				switch(field) {
					case 1:
						selected.AppName = newValue;
						break;
					case 2:
						selected.Username = newValue;
						break;
					case 3:
						selected.Email = newValue;
						break;
					case 4:
						selected.Pass = newValue;
						break;
				}
				break;
			} else {
				system("cls");
				cout << "Invalid choice." << endl;
				this_thread::sleep_for(chrono::seconds(2));
				cout << "Please try again." << endl;
				this_thread::sleep_for(chrono::seconds(2));
				cout << "Returning to the Edit Section [page 2/3]..." << endl;
				this_thread::sleep_for(chrono::seconds(3));
				system("cls");
				continue;
			}
		}		
	
		ofstream outFile(filename, ios::binary | ios::trunc);
		for (const auto& e : entries) {
			size_t len;
		
			len = e.AppName.size();
			outFile.write(reinterpret_cast<const char*>(&len), sizeof(len));
			outFile.write(e.AppName.c_str(), len);
		
			len = e.Username.size();
			outFile.write(reinterpret_cast<const char*>(&len), sizeof(len));
			outFile.write(e.Username.c_str(), len);
			
			len = e.Email.size();
			outFile.write(reinterpret_cast<const char*>(&len), sizeof(len));
			outFile.write(e.Email.c_str(), len);
		
			len = e.Pass.size();
			outFile.write(reinterpret_cast<const char*>(&len), sizeof(len));
    		outFile.write(e.Pass.c_str(), len);
		}
	
		outFile.close();
		system("cls");
		cout << "Entry Updated Successfully!" << endl;
		this_thread::sleep_for(chrono::seconds(2));
		cout << "Returning to the Main Menu page..." << endl;
		this_thread::sleep_for(chrono::seconds(3));
		system("cls");
		return;
	}
}

string toLower(const string& str) {
	string result = str;
	transform(result.begin(), result.end(), result.begin(), ::tolower);
	return result;
}

void SearchEntry();

void SearchEntry() {
	string path = getSavePath();
	string filename = path + "data.bin";
	ifstream inFile(filename, ios::binary);
	
	if(!inFile) {
		system("cls");
		cout << "Cannot open or missing this file." << endl;
		this_thread::sleep_for(chrono::seconds(2));
		cout << "Returning to the Main Menu page..." << endl;
		this_thread::sleep_for(chrono::seconds(3));
		system("cls");
		return;
	}
	
	vector<Entry> entries;
	while (true) {
		Entry e;
		size_t len;
        char* buffer;
		
		if (!inFile.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
        buffer = new char[len + 1];
        if (!inFile.read(buffer, len)) { 
			delete[] buffer; 
			break; 
		}
        buffer[len] = '\0';
        e.AppName = buffer;
        delete[] buffer;
		
		if (!inFile.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
        buffer = new char[len + 1];
        if (!inFile.read(buffer, len)) {
			delete[] buffer; 
			break; 
		}
        buffer[len] = '\0';
        e.Username = buffer;
        delete[] buffer;
		
		if (!inFile.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
        buffer = new char[len + 1];
        if (!inFile.read(buffer, len)) { 
			delete[] buffer; 
			break; 
		}
        buffer[len] = '\0';
        e.Email = buffer;
        delete[] buffer;
		
		if (!inFile.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
        buffer = new char[len + 1];
        if (!inFile.read(buffer, len)) { 
			delete[] buffer; 
			break; 
		}
        buffer[len] = '\0';
        e.Pass = buffer;
        delete[] buffer;
		
		entries.push_back(e);
	}
	inFile.close();
	
	if (entries.empty()) {
		system("cls");
		cout << "No entries found to search." << endl;
		this_thread::sleep_for(chrono::seconds(2));
		cout << "Returning to the Main Menu page..." << endl;
		this_thread::sleep_for(chrono::seconds(3));
		system("cls");
		return;
	}
	system("cls");
	
	string WApp;
	cout << "----- Search Entry -----" << endl;
	cout << endl;
	cout << "Searching for: ";
	cin.clear();
	getline(cin >> ws, WApp);
	WApp = toLower(WApp);
	
	bool found = false;
	system("cls");
	cout << "----- Entry Found -----" << endl;
	cout << endl;

	for (const auto& e : entries) {
		if (toLower(e.AppName).find(toLower(WApp)) != string::npos) {
			cout << "App Name: " << e.AppName << endl;
			cout << "Username: " << e.Username << endl;
			cout << "Email: " << e.Email << endl;
			cout << "Password: " << e.Pass << endl;
			cout << endl;
			found = true;
		}
	}
	
	if (!found) {
		system("cls");
		cout << "No entry found." << endl;
		this_thread::sleep_for(chrono::seconds(2));
		cout << "Returning to the Main Menu page..." << endl;
		this_thread::sleep_for(chrono::seconds(3));
		system("cls");
	} else {
		cout << "Press ENTER to return to the Main Menu page...";
		cin.get();
		system("cls");
	}
}

void DeleteEntry();

void DeleteEntry() {
	string path = getSavePath();
	string filename = path + "data.bin";
	
	ifstream inFile(filename, ios::binary);
	if(!inFile) {
		system("cls");
		cout << "Cannot open or missing this file." << endl;
		this_thread::sleep_for(chrono::seconds(2));
		cout << "Returning to the Main Menu page..." << endl;
		this_thread::sleep_for(chrono::seconds(3));
		system("cls");
		return;
	}
	
	vector<Entry> entries;
	
	while (true) {
		Entry e;
        size_t len;
        char* buffer;
		
		if (!inFile.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
        buffer = new char[len + 1];
        if (!inFile.read(buffer, len)) { 
			delete[] buffer; 
			break; 
		}
        buffer[len] = '\0';
        e.AppName = buffer;
        delete[] buffer;
		
		if (!inFile.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
        buffer = new char[len + 1];
        if (!inFile.read(buffer, len)) { 
			delete[] buffer; 
			break; 
		}
        buffer[len] = '\0';
        e.Username = buffer;
        delete[] buffer;
		
		if (!inFile.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
        buffer = new char[len + 1];
        if (!inFile.read(buffer, len)) { 
			delete[] buffer; 
			break; 
		}
        buffer[len] = '\0';
        e.Email = buffer;
        delete[] buffer;
		
		if (!inFile.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
        buffer = new char[len + 1];
        if (!inFile.read(buffer, len)) { 
			delete[] buffer; 
			break; 
		}
        buffer[len] = '\0';
        e.Pass = buffer;
        delete[] buffer;
		
		entries.push_back(e);
	}
	inFile.close();
	
	if (entries.empty()) {
		system("cls");
		cout << "No entry found to delete." << endl;
		this_thread::sleep_for(chrono::seconds(2));
		cout << "Returning to the Main Menu page..." << endl;
		this_thread::sleep_for(chrono::seconds(3));
		system("cls");
		return;
	}
	
	int choice4;
	while (true) {
		system("cls");
		cout << "----- Delete Entry -----" << endl;
		for (size_t i = 0; i < entries.size(); i++) {
			cout << i + 1 << ". " << entries[i].AppName << endl;
		}
		cout << endl;
		cout << "Your choice: ";
		cin >> choice4;
		
		if (cin.fail()) {
			cin.clear();
			cin.ignore();
			system("cls");
			cout << "Invalid input." << endl;
			this_thread::sleep_for(chrono::seconds(2));
			cout << "You must enter a number." << endl;
			this_thread::sleep_for(chrono::seconds(2));
			cout << "Returning to the Main Menu page..." << endl;
			this_thread::sleep_for(chrono::seconds(3));
			system("cls");
			return;
		}
	
		cin.ignore();
		
		if (choice4 < 1 || choice4 > (int)entries.size()) {
			system("cls");
			cout << "Invalid choice." << endl;
			this_thread::sleep_for(chrono::seconds(2));
			cout << "Please try again." << endl;
			this_thread::sleep_for(chrono::seconds(2));
			cout << "Returning to the Delete page..." << endl;
			this_thread::sleep_for(chrono::seconds(3));
			system("cls");
			continue;
		}
		break;
	}
	entries.erase(entries.begin() + (choice4 - 1));
	
	ofstream outFile(filename, ios::binary | ios::trunc);
	if (!outFile) {
		system("cls");
		cout << "Error creating the file." << endl;
		this_thread::sleep_for(chrono::seconds(2));
		cout << "Exiting the program..." << endl;
		this_thread::sleep_for(chrono::seconds(3));
		exit(1);
	}
	
	for (const auto& e : entries) {
		size_t len;
		
		len = e.AppName.size();
		outFile.write(reinterpret_cast<const char*>(&len), sizeof(len));
		outFile.write(e.AppName.c_str(), len);
		
		len = e.Username.size();
		outFile.write(reinterpret_cast<const char*>(&len), sizeof(len));
		outFile.write(e.Username.c_str(), len);
		
		len = e.Email.size();
		outFile.write(reinterpret_cast<const char*>(&len), sizeof(len));
		outFile.write(e.Email.c_str(), len);
		
		len = e.Pass.size();
		outFile.write(reinterpret_cast<const char*>(&len), sizeof(len));
		outFile.write(e.Pass.c_str(), len);
	}
	outFile.close();
	system("cls");
	cout << "Entry Deleted Successfully!" << endl;
	this_thread::sleep_for(chrono::seconds(2));
	cout << "Returning to the Main Menu page..." << endl;
	this_thread::sleep_for(chrono::seconds(3));
	system("cls");
}

void ShowAllEntries();

void ShowAllEntries() {
	string path = getSavePath();
	string filename = path + "data.bin";
	
	ifstream inFile(filename, ios::binary);
	if (!inFile.is_open()) {
		system("cls");
		cout << "Cannot open or missing this file." << endl;
		this_thread::sleep_for(chrono::seconds(2));
		cout << "Returning to the Main Menu page..." << endl;
		this_thread::sleep_for(chrono::seconds(3));
		return;
	}
	
	system("cls");
	cout << "----- All Entries -----";
	cout << endl;
	vector<Entry> entries;
	
	while (true) {
		Entry e;
		size_t len;
		char* buffer;
		
		if (!inFile.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
		buffer = new char[len + 1];
		inFile.read(buffer, len);
		buffer[len] = '\0';
		e.AppName = buffer;
		delete[] buffer;
		
		if (!inFile.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
		buffer = new char[len + 1];
		inFile.read(buffer, len);
		buffer[len] = '\0';
		e.Username = buffer;
		delete[] buffer;
		
		if (!inFile.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
		buffer = new char[len + 1];
		inFile.read(buffer, len);
		buffer[len] = '\0';
		e.Email = buffer;
		delete[] buffer;
		
		if (!inFile.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
		buffer = new char[len + 1];
		inFile.read(buffer, len);
		buffer[len] = '\0';
		e.Pass = buffer;
		delete[] buffer;
		
		cout << endl;
		cout << "App Name: " << e.AppName << endl;
		cout << "Username: " << e.Username << endl;
		cout << "Email: " << e.Email << endl;
		cout << "Password: " << e.Pass << endl;
		cout << endl;
		cout << "--------------------------------------" << endl;
		
		entries.push_back(e);
	}
	
	inFile.close();
	
	if (entries.empty()) {
		system("cls");
		cout << "No entries found to show." << endl;
		this_thread::sleep_for(chrono::seconds(2));
		cout << "Returning to the Main Menu page..." << endl;
		this_thread::sleep_for(chrono::seconds(3));
		system("cls");
		return;
	} else {
		cout << endl;
		cout << "Press ENTER to return to the Main Menu page...";
		cin.get();
		system("cls");
	}
}

void BuckUp();

void BuckUp() {
	string path = getSavePath();
	string binFile = path + "data.bin";
	string txtFile = path + "data_backup.txt";
	
	ifstream inFile(binFile, ios::binary);
	if(!inFile) {
		system("cls");
		cout << "Cannot open or missing this file." << endl;
		this_thread::sleep_for(chrono::seconds(2));
		cout << "Returning to the Main Menu page..." << endl;
		this_thread::sleep_for(chrono::seconds(3));
		system("cls");
		return;
	}
	
	ofstream outFile(txtFile);
	if (!outFile) {
		system("cls");
		cout << "Error creating back up file." << endl;
		this_thread::sleep_for(chrono::seconds(2));
		cout << "Returning to the Main Menu page..." << endl;
		this_thread::sleep_for(chrono::seconds(3));
		system("cls");
		return;
	}
	
	vector<Entry> entries;
	int eCount = 0;
	
	while (true) {
		Entry e;
        size_t len;
        char* buffer;
		
		if (!inFile.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
        buffer = new char[len + 1];
        if (!inFile.read(buffer, len)) { 
			delete[] buffer; 
			break; 
		}
        buffer[len] = '\0';
        e.AppName = buffer;
        delete[] buffer;
		
		if (!inFile.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
        buffer = new char[len + 1];
        if (!inFile.read(buffer, len)) { 
			delete[] buffer; 
			break; 
		}
        buffer[len] = '\0';
        e.Username = buffer;
        delete[] buffer;
		
		if (!inFile.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
        buffer = new char[len + 1];
        if (!inFile.read(buffer, len)) { 
			delete[] buffer; 
			break; 
		}
        buffer[len] = '\0';
        e.Email = buffer;
        delete[] buffer;
		
		if (!inFile.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
        buffer = new char[len + 1];
        if (!inFile.read(buffer, len)) { 
			delete[] buffer; 
			break; 
		}
        buffer[len] = '\0';
        e.Pass = buffer;
        delete[] buffer;
		
		outFile << "-----------------------------------------" << endl;
		outFile << "App Name: " << e.AppName << endl;
		outFile << "Username: " << e.Username << endl;
		outFile << "Email: " << e.Email << endl;
		outFile << "Password: " << e.Pass << endl;
		outFile << "-----------------------------------------" << endl;
		outFile << endl;
		
		eCount++;
	}
	
	inFile.close();
	
	outFile.close();
	system("cls");
	if (eCount == 0) {
		cout << "No entry found to back up." << endl;
	} else {
		cout << "Back Up Succesfully to " << txtFile << " !" << endl;
	}
	
	this_thread::sleep_for(chrono::seconds(2));
	cout << "Returning to the Main Menu page..." << endl;
	this_thread::sleep_for(chrono::seconds(3));
	system("cls");
 }

void MainMenu(const string& filename, const string& email, const string& username) {
	int choice2;
	while (true) {
		system("cls");
		cout << "----- Main Menu of PassGuard -----" << endl;
		cout << "1. Add Entry" << endl;
		cout << "2. Edit Entry" << endl;
		cout << "3. Search Entry" << endl;
		cout << "4. Delete Entry" << endl;
		cout << "5. Show All Entries" << endl;
		cout << "6. Change Login Password" << endl;
		cout << "7. BackUp All Data" << endl;
		cout << "8. Log out and Exit Program" << endl;
		cout << endl;
		cout << "Please enter your choice: ";
		cin >> choice2;
		cin.ignore();
		system("cls");
		
		if (cin.fail()) {
			cin.clear();
			cin.ignore();
			cout << "Invalid input." << endl;
			this_thread::sleep_for(chrono::seconds(2));
			cout << "You must enter a number." << endl;
			this_thread::sleep_for(chrono::seconds(2));
			cout << "Returning to the Main Menu page..." << endl;
			this_thread::sleep_for(chrono::seconds(3));
			system("cls");
			continue;
		}
		
		switch (choice2) {
			case 1:
				AddEntry();
				break;
			case 2:
				EditEntry();
				break;
			case 3:
				SearchEntry();
				break;
			case 4:
				DeleteEntry();
				break; 
			case 5:
				ShowAllEntries();
				break;
			case 6:
				changePass(filename, email, username, true);
				break;
			case 7:
				BuckUp();
				break;
			case 8:
				cout << "Logging out and exiting program..." << endl;
				this_thread::sleep_for(chrono::seconds(3));
				exit(0);
			default:
				cout << "Invalid choice!" << endl;
				this_thread::sleep_for(chrono::seconds(2));
				cout << "Returning to the Main Menu page..." << endl;
				this_thread::sleep_for(chrono::seconds(3));
				system("cls");
				break;
		}
	}
}

int main(int argc, char** argv) {
	int choice;
	while (true) {
		system("cls");
		cout << "----- Welcome to PassGuard -----" << endl;
		cout << "1. Sign Up" << endl;
		cout << "2. Login" << endl;
		cout << "3. Exit" << endl;
		cout << endl;
		cout << "Please enter your choice: ";
		cin >> choice;
		system("cls");
	
		if (cin.fail()) {
			cin.clear();
			cin.ignore();
			cout << "Invalid input." << endl;
			this_thread::sleep_for(chrono::seconds(2));
			cout << "You must enter a number." << endl;
			this_thread::sleep_for(chrono::seconds(2));
			cout << "Returning to the Sign Up/Login page..." << endl;
			this_thread::sleep_for(chrono::seconds(3));
			system("cls");
			continue;
		}	
	
		switch (choice) {
			case 1:
				RegisterUser();
				break;
			case 2:
				LoginUser();
				break;
			case 3:
				cout << "Exiting program..." << endl;
				this_thread::sleep_for(chrono::seconds(3));
				exit(0);
			default:
				cout << "Invalid choice!" << endl;
				this_thread::sleep_for(chrono::seconds(2));
				cout << "Returning to the Sign Up/Login page..." << endl;
				this_thread::sleep_for(chrono::seconds(3));
				system("cls");
				break;
		}
	}
	return 0;
}
