#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class TextFile {
public:
	TextFile(string);
	TextFile();
	TextFile(TextFile&);
	~TextFile();

	void open();
	void add_text(string);
	void save(string);
	void insert(string, int);
	const void print();
	string get_name();

	TextFile& operator= (const TextFile& file_to_copy);
	TextFile& operator= (string text);
	/*string& operator= (TextFile& file);*/
	TextFile& operator+= (string text);

	friend ostream& operator<< (ostream& out, const TextFile& file);
	friend istream& operator>> (istream& in, TextFile& file);

	friend void open_text(TextFile&, string);
	friend void close_text(TextFile&);
	friend int get_file_size(TextFile&);
private:
	string name;
	string buff;
	FILE* file;
};

TextFile::TextFile(string fname) :name(fname), file(0) {
}
TextFile::TextFile() : name("file.txt"), buff(""), file(0) {
}
TextFile::TextFile(TextFile& obj) {
	name = obj.name;
	buff = obj.buff;
	file = obj.file;
}
TextFile::~TextFile() {
	cout << ":)" << endl;
}

void TextFile::open() {
	open_text(*this, name);

	int size = get_file_size(*this);
	char* temp_buff = new char[size + 1]();
	while (fgets(temp_buff, 256, file) != 0) {
		buff.append(string(temp_buff));
	}
	delete temp_buff;

	close_text(*this);
}

void TextFile::add_text(string str) {
	open_text(*this, name);
	buff.append(str);
	char* temp_buff = new char[buff.length() + 1]();
	strcpy_s(temp_buff, buff.length() + 1, buff.c_str());
	fputs(temp_buff, file);
	delete temp_buff;
	close_text(*this);
}

void TextFile::save(string newname) {
	if (newname == name) {
		open_text(*this, name);
	}
	else {
		rename(name.c_str(), newname.c_str());
		open_text(*this, newname);
	}
	close_text(*this);
}

void TextFile::insert(string str, int n) {
	open_text(*this, name);
	buff.insert(n, str);
	char* temp_buff = new char[buff.length() + 1]();
	strcpy_s(temp_buff, buff.length() + 1, buff.c_str());
	fputs(temp_buff, file);
	delete temp_buff;
	close_text(*this);
}

const void TextFile::print() {
	cout << "\tFile name: " << name << endl;
	cout << buff << endl;
}

string TextFile::get_name() {
	return name;
}


TextFile& TextFile::operator= (const TextFile& file_to_copy) {
	if (this == &file_to_copy) {
		return *this;
	}

	file = file_to_copy.file;
	name = file_to_copy.name;
	buff = file_to_copy.buff;

	return *this;
}

TextFile& TextFile::operator= (string text) {
	open_text(*this, name);
	buff = text;
	char* temp_buff = new char[buff.length() + 1]();
	strcpy_s(temp_buff, buff.length() + 1, buff.c_str());
	fputs(temp_buff, file);
	delete temp_buff;
	close_text(*this);

	return *this;
}

//string& TextFile::operator= (TextFile& file) {
//	string text = file.buff;
//	return text;
//}

TextFile& TextFile::operator+= (string text) {
	this->add_text(text);
	return *this;
}

ostream& operator<< (ostream& out, const TextFile& file) {
	out << "\tFile name: " << file.name << endl << file.buff << endl;
	return out;
}

istream& operator>> (istream& in, TextFile& file) {
	in >> file.name;
	in >> file.buff;

	return in;
}

void open_text(TextFile& file, string name) {
	errno_t err = fopen_s(&file.file, name.c_str(), "r+");
	if (err != 0) {
		cout << "Such file doesn't exist, creating a new file...\n";
		err = fopen_s(&file.file, name.c_str(), "w+");
		if (err == 0) {
			cout << "The file was created and opened\n";
		}
		else {
			cout << "Error: the file was not opened or created\n";
			exit(1);
		}
	}
}

void close_text(TextFile& file) {
	if (file.file) {
		errno_t err = fclose(file.file);
		if (err != 0) {
			cout << "Error: the file was not closed" << endl;
			exit(2);
		}
	}
	else {
		cout << "Error with the file" << endl;
		exit(3);
	}
}

int get_file_size(TextFile& file) {
	if (fseek(file.file, 0, SEEK_END) != 0) {
		cout << "Error while getting file size" << endl;
		exit(4);
	}

	long size = ftell(file.file);

	if (size == -1L) {
		cout << "Error while getting file size" << endl;
		exit(5);
	}

	rewind(file.file);

	return size;
}


int main() {
	TextFile file;
	cin >> file;

	TextFile file2("test2.txt");

	file2 = file;
	cout << file2;
	cout << "------------------------" << endl;

	file2 = "helllaaaaa";
	cout << file2;
	cout << "------------------------" << endl;
	//string text = file2;
	file2 += "amigosssss";
	cout << file2;
	file2 += ":(";
	cout << file2;

	//comment from malish 2/0
	//lololololo
}