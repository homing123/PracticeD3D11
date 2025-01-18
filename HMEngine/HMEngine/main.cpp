#include "App.h"
#include <iostream>

using namespace std;
int main() {
	App app;
	App::pIns = &app;
	if (app.Init() == false)
	{
		cout << "App Init failed" << endl;
		return -1;
	}

	return app.Run();
}