//============================================================================
// Name        : krw55_MachineLearningLinearRegression.cpp
// Author      : Kevin Wright
// NetID	   : krw55
// Course	   : CS4378V.251
// Assignment  : Homework 2: Problem 3
// Description : Finds the expected MPG of a car using a Linear Regression model
//============================================================================

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>

using namespace std;

class Regression{
public:
	struct node{
		double mpg; //miles per gallon
		double cylinders;
		double displ; //displacement
		double horse; //horsePower
		double weight;
		double accel; //acceleration
	};

	bool init();
	void runRegress();

private:

	node points[398]; // array of the sample data
	node testPoints[3]; // array of the points to be tested

	double maxMpg, maxCyl, maxDispl, maxHorse, maxWeight, maxAccel; // max values for normalization
	double minMpg, minCyl, minDispl, minHorse, minWeight, minAccel; // min Values for normalization

};

bool Regression::init(){

	char next; // to read in the '?' from the input file

	//setting the min / max values to guarantee assignment on the first loop.
	maxMpg = -1; maxCyl = -1; maxDispl = -1; maxHorse = -1; maxWeight = -1; maxAccel = -1;
	minMpg = 9999999; minCyl = 9999999; minDispl = 9999999; minHorse = 9999999; minWeight = 9999999; minAccel = 9999999;

	ifstream input;
	input.open("linear_regression_data.txt");
	if (input.is_open()){ // check for the input file
		for (int x = 0; x < 398; x++){ //loop to import the data and find the min / max values for each variable.
			input >> points[x].mpg;
			if (points[x].mpg > maxMpg)
				maxMpg = points[x].mpg;
			if (points[x].mpg < minMpg)
				minMpg = points[x].mpg;

			input >> points[x].cylinders;
			if (points[x].cylinders > maxCyl)
				maxCyl = points[x].cylinders;
			if (points[x].cylinders < minCyl)
				minCyl = points[x].cylinders;

			input >> points[x].displ;
			if (points[x].displ > maxDispl)
				maxDispl = points[x].displ;
			if (points[x].displ < minDispl)
				minDispl = points[x].displ;

			input >> points[x].horse;
			if (points[x].horse > maxHorse)
				maxHorse = points[x].horse;
			if (points[x].horse < minHorse)
				minHorse = points[x].horse;

			input >> points[x].weight;
			if (points[x].weight > maxWeight)
				maxWeight = points[x].weight;
			if (points[x].weight < minWeight)
				minWeight = points[x].weight;

			input >> points[x].accel;
			if (points[x].accel > maxAccel)
				maxAccel = points[x].accel;
			if (points[x].accel < minAccel)
				minAccel = points[x].accel;
		}

		for (int x = 0; x < 3; x++){	//read the last 3 values, the ones to be tested.
			input >> next; // read in placeholder '?'
			testPoints[x].mpg = -1;

			input >> testPoints[x].cylinders;
			if (testPoints[x].cylinders > maxCyl)
				maxCyl = testPoints[x].cylinders;
			if (testPoints[x].cylinders < minCyl)
				minCyl = testPoints[x].cylinders;

			input >> testPoints[x].displ;
			if (testPoints[x].displ > maxDispl)
				maxDispl = testPoints[x].displ;
			if (testPoints[x].displ < minDispl)
				minDispl = testPoints[x].displ;

			input >> testPoints[x].horse;
			if (testPoints[x].horse > maxHorse)
				maxHorse = testPoints[x].horse;
			if (testPoints[x].horse < minHorse)
				minHorse = testPoints[x].horse;

			input >> testPoints[x].weight;
			if (testPoints[x].weight > maxWeight)
				maxWeight = testPoints[x].weight;
			if (testPoints[x].weight < minWeight)
				minWeight = testPoints[x].weight;

			input >> testPoints[x].accel;
			if (testPoints[x].accel > maxAccel)
				maxAccel = testPoints[x].accel;
			if (testPoints[x].accel < minAccel)
				minAccel = testPoints[x].accel;
		}

		for (int x = 0; x < 398; x++){ //normalizes the samples
			points[x].mpg = (points[x].mpg - minMpg) / (maxMpg - minMpg);
			points[x].cylinders = (points[x].cylinders - minCyl) / (maxCyl - minCyl);
			points[x].displ = (points[x].displ - minDispl) / (maxDispl - minDispl);
			points[x].horse = (points[x].horse - minHorse) / (maxHorse - minHorse);
			points[x].weight = (points[x].weight - minWeight) / (maxWeight - minWeight);
			points[x].accel = (points[x].accel - minAccel) / (maxAccel - minAccel);
		}

		for (int x = 0; x < 3; x++){	//normalizes the known values for the nodes to be tested
			testPoints[x].cylinders = testPoints[x].cylinders / maxCyl;
			testPoints[x].displ = testPoints[x].displ / maxDispl;
			testPoints[x].horse = testPoints[x].horse / maxHorse;
			testPoints[x].weight = testPoints[x].weight / maxWeight;
			testPoints[x].accel = testPoints[x].accel / maxAccel;
		}
		return true; // file was found and the work was done.
	}
	else
		return false; // file wasn't found
}

