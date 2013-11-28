OBJECTS = MSA.o Sequences.o ScoringMatrix.o substitutionMatrix.o Profile.o misc.o txtProc.o vecUtil.o findVal.o


MSA: $(OBJECTS)
	clang++ -std=c++11 -stdlib=libc++ -o $@ $(OBJECTS) /Users/joanna/Documents/MASTER_THESIS/software/boost/stage/lib/libboost_program_options.a


MSA.o: MSA.cpp 
	clang++ -std=c++11 -stdlib=libc++ -c MSA.cpp 

Sequences.o: Sequences.h  Sequences.cpp
	clang++ -std=c++11 -stdlib=libc++ -c Sequences.cpp 

ScoringMatrix.o: ScoringMatrix.h ScoringMatrix.cpp
	clang++ -std=c++11 -stdlib=libc++ -c ScoringMatrix.cpp 

substitutionMatrix.o: substitutionMatrix.h substitutionMatrix.cpp
	clang++ -std=c++11 -stdlib=libc++ -c substitutionMatrix.cpp
Profile.o: Profile.h Profile.cpp
	clang++ -std=c++11 -stdlib=libc++ -c Profile.cpp 
misc.o: misc.h misc.cpp
	clang++ -std=c++11 -stdlib=libc++ -c misc.cpp
txtProc.o: txtProc.h txtProc.cpp
	clang++ -std=c++11 -stdlib=libc++ -c txtProc.cpp
vecUtil.o: vecUtil.h vecUtil.cpp
	clang++ -std=c++11 -stdlib=libc++ -c vecUtil.cpp
findVal.o: findVal.h findVal.cpp
	clang++ -std=c++11 -stdlib=libc++ -c findVal.cpp
clean: 
	rm -f *.o MSA *.h.gch
