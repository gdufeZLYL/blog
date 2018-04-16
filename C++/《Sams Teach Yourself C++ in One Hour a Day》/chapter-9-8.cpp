//#pragma warning(disable:4996)
//#include <iostream>
//#include <cstring>
//
//using namespace std;
//
//class MyString
//{
//private:
//	char* buffer;
//	//int num;
//public:
//	MyString(const char* initString) // constructor
//	{
//		buffer = NULL;
//		//num = 2;
//		if (initString != NULL)
//		{
//			buffer = new char[strlen(initString) + 1];
//			strcpy(buffer, initString);
//		}
//	}
//
//	~MyString()
//	{
//		cout << "Invoking destructor, clearing up" << endl;
//		delete[] buffer;
//	}
//
//	int GetLength()
//	{
//		//cout << "test: " << &num << endl;
//		cout << static_cast<const void *>(buffer) << endl;
//		return strlen(buffer);
//	}
//
//	const char* GetString()
//	{
//		return buffer;
//	}
//};
//
//void UseMyString(MyString str)
//{
//	//cout << &str << endl;
//	cout << "String buffer in MyString is " << str.GetLength();
//	cout << " characters long" << endl;
//
//	cout << "buffer contains: " << str.GetString() << endl;
//	return;
//}
//
//int main() {
//	MyString sayHello("Hello from String Class");
//	//cout << &sayHello << endl;
//	sayHello.GetLength();
//	UseMyString(sayHello);
//	//MyString sayHello(NULL);
//
//	/*int* num = new int;
//	int* num2 = num;
//	delete num;
//	delete num2;*/
//	return 0;
//}
//
