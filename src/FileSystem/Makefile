.SUFFIXES = .c .cpp

BOOST=../../../boost
SALIDA_EJECUTABLE=../../bin

MIS_DEFINE=-DBOOST_ALL_NO_LIB

RAIZ=.
INCLUDED=${BOOST}/ \
    ${BOOST}/boost \
    ${BOOST}/libs/filesystem/src \
    ${BOOST}/boost/filesystem \
    ${BOOST}/libs/system/src \
    ${BOOST}/boost/system

PARAM_INCLUDE:=$(addprefix "-I", ${INCLUDED})

VPATH=${INCLUDED}

DIR_OBJETOS = Objs/

BOOST_LIBS=operations.o path.o portability.o error_code.o
OBJETOS += $(addprefix ${DIR_OBJETOS}, ${BOOST_LIBS})

PROGRAMA := FileSystem.o
OBJETOS += $(addprefix ${DIR_OBJETOS}, ${PROGRAMA})

FileSystem: crear_directorios Makefile ${OBJETOS}
	@$(CXX) -s $(LDFLAGS) ${MIS_LINKS} -o $(SALIDA_EJECUTABLE)/$@ $(OBJETOS)
	@echo "Todo terminado????"
#	@echo "Ejecutando ...."
#	@/tmp/$@

${DIR_OBJETOS}%.o: %.cpp
	@echo "Compilando C++ $? -> $@"
	@${CXX} -O2 -s -x c++ -pipe ${CFLAGS} ${MIS_DEFINE} ${PARAM_INCLUDE} -c $? -o $@

${DIR_OBJETOS}%.o:%.c
	@echo "Compilando C $? -> $@"
	@${CXX} ${PARAM_INCLUDE} -c $? -o $@

clean:
	@rm ${OBJETOS}

crear_directorios:
	@mkdir -p ${DIR_OBJETOS}

compilar:
	@echo ${VPATH} ${OBJETOS}
