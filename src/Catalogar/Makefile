.SUFFIXES = .c .cpp

export FIREBIRD=../../lib
export LD_LIBRARY_PATH=../../lib

MIS_DEFINE= -DIBPP_LINUX -DBOOST_ALL_NO_LIB
MIS_LINKS=-lfbembed -L${LD_LIBRARY_PATH}

SALIDA_EJECUTABLE=../../bin

LIBRERIAS=../../../Librerias
BOOST=../../../boost

INCLUDED=${LIBRERIAS} \
    ${LIBRERIAS}/GuruCompat/ \
    ${BOOST}/ \
    ${BOOST}/libs/regex/src/ \
    ${LIBRERIAS}/argtable/ \
    ${LIBRERIAS}/ibpp/ \
    ../Comun/ \
    ${LIBRERIAS}/polarssl/include/ \
    ${LIBRERIAS}/polarssl/library/

PARAM_INCLUDE:=$(addprefix "-I", ${INCLUDED})

VPATH=${INCLUDED}

DIR_OBJETOS = Objs/

PROGRAMA := main.o 
OBJETOS += $(addprefix ${DIR_OBJETOS}, ${PROGRAMA})

ARGTABLE= arg_int.o arg_str.o arg_dbl.o arg_lit.o argtable2.o arg_end.o arg_rem.o getopt1.o arg_rex.o getopt.o
OBJETOS += $(addprefix ${DIR_OBJETOS}, ${ARGTABLE})

BOOST_REGEX=regex.o cpp_regex_traits.o posix_api.o icu.o regex_traits_defaults.o c_regex_traits.o\
regex_raw_buffer.o fileiter.o usinstances.o instances.o static_mutex.o 
OBJETOS += $(addprefix ${DIR_OBJETOS}, ${BOOST_REGEX})

POLARSSL:=sha1.o sha2.o sha4.o md5.o 
OBJETOS += $(addprefix ${DIR_OBJETOS}, ${POLARSSL})

MIS_LIBS := xtoa.o all_in_one.o GuruFDBBase.o 
OBJETOS += $(addprefix ${DIR_OBJETOS}, ${MIS_LIBS})

PROPIOS := ModeloCatalogo.o
OBJETOS += $(addprefix ${DIR_OBJETOS}, ${PROPIOS})

catalogador: crear_directorios Makefile ${OBJETOS}
	@$(CXX) -s $(LDFLAGS) -o ${SALIDA_EJECUTABLE}/$@ $(OBJETOS) ${MIS_LINKS}
	@echo "Todo terminado..."
	@echo "Ejecutando ${SALIDA_EJECUTABLE}/$@..."
	@./${SALIDA_EJECUTABLE}/$@ --ayuda

${DIR_OBJETOS}%.o: %.cpp
	@echo "Compilando C++ $? -> $@"
	@${CXX} -O3 -Wall -s -x c++ -pipe ${CFLAGS} ${MIS_DEFINE} ${PARAM_INCLUDE} -c $? -o $@

${DIR_OBJETOS}%.o:%.c
	@echo "Compilando C $? -> $@"
	@${CXX} ${PARAM_INCLUDE} -c $? -o $@

clean:
	@rm -f ${OBJETOS}

crear_directorios:
	@mkdir -p ${DIR_OBJETOS}

compilacion:
	@echo ${VPATH} ${OBJETOS}

