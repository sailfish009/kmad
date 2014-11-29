#include "msa.h"
#include "txtProc.h"
#include "FeaturesProfile.h"
#include "Profile.h"
#include "Sequences.h"

#include <iostream>
#include <vector>
#include <tuple>
string_sequences msa::run_msa(Sequences sequences,
                              std::string conf_filename,
                              double gapPen,
                              double gapExt,
                              double endPenalty,
                              double lcr_mod,
                              int domainScore, 
                              int motifScore,
                              int phosphScore,
                              int codonLength,
                              bool weightsModeOn, 
                              ids_list motifs_ids,
                              probs_list motifs_probs){

  		Profile prf;
  		FeaturesProfile fprf(domainScore, phosphScore, motifScore, lcr_mod, 
                           motifs_ids, motifs_probs);
  		if (!conf_filename.empty()){
  			txtProc::process_conf_file(conf_filename, fprf, sequences);
  		}
      std::vector<double> identities;
  		//first round of the alignment - all vs 1st
  		std::vector<std::string> multipleAlignment(sequences.performMSAfirstround(prf, fprf, 
                                                                                gapPen, 
                                                                                endPenalty, 
                                                                                gapExt, 
                                                                                weightsModeOn, 
                                                                                codonLength, 
                                                                                identities));
  		string_sequences alignment;
  		int prev_alignments = 0;
  		for (int i = 8; i >= 0; i--){
  			double cutoff = double(i)/10;
  			sequences.performMSAnextRounds(&alignment, prf, fprf, gapPen, 
                                       endPenalty, gapExt,
                                       weightsModeOn, cutoff, codonLength, 
                                       identities, prev_alignments);
  			//prev_alignments - number of alignments performed in the previous round - 
        //to omit this round if the number of aligned sequences is the same as
        //in the previous round
  		}
  		prev_alignments = 0;  // to align (again) all sequences to the profile
  		sequences.performMSAnextRounds(&alignment, prf, fprf, 
                                     gapPen, endPenalty, gapExt, 
                                     weightsModeOn, 0, 
                                     codonLength, identities, 
                                     prev_alignments);
      return alignment;
}