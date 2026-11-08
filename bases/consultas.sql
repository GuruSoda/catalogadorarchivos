-- Para mostrar aquellos los archivos que estan repetidos.
select a.nombre, a.tamanio,a.FECHA, a.crc32, c.nombre
from ARCHIVO a, CATALOGO b, DIRECTORIO c
where a.ID_ARCHIVO = b.ID_ARCHIVO and b.ID_DIRECTORIO = C.ID_DIRECTORIO
    and (select count(b.tamanio) 
        from ARCHIVO b 
        where a.TAMANIO = b.TAMANIO and a.CRC32 = b.CRC32 and a.SHA1 = b.SHA1 and a.ID_ARCHIVO <> b.ID_ARCHIVO and a.ID_ARCHIVO < b.ID_ARCHIVO) > 0
order by a.ID_ARCHIVO

-- otra forma de listar los archivos repetidos
select a.nombre, a.tamanio,a.FECHA, a.crc32, c.nombre
from ARCHIVO a, CATALOGO b, DIRECTORIO c
where a.ID_ARCHIVO = b.ID_ARCHIVO and b.ID_DIRECTORIO = C.ID_DIRECTORIO
    and in (select b.tamanio from ARCHIVO b 
        where a.TAMANIO = b.TAMANIO and a.CRC32 = b.CRC32 and a.SHA1 = b.SHA1 and a.ID_ARCHIVO <> b.ID_ARCHIVO)

-- Muestra los archivos repetidos pero con distinto nombre.
select tamanio,crc32,sha1,sha512,count(*)
from ARCHIVO
group by tamanio,crc32,sha1,sha512
having count(*) > 1
order by 5 DESC

-- para ver los detalles de un archivo.
select * from ARCHIVO
where sha1='a982e328943a6527285ee0f0e8878d640c8dd73d'

select * from ARCHIVO
where id_archivo=1159

-- muestra los archivos iguales repetidos en el catalogo
select id_archivo,count(*)
from CATALOGO
group by id_archivo
having count(*) > 1
order by 2 DESC

-- muestra los archivos iguales repetidos en el catalogo
select a.id_archivo,b.NOMBRE,count(*)
from CATALOGO A, ARCHIVO B
where A.ID_ARCHIVO = B.ID_ARCHIVO
group by id_archivo,nombre
having count(*) > 1
order by 3 DESC

-- muestra los archivos repetido 
select ARCHIVO.NOMBRE,ARCHIVO.TAMANIO,ARCHIVO.FECHA,count(*) 
from archivo, directorio, CATALOGO
where CATALOGO.ID_ARCHIVO = ARCHIVO.ID_ARCHIVO and CATALOGO.ID_DIRECTORIO = DIRECTORIO.ID_DIRECTORIO
group by ARCHIVO.NOMBRE,ARCHIVO.TAMANIO,ARCHIVO.FECHA
having count(*) > 1
order by count(*) DESC

-- lista los archivos que tiene el mismo tamanio.
select ARCHIVO.TAMANIO,count(*) 
from archivo, directorio, CATALOGO
where CATALOGO.ID_ARCHIVO = ARCHIVO.ID_ARCHIVO and CATALOGO.ID_DIRECTORIO = DIRECTORIO.ID_DIRECTORIO
group by ARCHIVO.TAMANIO
having count(*) > 1
order by count(*) DESC

-- lista los archivo que estan en la tabla archivo pero no estan en la tabla catalogo
"select * from archivo a where a.id_archivo not in (select id_archivo from catalogo b where b.id_archivo=a.id_archivo)"

-- borrar los archivos que NO estan en el catalogo, pero si en la tabla archivo.
"delete from archivo a where a.id_archivo not in (select id_archivo from catalogo b where b.id_archivo=a.id_archivo)"

-- Muestra los directorio que contengan la palabra mame en algun lugar
"select * from directorio where lower(nombre) like '%mame%'"


-- Listado totales por extension
select lower(right(a.NOMBRE, 4)),count(*)
from CATALOGO c, ARCHIVO a
where c.ID_ARCHIVO = a.ID_ARCHIVO
group by 1
order by 2 desc


-- Lista los directorios que hay debajo de un directorio
select distinct(substring(substring(d.NOMBRE from char_length('/Compilados/')+1) from 1 for position ('/' in substring(d.NOMBRE from char_length('/Compilados/')+1))))
from DIRECTORIO d
where d.NOMBRE like '/Compilados/%'

-- lista los archivos que hay en un directorio
select a.NOMBRE
from CATALOGO c, DIRECTORIO d, ARCHIVO a
where c.ID_DIRECTORIO = d.ID_DIRECTORIO and c.ID_ARCHIVO = a.ID_ARCHIVO
    and d.NOMBRE = '/Compilados/'
