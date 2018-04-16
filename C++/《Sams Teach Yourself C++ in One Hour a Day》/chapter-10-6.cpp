//#include <iostream>
//
//using namespace std;
//
//class Fish
//{
//public:
//	void Swim()
//	{
//		cout << "Fish swims... !" << endl;
//	}
//	void Swim(bool isFreshWaterFish) // overloaded version
//	{
//		if (isFreshWaterFish)
//			cout << "Swims in lake" << endl;
//		else
//			cout << "Swims in sea" << endl;
//	}
//};
//
//class Tuna : public Fish
//{
//public:
//	void Swim()
//	{
//		cout << "Tuna swims real fast" << endl;
//	}
//};
//
//int main() {
//
//	Tuna myDinner;
//	cout << "About my food" << endl;
//
//	// myDinner.Swim(false);//failure: Tuna::Swim() hides Fish::Swim(bool)
//	myDinner.Swim();
//
//	return 0;
//}