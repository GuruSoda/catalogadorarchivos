create table usuario (
        id_usuario    	INTEGER NOT NULL,
        usuario       	CHAR (32) NOT NULL,
	nombre		CHAR (256) NOT NULL,
        correo        	CHAR (64) NOT NULL,
        clave         	CHAR (32) NOT NULL,
        rol		INTEGER NOT NULL,

	PRIMARY KEY(id_usuario)
);

create table resultado (
	id_consulta	INTEGER NOT NULL,
	id_archivo 	INTEGER NOT NULL
);

create table consulta (
	id_consulta	INTEGER NOT NULL,
	consulta	CHAR (2048) NOT NULL,
	nombre		CHAR (256) NOT NULL,
	catalogo	CHAR (268) NOT NULL,
	etiqueta	CHAR (32) NOT NULL
);

