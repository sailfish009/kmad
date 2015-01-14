#include "substitution_matrix.h"

#include <algorithm>
#include <iostream>


typedef std::vector<std::vector<int>> SbstMatrix;
typedef std::map<char, std::vector<double>> SimilarityScoresMap;

namespace {
  static const std::vector<char> Alphabet = {'A','R','N','D','C','Q','E','G',
                                             'H','I','L','K','M','F','P','S',
                                             'T','W','Y','V'};
  //BLOSUM62
  //TODO: make it a map
  static const SbstMatrix SimScores = {
    { 4, -1, -2, -2,  0, -1, -1,  0, -2, -1, -1, -1, -1, -2, -1, 1, 0, -3, -2, 0},
    {-1,  5,  0, -2, -3,  1,  0, -2, 0, -3, -2, 2, -1, -3, -2, -1, -1, -3, -2, -3},
    {-2,  0,  6,  1, -3,  0,  0,  0, 1, -3, -3, 0, -2, -3, -2, 1, 0, -4, -2, -3},
    {-2, -2,  1,  6, -3,  0,  2, -1, -1, -3, -4, -1, -3, -3, -1, 0, -1, -4, -3, -3},
    { 0, -3, -3, -3,  9, -3, -4, -3, -3, -1, -1, -3, -1, -2, -3, -1, -1, -2, -2, -1},
    {-1,  1,  0,  0, -3,  5,  2, -2, 0, -3, -2, 1, 0, -3, -1, 0, -1, -2, -1, -2},
    {-1,  0,  0,  2, -4,  2,  5, -2, 0, -3, -3, 1, -2, -3, -1, 0, -1, -3, -2, -2},
    { 0, -2,  0, -1, -3, -2, -2,  6, -2, -4, -4, -2, -3, -3, -2, 0, -2, -2, -3, -3},
    {-2,  0,  1, -1, -3,  0,  0, -2, 8, -3, -3, -1, -2, -1, -2, -1, -2, -2, 2, -3},
    {-1, -3, -3, -3, -1, -3, -3, -4, -3, 4, 2, -3, 1, 0, -3, -2, -1, -3, -1, 3},
    {-1, -2, -3, -4, -1, -2, -3, -4, -3, 2, 4, -2, 2, 0, -3, -2, -1, -2, -1, 1},
    {-1,  2,  0, -1, -3,  1,  1, -2, -1, -3, -2, 5, -1, -3, -1, 0, -1, -3, -2, -2},
    {-1, -1, -2, -3, -1,  0, -2, -3, -2, 1, 2, -1, 5, 0, -2, -1, -1, -1, -1, 1},
    {-2, -3, -3, -3, -2, -3, -3, -3, -1, 0, 0, -3, 0, 6, -4, -2, -2, 1, 3, -1},
    {-1, -2, -2, -1, -3, -1, -1, -2, -2, -3, -3, -1, -2, -4, 7, -1, -1, -4, -3, -2},
    { 1, -1,  1,  0, -1,  0,  0,  0, -1, -2, -2, 0, -1, -2, -1, 4, 1, -3, -2, -2},
    { 0, -1,  0, -1, -1, -1, -1, -2, -2, -1, -1, -1, -1, -2, -1, 1, 5, -2, -2, 0},
    {-3, -3, -4, -4, -2, -2, -3, -2, -2, -3, -2, -3, -1, 1, -4, -3, -2, 11, 2, -3},
    {-2, -2, -2, -3, -2, -1, -2, -3, 2, -1, -1, -2, -1, 3, -3, -2, -2, 2, 7, -1},
    { 0, -3, -3, -3, -1, -2, -2, -3, -3, 3, 1, -2, 1, -1, -2, -2, 0, -3, -1, 4}};
}


std::vector<double> substitution_matrix::get_column(const char& aa) {
  int column_no = std::find(Alphabet.begin(), Alphabet.end(), aa)
    - Alphabet.begin();
  std::vector<double> column;
  for (auto& item : SimScores[column_no]) {
    column.push_back(double(item));
  }
  return column;
}
