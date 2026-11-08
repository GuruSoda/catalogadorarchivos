.SUFFIXES = .c .cpp

SALIDA_EJECUTABLE=../../bin

LIBRERIAS=../../../Librerias

INCLUDED=\
	../lib \
	${LIBRERIAS} \
	${LIBRERIAS}/polarssl/include \
	${LIBRERIAS}/polarssl/library


PARAM_INCLUDE:=$(addprefix -I, ${INCLUDED})

VPATH=${INCLUDED}

DIR_OBJETOS = Objs/

PROGRAMA := SHA.o 
OBJETOS += $(addprefix ${DIR_OBJETOS}, ${PROGRAMA})

MIS_LIBS := GuruCRC.o GuruSHA1.o GuruSHA2.o GuruSHA4.o sha1.o sha2.o sha4.o
OBJETOS += $(addprefix ${DIR_OBJETOS}, ${MIS_LIBS})

sha: crear_directorios Makefile ${OBJETOS}
	@$(CXX) -s $(LDFLAGS) ${MIS_LINKS} -o ${SALIDA_EJECUTABLE}/$@ $(OBJETOS)
	@echo "Todo terminado..."
#	@echo "Ejecutando ${SALIDA_EJECUTABLE}/$@..."
#	@./${SALIDA_EJECUTABLE}/$@ --ayuda

${DIR_OBJETOS}%.o:%.cpp
	@echo "Compilando C++ $? -> $@"
	@${CXX} -I. -O2 -s -x c++ -pipe ${CFLAGS} ${MIS_DEFINE} ${PARAM_INCLUDE} -c $? -o $@

${DIR_OBJETOS}%.o:%.c
	@echo "Compilando C $? -> $@"
	@${CC} -I. -pipe ${CFLAGS} ${MIS_DEFINE} ${PARAM_INCLUDE} -c $? -o $@

clean:
	@rm ${OBJETOS}

crear_directorios:
	@mkdir -p ${DIR_OBJETOS}

compilacion:
	@echo ${VPATH} ${OBJETOS} ${DIR_OBJETOS}

