//#include <iostream>
//
//using namespace std;
//
//class Fish
//{
//protected:
//	bool isFreshWaterFish;	// accessible only to derived classes
//public:
//	// Fish constructor
//	Fish(bool isFreshWater) : isFreshWaterFish(isFreshWater) {}
//
//	void Swim()
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
//	Tuna():Fish(false) {} // constructor initializes base
//};
//
//class Carp : public Fish
//{
//public:
//	Carp():Fish(true) {}
//};
//
//int main() {
//
//	Carp myLunch;
//	Tuna myDinner;
//
//	cout << "About my food:" << endl;
//
//	cout << "Lunch: ";
//	myLunch.Swim();
//
//	cout << "Dinner: ";
//	myDinner.Swim();
//
//	return 0;
//}