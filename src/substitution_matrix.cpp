#include "substitution_matrix.h"
#include "residue.h"
#include "misc.h"
#include "vec_util.h"

#include<boost/range/numeric.hpp>
#include <iostream>
#include <vector>
typedef std::vector<char> AlphaList;
typedef std::vector<SbstMatColumn> SbstMatColumnsList;
 
namespace {
	static const AlphabetVec Alphabet = {'A','R','N','D','C','Q','E','G',
                                       'H','I','L','K','M','F','P','S',
                                       'T','W','Y','V'};
	//BLOSUM62
	static const SbstMatrix SimScores = {{4, -1, -2, -2, 0, -1, -1, 0, -2, -1, 
                                         -1, -1, -1, -2, -1, 1, 0, -3, -2, 0},
                                        {-1, 5, 0, -2, -3, 1, 0, -2, 0, -3, -2,
                                          2, -1, -3, -2, -1, -1, -3, -2, -3},
                                        {-2, 0, 6, 1, -3, 0, 0, 0, 1, -3, -3, 
                                          0, -2, -3, -2, 1, 0, -4, -2, -3},
                                        {-2, -2, 1, 6, -3, 0, 2, -1, -1, -3, 
                                         -4, -1, -3, -3, -1, 0, -1, -4, -3, -3},
                                        {0, -3, -3, -3, 9, -3, -4, -3, -3, -1, 
                                         -1, -3, -1, -2, -3, -1, -1, -2, -2, -1},
                                        {-1, 1, 0, 0, -3, 5, 2, -2, 0, -3, -2, 
                                          1, 0, -3, -1, 0, -1, -2, -1, -2}, 
                                        {-1, 0, 0, 2, -4, 2, 5, -2, 0, -3, -3, 
                                          1, -2, -3, -1, 0, -1, -3, -2, -2}, 
                                        {0, -2, 0, -1, -3, -2, -2, 6, -2, -4, 
                                         -4, -2, -3, -3, -2, 0, -2, -2, -3, -3}, 
                                        {-2, 0, 1, -1, -3, 0, 0, -2, 8, -3, -3, 
                                          -1, -2, -1, -2, -1, -2, -2, 2, -3}, 
                                        {-1, -3, -3, -3, -1, -3, -3, -4, -3, 4, 
                                          2, -3, 1, 0, -3, -2, -1, -3, -1, 3}, 
                                        {-1, -2, -3, -4, -1, -2, -3, -4, -3, 2, 
                                          4, -2, 2, 0, -3, -2, -1, -2, -1, 1}, 
                                        {-1, 2, 0, -1, -3, 1, 1, -2, -1, -3, 
                                          -2, 5, -1, -3, -1, 0, -1, -3, -2, -2}, 
                                        {-1, -1, -2, -3, -1, 0, -2, -3, -2, 1, 
                                          2, -1, 5, 0, -2, -1, -1, -1, -1, 1}, 
                                        {-2, -3, -3, -3, -2, -3, -3, -3, -1, 0, 
                                          0, -3, 0, 6, -4, -2, -2, 1, 3, -1}, 
                                        {-1, -2, -2, -1, -3, -1, -1, -2, -2, 
                                         -3, -3, -1, -2, -4, 7, -1, -1, -4, -3, -2}, 
                                        {1, -1, 1, 0, -1, 0, 0, 0, -1, -2, -2, 
                                          0, -1, -2, -1, 4, 1, -3, -2, -2}, 
                                        {0, -1, 0, -1, -1, -1, -1, -2, -2, -1, 
                                          -1, -1, -1, -2, -1, 1, 5, -2, -2, 0}, 
                                        {-3, -3, -4, -4, -2, -2, -3, -2, -2, 
                                         -3, -2, -3, -1, 1, -4, -3, -2, 11, 2, -3}, 
                                        {-2, -2, -2, -3, -2, -1, -2, -3, 2, -1, 
                                          -1, -2, -1, 3, -3, -2, -2, 2, 7, -1}, 
                                        {0, -3, -3, -3, -1, -2, -2, -3, -3, 3, 
                                          1, -2, 1, -1, -2, -2, 0, -3, -1, 4}};
}


int substitution_matrix::get_element(char char1, char char2){
	int index1=0;
	int index2=0;
  for (unsigned int i = 0; i < Alphabet.size(); i++){
		if(Alphabet[i] == char1){
			index1 = i;
		}
		if(Alphabet[i] == char2){
			index2 = i;
		}
	}
	return SimScores[index1][index2];
}


ProfileMatrix substitution_matrix::ConvertToProfileFormat(ResidueSequence& seq){
	ProfileMatrix result(seq.size());
	SbstMatColumnsList new_sbst_row;
	for (unsigned int i = 0; i < result.size(); i++){
		if (seq[i].get_aa() == 'B'){
			new_sbst_row.clear();
			new_sbst_row.push_back(SimScores[2]);
			new_sbst_row.push_back(SimScores[3]);
			result[i] = vec_util::Average(new_sbst_row);
		}
		else if (seq[i].get_aa() == 'Z'){
			new_sbst_row.clear();
			new_sbst_row.push_back(SimScores[6]);
			new_sbst_row.push_back(SimScores[7]);
			result[i] = vec_util::Average(new_sbst_row);

		}
		else if (seq[i].get_aa() == 'X'){
			result[i] = vec_util::Average(SimScores);
		}
		else{
			int aAcidInt = FindAminoAcidsIndex(seq[i].get_aa());
      SbstMatColumn sbst_column_int = SimScores[aAcidInt];
			result[i] = vec_util::ConvertIntVectorToDoubleVector(sbst_column_int);//adds a column to the result(converted from int to double)
		}
	}
	vec_util::TransposeVec(result);
	return result;
}


int substitution_matrix::get_element(int i,int j){
	return SimScores[i][j];
}


void substitution_matrix::get_column(unsigned int& column_no, 
                                     SbstMatColumn& column_int){
	column_int = SimScores[column_no];
}


int substitution_matrix::FindAminoAcidsIndex(char aa){
	int aacid_index = -1;
	for (unsigned int i = 0; i < Alphabet.size();i++){
		if (aa == Alphabet[i]){
			aacid_index = i;
			break;
		}
	}
	return aacid_index;
}
