.SUFFIXES = .c .cpp

# export FIREBIRD=/opt/libfirebird
export LD_LIBRARY_PATH=/opt/libfirebird

MIS_DEFINE= -DIBPP_LINUX
MIS_LINKS=-lfbembed -L${LD_LIBRARY_PATH}
MIS_LIBRERIAS=../../../Librerias
BOOST=../../../boost
SALIDA_EJECUTABLE=../../bin

RAIZ=.
INCLUDED=${MIS_LIBRERIAS}/ \
	${MIS_LIBRERIAS}/GuruCompat/ \
	${BOOST}/ \
	${BOOST}/libs/program_options/src/ \
	${MIS_LIBRERIAS}/ibpp/ \
	../Comun/

PARAM_INCLUDE:=$(addprefix -I, ${INCLUDED})

VPATH=${INCLUDED}

DIR_OBJETOS = Objs/

PROGRAMA := Buscar.o
OBJETOS += $(addprefix ${DIR_OBJETOS}, ${PROGRAMA})

BOOST_PROGRAM_OPTIONS=options_description.o variables_map.o value_semantic.o cmdline.o \
convert.o positional_options.o utf8_codecvt_facet.o
OBJETOS += $(addprefix ${DIR_OBJETOS}, ${BOOST_PROGRAM_OPTIONS})

LIBRERIAS := xtoa.o all_in_one.o 
OBJETOS += $(addprefix ${DIR_OBJETOS}, ${LIBRERIAS})

PROPIOS := ModeloCatalogo.o
OBJETOS += $(addprefix ${DIR_OBJETOS}, ${PROPIOS})

Buscar: crear_directorios Makefile ${OBJETOS}
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
	@rm -f ${OBJETOS}

crear_directorios:
	@mkdir -p ${DIR_OBJETOS}

compilar:
	@echo ${VPATH} ${OBJETOS} ${PARAM_INCLUDE}

