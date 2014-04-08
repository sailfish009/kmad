#include <iostream>
#include <string>
#include <vector>
class Profile{
public:
	Profile(std::vector< std::vector<double> >); //constructor
	Profile();
	void buildPseudoProfile(std::vector< std::vector< std::string > >&, const std::vector<bool>&, const std::vector<double>&, bool);
	//getters//
	std::string getConsensusSequence();
	std::vector< std::vector<double> > getMatrix() const;
	void setMatrix(std::vector<std::vector<double> >);
	double getElement(int, char);
	double getElement(int, int);
	void printProfile(int,int);
	void printProfile();
private:
	//functions
	void createProfile(std::vector< std::vector<std::string> >&,const std::vector<bool>&, const std::vector<double>&, bool);
	void countOccurences(std::vector< std::vector<double> >&,std::vector< std::vector<std::string> >&,const std::vector<bool>&);
	void countOccurences(std::vector< std::vector<double> >&,std::vector< std::vector<std::string> >&,const std::vector<bool>&,const std::vector<double>&);
	double countNonGaps(int);
	int getMaxDoubleValue(std::vector<double>);
	//variables
	std::vector< std::vector<double> > prfMatrix;
};
