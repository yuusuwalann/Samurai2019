CXXFLAGS= -g -Wall -std=c++14 -std=c++1z
CXX= c++
LDFLAGS= -lm -lpthread

.cpp.o:
	${CXX} -c ${CXXFLAGS} $*.cpp -o $*.o
	${CXX} -MM ${CXXFLAGS} $*.cpp > $*.d

all:SamuraiCoding.o SamuraiManager.o DogManager.o GameInfo.o Cell.o \
	Agent.o HaveDugInfo.o HaveBarkedInfo.o AccessInfo.o RevealedMapInfo.o \
	MoveFreeInfo.o IntelligentPetDogAwarness.o  IntelligentStokingDogAwarness.o  \
	Awarness.o AwarnessControl.o IntelligentAwarness.o AwarnessControlForDog.o \
	IntelligentEscapeAwarness.o IntelligentMoveAwarness.o IntelligentDigAwarness.o \
	IntelligentChaisAwarness.o IntelligentDogGuardAwarness.o
	${CXX} ${CFLAGS} -o ThreeStones SamuraiCoding.o SamuraiManager.o DogManager.o GameInfo.o Cell.o \
	Agent.o HaveDugInfo.o HaveBarkedInfo.o AccessInfo.o RevealedMapInfo.o \
	MoveFreeInfo.o IntelligentPetDogAwarness.o  IntelligentStokingDogAwarness.o \
	Awarness.o AwarnessControl.o IntelligentAwarness.o AwarnessControlForDog.o \
	IntelligentEscapeAwarness.o IntelligentMoveAwarness.o IntelligentDigAwarness.o \
	IntelligentChaisAwarness.o IntelligentDogGuardAwarness.o

clean:
	rm -f *.o *.d
