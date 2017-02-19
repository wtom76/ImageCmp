#pragma once
#include <string>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
using namespace std;
using namespace cv;

namespace ObjectTracking
{
	class Object
	{
	public:
		Object();
		~Object(void);

		Object(string name);

		int getXPos();
		void setXPos(int x);

		int getYPos();
		void setYPos(int y);

		int area() const { return area_; }
		void setArea(int val) { area_ = val; }

		Scalar getHSVmin();
		Scalar getHSVmax();

		void setHSVmin(Scalar min);
		void setHSVmax(Scalar max);

		string getType(){return type;}
		void setType(string t){type = t;}

		Scalar getColor(){
			return Color;
		}
		void setColor(Scalar c){

			Color = c;
		}

	private:

		int xPos;
		int yPos;
		int area_;
		string type;
		Scalar HSVmin, HSVmax;
		Scalar Color;
	};

	int run();
	void runImage();
}