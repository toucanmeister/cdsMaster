BUILD_DIR ?= ./build
CXXFLAGS ?=
LDFLAGS ?=
RPATHS ?=
LIBXSMM_DIR ?= libxsmm
OPTIONS = -O2 -std=c++20 -pedantic -Wall -Wextra -DTORCH_API_INCLUDE_EXTENSION_H -I.

PYTORCH_INCLUDE = $(shell python -c 'from torch.utils.cpp_extension import include_paths; [print(p) for p in include_paths()]')
PYTORCH_LINK = $(shell python -c 'from torch.utils.cpp_extension import library_paths; [print(p) for p in library_paths()]')

CXXFLAGS += $(foreach inc,$(PYTORCH_INCLUDE),-isystem$(inc)) -fopenmp
LDFLAGS += $(foreach lin,$(PYTORCH_LINK),-L$(lin)) -lc10 -ltorch -ltorch_cpu -ldl ${LIBXSMM_DIR}/lib/libxsmm.a
RPATHS += $(foreach lin,$(PYTORCH_LINK),-Wl,-rpath,$(lin))

$(info $$CXXFLAGS is [${CXXFLAGS}])
$(info $$LDFLAGS is [${LDFLAGS}])

#
# Matmul
#
${BUILD_DIR}/backend/MatmulAtenBlocked.o: src/backend/MatmulAtenBlocked.cpp
		$(CXX) ${OPTIONS} ${CXXFLAGS} -c src/backend/MatmulAtenBlocked.cpp -o ${BUILD_DIR}/backend/MatmulAtenBlocked.o

${BUILD_DIR}/tests/MatmulAtenBlocked.test.o: src/backend/MatmulAtenBlocked.test.cpp
		$(CXX) ${OPTIONS} ${CXXFLAGS} -c src/backend/MatmulAtenBlocked.test.cpp -o ${BUILD_DIR}/tests/MatmulAtenBlocked.test.o

${BUILD_DIR}/backend/MatmulLibxsmm.o: src/backend/MatmulLibxsmm.cpp
		$(CXX) ${OPTIONS} ${CXXFLAGS} -c src/backend/MatmulLibxsmm.cpp -I${LIBXSMM_DIR}/include -o ${BUILD_DIR}/backend/MatmulLibxsmm.o

${BUILD_DIR}/tests/MatmulLibxsmm.test.o: src/backend/MatmulLibxsmm.test.cpp
		$(CXX) ${OPTIONS} ${CXXFLAGS} -c src/backend/MatmulLibxsmm.test.cpp -o ${BUILD_DIR}/tests/MatmulLibxsmm.test.o

${BUILD_DIR}/backend/MatmulReluAten.o: src/backend/MatmulReluAten.cpp
		$(CXX) ${OPTIONS} ${CXXFLAGS} -c src/backend/MatmulReluAten.cpp -I${LIBXSMM_DIR}/include -o ${BUILD_DIR}/backend/MatmulReluAten.o

${BUILD_DIR}/tests/MatmulReluAten.test.o: src/backend/MatmulReluAten.test.cpp
		$(CXX) ${OPTIONS} ${CXXFLAGS} -c src/backend/MatmulReluAten.test.cpp -I${LIBXSMM_DIR}/include -o ${BUILD_DIR}/tests/MatmulReluAten.test.o

matmul: ${BUILD_DIR}/backend/MatmulAtenBlocked.o ${BUILD_DIR}/tests/MatmulAtenBlocked.test.o ${BUILD_DIR}/backend/MatmulLibxsmm.o ${BUILD_DIR}/tests/MatmulLibxsmm.test.o ${BUILD_DIR}/backend/MatmulReluAten.o ${BUILD_DIR}/tests/MatmulReluAten.test.o
		$(CXX) ${OPTIONS} ${CXXFLAGS} src/performance_matmul.cpp  ${BUILD_DIR}/backend/Matmul*.o -o ${BUILD_DIR}/performance_matmul ${RPATHS} ${LDFLAGS}

all: matmul
		$(CXX) ${OPTIONS} ${CXXFLAGS} src/test.cpp ${BUILD_DIR}/backend/*.o ${BUILD_DIR}/tests/*.o -o ${BUILD_DIR}/test ${RPATHS} ${LDFLAGS}

$(shell mkdir -p build/backend)
$(shell mkdir -p build/tests)
