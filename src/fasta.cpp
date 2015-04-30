#include "fasta.h"

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include <fstream>
#include <sstream>


namespace fs = boost::filesystem;
namespace {
  std::map<char, std::string> ptm_code_map = {{'N', "ptm_phosph0"},
                                              {'O', "ptm_phosph1"},
                                              {'P', "ptm_phosph2"},
                                              {'Q', "ptm_phosph3"},
                                              {'B', "ptm_acet0"},
                                              {'C', "ptm_acet1"},
                                              {'D', "ptm_acet2"},
                                              {'E', "ptm_acet3"},
                                              {'F', "ptm_Nglyc0"},
                                              {'G', "ptm_Nglyc1"},
                                              {'H', "ptm_Nglyc2"},
                                              {'I', "ptm_Nglyc3"},
                                              {'J', "ptm_amid0"},
                                              {'K', "ptm_amid1"},
                                              {'L', "ptm_amid2"},
                                              {'M', "ptm_amid3"},
                                              {'R', "ptm_hydroxy0"},
                                              {'S', "ptm_hydroxy1"},
                                              {'T', "ptm_hydroxy2"},
                                              {'U', "ptm_hydroxy3"},
                                              {'V', "ptm_methyl0"},
                                              {'W', "ptm_methyl1"},
                                              {'X', "ptm_methyl2"},
                                              {'Y', "ptm_methyl3"},
                                              {'Z', "ptm_Oglyc0"},
                                              {'a', "ptm_Oglyc1"},
                                              {'b', "ptm_Oglyc2"},
                                              {'c', "ptm_Oglyc3"},
                                              {'d', "ptm_phosphP"}};
}


fasta::FastaData fasta::parse_fasta(std::string filename, int codon_length,
    bool refine) {
  fs::path p(filename);
  if (!fs::exists(p)) {
    throw std::invalid_argument("File not found: " + filename);
  }

  std::ifstream fastafile(filename.c_str());
  std::string line;
  bool in_sequence_section = true;
  fasta::FastaData fd;
  while (std::getline(fastafile, line)) {
    if (line == std::string("## PROBABILITIES")) {
      in_sequence_section = false;
      continue;
    }

    if (in_sequence_section) {
      assert(line.substr(0, 1) == ">");

      std::string description = line;
      std::getline(fastafile, line);
      line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
      fd.sequences.push_back(fasta::make_sequence(description, line,
                                                  codon_length));
    } else {
      std::vector<std::string> result;
      boost::split(result, line, boost::is_any_of("\t "));

      if (result.size() != 2) {
        throw std::runtime_error("Invalid probability format: " + line);
      }
      fd.probabilities["motif_" + result[0]] = std::stod(result[1]);
    }
  }
  fastafile.close();
  if (refine && !check_length(fd.sequences)) {
        throw std::runtime_error("In the 'refine' mode all sequences should "
                                 "have the same length");
  }

  return fd;
}


fasta::Sequence fasta::make_sequence(const std::string& description,
                                     const std::string& codons,
                                     int codon_length)
{
  fasta::Sequence s;
  s.description = description;
  
  for (unsigned i = 0; i < codons.size(); i += codon_length) {
    boost::regex re("[a-zA-Z0-9-]{" + std::to_string(codon_length) + "}");
    std::string codon = codons.substr(i, codon_length);
    if (!boost::regex_match(codon, re)) {
      throw std::runtime_error("Invalid codon: " + codon);
    }
    fasta::Residue r = fasta::make_residue(codon);
    s.residues.push_back(r);
  }
  return s;
}

fasta::Sequence fasta::make_sequence(const std::vector<Residue>& residue_list)
{
  fasta::Sequence s;
  s.residues = residue_list;
  return s;
}

fasta::Sequence fasta::make_sequence(unsigned long sequence_length,
                                     const fasta::Residue residue) {
  fasta::Sequence s;
  s.residues = std::vector<fasta::Residue>(sequence_length, residue);
  return s;
}

fasta::Sequence fasta::make_sequence(
    const std::string sequence_string,
    int codon_length) {
  fasta::Sequence s;
  for (size_t i = 0; i < sequence_string.size(); ++i) {
    if (i % codon_length == 0) {
      std::string codon = sequence_string.substr(i, codon_length);
      fasta::Residue res = fasta::make_residue(codon);
      s.residues.push_back(res);
    }
  }
  return s;
}

std::string fasta::make_string(const fasta::Sequence seq) {
  std::string result;
  for (auto& residue : seq.residues) {
    result.push_back(residue.codon[0]);
  }
  return result;
}

fasta::Residue fasta::make_residue(const std::string& codon) {
  std::vector<std::string> features;
  if (codon.size() >= 5 && codon[4] != 'A') {
    features.push_back(ptm_code_map.at(codon[4]));
  }
  if (codon.size() >= 7 && codon.substr(5, 2) != "AA") {
    features.push_back("motif_" + codon.substr(5, 2));
  }
  if (codon.size() >= 4 && codon.substr(2, 2) != "AA") {
    features.push_back("domain_" + codon.substr(2, 2));
  }
  return Residue(codon, features);
}

bool fasta::check_length(fasta::SequenceList sequences) {
  bool result = true;
  size_t prev_length = sequences[0].residues.size();
  size_t i = 1;
  while (result && i < sequences.size()){
    size_t length = sequences[i].residues.size();
    if (length != prev_length) {
      result = false; 
    }
    ++i;
  }
  return result;
}
