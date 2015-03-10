#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main

#include "fasta.h"

#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>


BOOST_AUTO_TEST_SUITE(test_fasta)


BOOST_AUTO_TEST_CASE(test_parse_fasta)
{
  fasta::FastaData fd = fasta::parse_fasta("tests/TAU_SPECI.fasta.7c", 7);

  BOOST_CHECK_EQUAL(fd.sequences.size(), 19);
  BOOST_CHECK_EQUAL(fd.probabilities.size(), 97);

  BOOST_CHECK_THROW(fasta::parse_fasta("tests/nonexistent.fasta.7c", 7),
                    std::invalid_argument);
  BOOST_CHECK_THROW(fasta::parse_fasta("tests/wrong_probs_format.fasta.7c", 7),
                    std::runtime_error);
  BOOST_CHECK_THROW(fasta::parse_fasta("tests/invalid_codon.fasta.7c", 7),
                    std::runtime_error);


}


BOOST_AUTO_TEST_CASE(test_make_sequence)
{
  fasta::Sequence s = fasta::make_sequence("desc", "AaaaaaBbbbbbCccccc", 6);

  BOOST_CHECK_EQUAL(s.residues.size(), 3);
  s = fasta::make_sequence("ALSK", 1);
  BOOST_CHECK_EQUAL(s.residues.size(), 4);
  s = fasta::make_sequence("LAAACAAA", 4);
  BOOST_CHECK_EQUAL(s.residues.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_make_string)
{
  fasta::Sequence s = fasta::make_sequence("LSKAL", 1);
  std::string string_seq = fasta::make_string(s);
  std::string expected = "LSKAL";
  BOOST_CHECK_EQUAL(string_seq, expected);
}
BOOST_AUTO_TEST_SUITE_END()
