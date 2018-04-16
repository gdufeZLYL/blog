//#include <iostream>
//
//using namespace std;
//
//class Motor
//{
//public:
//	void SwitchIgnition()
//	{
//		cout << "Ignition ON" << endl;
//	}
//	void PumpFuel()
//	{
//		cout << "Fuel in cylinders" << endl;
//	}
//	void FireCylinders()
//	{
//		cout << "Vroooom" << endl;
//	}
//};
//
//class Car :private Motor // private inheritance
//{
//public:
//	void Move()
//	{
//		SwitchIgnition();
//		PumpFuel();
//		FireCylinders();
//	}
//};
//
//int main() {
//	Car myDreamCar;
//	myDreamCar.Move();
//	//myDreamCar.PumpFuel(); // cannot access base's public member
//
//	return 0;
//}