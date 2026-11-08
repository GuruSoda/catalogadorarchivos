/*
 * ddl.h
 *
 *  Created on: 20/11/2008
 *      Author: fabricio
 */

#ifndef DDL_H_
#define DDL_H_

#define DDL const char* catalogo[] = {"\
create table directorio( \
	id_directorio INTEGER NOT NULL, \
	nombre VARCHAR (268) NOT NULL, \
	estado SMALLINT default 0 NOT NULL, \
\
	UNIQUE(NOMBRE), \
	PRIMARY KEY(id_directorio) \
) \
","\
create table archivo( \
	id_archivo INTEGER NOT NULL, \
	nombre VARCHAR (268) NOT NULL, \
	tamanio BIGINT NOT NULL, \
	fecha TIMESTAMP NOT NULL, \
	md5 VARCHAR (34) NOT NULL, \
	sha1 VARCHAR (42) NOT NULL, \
	sha512 VARCHAR (130) NOT NULL, \
	estado SMALLINT default 0 NOT NULL, \
\
	UNIQUE(nombre, tamanio, fecha), \
	PRIMARY KEY(id_archivo) \
) \
","\
create table etiqueta( \
	id_etiqueta SMALLINT NOT NULL, \
	nombre VARCHAR (65) NOT NULL, \
	directorio VARCHAR (268), \
	filtro VARCHAR (256), \
	opciones SMALLINT default 0 NOT NULL, \
\
	UNIQUE(nombre), \
	PRIMARY KEY(id_etiqueta) \
) \
","\
create table catalogo( \
	id_catalogo		INTEGER NOT NULL, \
	id_etiqueta		SMALLINT NOT NULL, \
	id_directorio	INTEGER NOT NULL, \
	id_archivo		INTEGER NOT NULL, \
	agregado		TIMESTAMP default current_timestamp, \
	estado 			SMALLINT default 0 NOT NULL, \
\
	UNIQUE(id_etiqueta, id_directorio, id_archivo), \
	FOREIGN KEY(id_directorio) REFERENCES directorio(id_directorio) on delete cascade, \
	FOREIGN KEY(id_archivo) REFERENCES archivo(id_archivo) on delete cascade, \
	FOREIGN KEY(id_etiqueta) REFERENCES etiqueta(id_etiqueta) on delete cascade \
)\
","\
	CREATE SEQUENCE SEQ_ID_CATALOGO; \
","\
	CREATE SEQUENCE SEQ_ID_ARCHIVO; \
","\
	CREATE SEQUENCE SEQ_ID_DIRECTORIO; \
","\
	CREATE SEQUENCE SEQ_ID_ETIQUETA;",NULL};

#endif /* DDL_H_ */
