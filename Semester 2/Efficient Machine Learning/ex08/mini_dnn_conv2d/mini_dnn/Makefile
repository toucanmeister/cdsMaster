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
# ReLU
#
${BUILD_DIR}/backend/ReluAten.o: src/backend/ReluAten.cpp
		$(CXX) ${OPTIONS} ${CXXFLAGS} -c src/backend/ReluAten.cpp -o ${BUILD_DIR}/backend/ReluAten.o

${BUILD_DIR}/tests/ReluAten.test.o: src/backend/ReluAten.test.cpp
		$(CXX) ${OPTIONS} ${CXXFLAGS} -c src/backend/ReluAten.test.cpp -o ${BUILD_DIR}/tests/ReluAten.test.o

${BUILD_DIR}/backend/ReluLibxsmm.o: src/backend/ReluLibxsmm.cpp
		$(CXX) ${OPTIONS} ${CXXFLAGS} -c src/backend/ReluLibxsmm.cpp -I${LIBXSMM_DIR}/include -o ${BUILD_DIR}/backend/ReluLibxsmm.o

${BUILD_DIR}/tests/ReluLibxsmm.test.o: src/backend/ReluLibxsmm.test.cpp
		$(CXX) ${OPTIONS} ${CXXFLAGS} -c src/backend/ReluLibxsmm.test.cpp -o ${BUILD_DIR}/tests/ReluLibxsmm.test.o

relu: ${BUILD_DIR}/backend/ReluAten.o ${BUILD_DIR}/tests/ReluAten.test.o ${BUILD_DIR}/backend/ReluLibxsmm.o ${BUILD_DIR}/tests/ReluLibxsmm.test.o
		$(CXX) ${OPTIONS} ${CXXFLAGS} src/performance_relu.cpp ${BUILD_DIR}/backend/Relu*.o -o ${BUILD_DIR}/performance_relu ${RPATHS} ${LDFLAGS}

#
# Conv2d
#
${BUILD_DIR}/tests/Conv2d.test.o: src/backend/Conv2d.test.cpp src/backend/Conv2d.hpp
		$(CXX) ${OPTIONS} ${CXXFLAGS} -c src/backend/Conv2d.test.cpp -o ${BUILD_DIR}/tests/Conv2d.test.o

${BUILD_DIR}/backend/Conv2dAten.o: src/backend/Conv2dAten.cpp src/backend/Conv2d.hpp
		$(CXX) ${OPTIONS} ${CXXFLAGS} -c src/backend/Conv2dAten.cpp -o ${BUILD_DIR}/backend/Conv2dAten.o

${BUILD_DIR}/backend/Conv2dIm2col.o: src/backend/Conv2dIm2col.cpp
		$(CXX) ${OPTIONS} ${CXXFLAGS} -c src/backend/Conv2dIm2col.cpp -o ${BUILD_DIR}/backend/Conv2dIm2col.o

${BUILD_DIR}/tests/Conv2dIm2col.test.o: src/backend/Conv2dIm2col.test.cpp
		$(CXX) ${OPTIONS} ${CXXFLAGS} -c src/backend/Conv2dIm2col.test.cpp -o ${BUILD_DIR}/tests/Conv2dIm2col.test.o


conv2d: relu src/test.cpp ${BUILD_DIR}/tests/Conv2d.test.o ${BUILD_DIR}/backend/Conv2dAten.o ${BUILD_DIR}/backend/Conv2dIm2col.o ${BUILD_DIR}/tests/Conv2dIm2col.test.o
		$(CXX) ${OPTIONS} ${CXXFLAGS} src/performance_conv2d.cpp ${BUILD_DIR}/backend/Relu*.o ${BUILD_DIR}/backend/Conv2d*.o -o ${BUILD_DIR}/performance_conv2d ${RPATHS} ${LDFLAGS}

all: relu conv2d
		$(CXX) ${OPTIONS} ${CXXFLAGS} src/test.cpp ${BUILD_DIR}/backend/*.o ${BUILD_DIR}/tests/*.o -o ${BUILD_DIR}/test ${RPATHS} ${LDFLAGS}

$(shell mkdir -p build/backend)
$(shell mkdir -p build/tests)
