#include <glm\glm.hpp>
#include <glm\gtc\constants.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <glm\gtx\projection.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace glm;

int main()
{
	auto temp = vec2(vec4(1, 2, 3, 4));
	if(temp == vec2(1,2))
	{
		cout << "Worked" << endl;
	}

	getchar();
}