void Regression::runRegress(){
	double cost = 0;// difference between estimated mpg and the actual sample.
	double summation;
	double step = .0001; // the alpha value from the equation.

	double intercept = 0, cylinderW = 1, distplW = 1, horseW = 1, weightW = 1, accelW = 1; // starting values to the Theta weights
	double oldCylW = -1, oldDistW = -1, oldHorseW = -1, oldWeighW = -1, oldAccelW = -1; //old weights from the last loop
	double oldCost;
	double estMpg; // estimated MPG


	for (int x = 0; x < 398; x++){ //finding the cost of the current line
		cost = cost + pow(((intercept + cylinderW * points[x].cylinders + distplW * points[x].displ + horseW * points[x].horse + weightW
				* points[x].weight + accelW * points[x].accel) - points[x].mpg),2);
	}
	cost = cost / 2;

	//
	cout << "starting cost: " << cost << endl;

	while (cost != oldCost){	// loop until further loops accomplish nothing

		oldCost = cost;
		oldCylW = cylinderW; //assign old steps values to the 'old' variables.
		oldDistW = distplW;
		oldHorseW = horseW;
		oldWeighW = weightW;
		oldAccelW = accelW;


		summation = 0;
		for (int y = 0; y < 398; y++){
			summation = summation + ((points[y].mpg - ( intercept + oldCylW * points[y].cylinders + oldDistW * points[y].displ + oldHorseW * points[y].horse + oldWeighW
					* points[y].weight + oldAccelW * points[y].accel)));
		}
		intercept = intercept + step * summation;

		summation = 0;
		for (int y = 0; y < 398; y++){
			summation = summation + ((points[y].mpg - ( intercept + oldCylW * points[y].cylinders + oldDistW * points[y].displ + oldHorseW * points[y].horse + oldWeighW
					* points[y].weight + oldAccelW * points[y].accel)) * points[y].cylinders);
		}
		cylinderW = oldCylW + step * summation;

		summation = 0;
		for (int y = 0; y < 398; y++){
			summation = summation + ((points[y].mpg - (intercept + oldCylW * points[y].cylinders + oldDistW * points[y].displ + oldHorseW * points[y].horse + oldWeighW
					* points[y].weight + oldAccelW * points[y].accel)) * points[y].displ);
		}
		distplW = oldDistW + step * summation;

		summation = 0;
		for (int y = 0; y < 398; y++){
			summation = summation + ((points[y].mpg - (intercept + oldCylW * points[y].cylinders + oldDistW * points[y].displ + oldHorseW * points[y].horse + oldWeighW
					* points[y].weight + oldAccelW * points[y].accel)) * points[y].horse);
		}
		horseW = oldHorseW + step * summation;

		summation = 0;
		for (int y = 0; y < 398; y++){
			summation = summation + ((points[y].mpg - (intercept + oldCylW * points[y].cylinders + oldDistW * points[y].displ + oldHorseW * points[y].horse + oldWeighW
					* points[y].weight + oldAccelW * points[y].accel)) * points[y].weight);
		}
		weightW = oldWeighW + step * summation;

		summation = 0;
		for (int y = 0; y < 398; y++){
			summation = summation + ((points[y].mpg - (intercept + oldCylW * points[y].cylinders + oldDistW * points[y].displ + oldHorseW * points[y].horse + oldWeighW
					* points[y].weight + oldAccelW * points[y].accel)) * points[y].accel);
		}
		accelW = oldAccelW + step * summation;

		summation = 0;
		cost = 0; // finding the new cost
		for (int x = 0; x < 398; x++){
			cost = cost + pow(((intercept + cylinderW * points[x].cylinders + distplW * points[x].displ + horseW * points[x].horse + weightW
					* points[x].weight + accelW * points[x].accel) - points[x].mpg),2);
		}
		cost = cost / 2;
		cout << "new Cost: "  << cost << endl;
	}

	for (int x = 0; x < 3; x++){ // finds the estimated MPG of the test points and outputs it to the console.
		if (x == 0)
			cout << "Weights found: Intercept: "<< intercept <<" Cylinder: " << cylinderW << " Displacement: " << distplW << " HorsePower: " << horseW <<
			" Weight: " << weightW <<" Acceleration: " << accelW << "\n";
		estMpg = intercept + cylinderW * testPoints[x].cylinders + distplW * testPoints[x].displ + horseW * testPoints[x].horse + weightW
				* testPoints[x].weight + accelW * testPoints[x].accel;
		estMpg = estMpg * (maxMpg - minMpg); //reversing the normalization to get an actual number.
		estMpg = estMpg + minMpg;
		cout << "for test sample " << x << " the estimated MPG is " << estMpg << endl;
	}
}

int main(){
	bool initSuccess = false;
	Regression reg;
	initSuccess = reg.init(); //returns false if unable to find the input file
	if (initSuccess){
		reg.runRegress();	//runs the body of the program
	}
	else{
		cout << "Faled to open File. Please make sure 'linear_regression_data.txt' can be found in the same folder as this program.\n";
	}
	return 0;
}
