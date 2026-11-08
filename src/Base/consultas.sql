select a.nombre
from ARCHIVO a
where (select count(b.nombre) from archivo b where a.sha1 = b.sha1) > 1

select a.nombre, (select count(b.nombre) from archivo b where b.nombre = a.nombre)
from ARCHIVO a
where (select count(b.nombre) from archivo b where b.nombre = a.nombre) > 1

-- ver archivos duplicados de verdad (con nombre y sha1 iguales)
select nombre, sha1, count(*) "Total Veces"
from archivo
group by nombre, sha1
ORDER by 3 DESC

-- archivos con nombres iguales
select nombre, count(*) "Total Veces"
from archivo
group by nombre
order by 2 DESC

-- archivos con sha1 iguales
select sha1, count(*) "Total Veces"
from archivo
group by sha1
order by 2 DESC

-- listado completo
select a.NOMBRE "Etiqueta", b.nombre "Directorio", c.nombre "Archivo"
from etiqueta a, directorio b, archivo c, catalogo d
where a.ID_ETIQUETA = d.id_etiqueta AND
    b.id_directorio = d.id_directorio AND
    c.id_archivo = d.id_archivo

-- cantidad de archivos en un determinado catalogo
SELECT COUNT(*)
from CATALOGO, ETIQUETA
WHERE ETIQUETA.NOMBRE = 'Libros-Laburo'
    and ETIQUETA.ID_ETIQUETA = CATALOGO.ID_ETIQUETA

-- ver que nombres de archivos estan duplicados y cuantas veces
select CATALOGO.ID_ARCHIVO, ARCHIVO.NOMBRE, ARCHIVO.SHA1, count(*) "Total Veces"
from CATALOGO, ARCHIVO
WHERE CATALOGO.ID_ARCHIVO = ARCHIVO.ID_ARCHIVO
group by CATALOGO.ID_ARCHIVO, ARCHIVO.NOMBRE, ARCHIVO.SHA1
having count(*) > 1;

-- para modificar el nombre de un catalogo
UPDATE ETIQUETA
SET ETIQUETA.NOMBRE = 'Libros-Laburo'
WHERE ETIQUETA.NOMBRE = 'Libros'

-- para ver lo que ocupa cada catalogo 
SELECT ETIQUETA.NOMBRE "Catalogo", SUM(ARCHIVO.TAMANIO)/1024 "Cantidad de Kb", COUNT(*) "Total Archivos"
from ARCHIVO, CATALOGO, ETIQUETA
where ARCHIVO.ID_ARCHIVO = CATALOGO.ID_ARCHIVO
and ETIQUETA.ID_ETIQUETA = CATALOGO.ID_ETIQUETA
GROUP by ETIQUETA.NOMBRE

-- ver los repetidos y sus lugares
SELECT DIRECTORIO.NOMBRE "Directorio", A.NOMBRE "Archivo", A.TAMANIO "Tamanio", A.sha1 "SHA1",
(SELECT COUNT(*) from ARCHIVO B, CATALOGO, ETIQUETA WHERE A.sha1 = B.SHA1 and CATALOGO.ID_ETIQUETA = ETIQUETA.ID_ETIQUETA and ETIQUETA.NOMBRE = 'Libros-Casa') "Repetido"
FROM ETIQUETA, DIRECTORIO, ARCHIVO A, CATALOGO
WHERE CATALOGO.ID_ETIQUETA = ETIQUETA.ID_ETIQUETA
AND CATALOGO.ID_DIRECTORIO = DIRECTORIO.ID_DIRECTORIO
AND CATALOGO.ID_ARCHIVO = A.ID_ARCHIVO
AND ETIQUETA.NOMBRE = 'Libros-Casa'
AND (   SELECT COUNT(*) 
        from ARCHIVO B--, CATALOGO, ETIQUETA 
        WHERE A.sha1 = B.SHA1 
--        and CATALOGO.ID_ETIQUETA = ETIQUETA.ID_ETIQUETA 
--        and ETIQUETA.NOMBRE = 'Libros-Casa'
    ) > 1
ORDER by 4 DESC

-- ver los repetidos y sus lugares
SELECT DIRECTORIO.NOMBRE "Directorio", A.NOMBRE "Archivo", A.TAMANIO "Tamanio", A.sha1 "SHA1",
(      SELECT COUNT(*)
        from ARCHIVO B, CATALOGO, ETIQUETA
        WHERE CATALOGO.ID_ETIQUETA = ETIQUETA.ID_ETIQUETA
              and CATALOGO.ID_ARCHIVO = A.ID_ARCHIVO
              and ETIQUETA.NOMBRE = 'Libros-Laburo'
              and A.sha1 = B.SHA1) "Repetido"
FROM DIRECTORIO, ARCHIVO A, CATALOGO, ETIQUETA
WHERE CATALOGO.ID_DIRECTORIO = DIRECTORIO.ID_DIRECTORIO
AND CATALOGO.ID_ARCHIVO = A.ID_ARCHIVO
and CATALOGO.ID_ETIQUETA = ETIQUETA.ID_ETIQUETA
and ETIQUETA.NOMBRE = 'Libros-Laburo'
AND (   SELECT COUNT(*)
        from ARCHIVO B, CATALOGO, ETIQUETA
        WHERE A.sha1 = B.SHA1
        and CATALOGO.ID_ETIQUETA = ETIQUETA.ID_ETIQUETA
        and CATALOGO.ID_ARCHIVO = A.id_archivo
        and ETIQUETA.NOMBRE = 'Libros-Laburo'
    ) > 1
ORDER by 4 DESC

-- DROP INDEX INDEX_SHA1;
-- CREATE INDEX INDEX_SHA1 ON ARCHIVO(SHA1);

-- ALTER TABLE ARCHIVO ADD INDEX INDEX_SHA1 (SHA1);

/* asi anda paro tarda 11 minutos...
SELECT DIRECTORIO.NOMBRE "Directorio", A.NOMBRE "Archivo", A.TAMANIO "Tamanio", A.sha1 "SHA1"
FROM ETIQUETA, DIRECTORIO, ARCHIVO A, CATALOGO
WHERE CATALOGO.ID_ETIQUETA = ETIQUETA.ID_ETIQUETA
AND CATALOGO.ID_DIRECTORIO = DIRECTORIO.ID_DIRECTORIO
AND CATALOGO.ID_ARCHIVO = A.ID_ARCHIVO
AND ETIQUETA.NOMBRE = 'Libros-Casa'
AND (   SELECT COUNT(*)
        from ARCHIVO B, CATALOGO, ETIQUETA
        WHERE A.sha1 = B.SHA1
        and CATALOGO.ID_ETIQUETA = ETIQUETA.ID_ETIQUETA
        and CATALOGO.ID_ARCHIVO = A.id_archivo
        and ETIQUETA.NOMBRE = 'Libros-Casa'
    ) > 1
ORDER by 4 DESC
*/

select count(*)
from archivo
where sha1 = '7fe11cc8d1a6f1c741456de3805c20a9949f6828'

-- cuantas veces aparece un sha1 en la tabla archivo.
select *
from archivo
where sha1 = 'f7bfcd4907bbaef1bae045e0228d0636acbf43cf'

-- cuantas veces aparece un sha1 igual en todo el catalogo.
select count(*)
from catalogo, ARCHIVO
where catalogo.ID_ARCHIVO = ARCHIVO.ID_ARCHIVO
and sha1 = 'dd1e008020233c1b4558278a51ab3feb3387a175'

-- archivos iguales en el catalogo (nombre, tamanio, crc, sha1).
select AE.NOMBRE, DIRECTORIO.NOMBRE "Directorio", AA.NOMBRE "Archivo", AA.TAMANIO/1024 "Kb", AA.SHA1 "SHA1"
from CATALOGO AC, DIRECTORIO, ARCHIVO AA, ETIQUETA AE
WHERE AC.ID_ARCHIVO = AA.ID_ARCHIVO
    and AC.ID_DIRECTORIO = DIRECTORIO.ID_DIRECTORIO
    and AC.ID_ETIQUETA = AE.ID_ETIQUETA
    and AE.NOMBRE = 'Libros-Laburo'
    and (SELECT COUNT(*) FROM CATALOGO BC, ETIQUETA BE WHERE BC.ID_ARCHIVO = AC.ID_ARCHIVO and BC.ID_ETIQUETA = BE.ID_ETIQUETA and BE.NOMBRE = 'Libros-Laburo') > 1
ORDER by AA.TAMANIO DESC

-- ver archivo iguales (CRC32 y SHA1 iguales)
SELECT ETIQUETA.DIRECTORIO "Unidad",DIRECTORIO.NOMBRE "Directorio", A.NOMBRE "Archivo", A.TAMANIO/1024 "Kb", A.SHA1 "SHA1"
from CATALOGO, DIRECTORIO, ARCHIVO A, ETIQUETA
WHERE CATALOGO.ID_DIRECTORIO = DIRECTORIO.ID_DIRECTORIO 
        AND CATALOGO.ID_ETIQUETA = ETIQUETA.ID_ETIQUETA
        AND A.ID_ARCHIVO = CATALOGO.ID_ARCHIVO 
        AND (SELECT COUNT(*) from ARCHIVO B WHERE A.SHA1 = B.SHA1 and A.CRC32 = B.CRC32) > 1
ORDER by A.TAMANIO DESC

-- cuando ocupa en Kb los archivos duplicados?
SELECT COUNT(*), SUM(A.TAMANIO)/1024
from CATALOGO, DIRECTORIO, ARCHIVO A
WHERE CATALOGO.ID_DIRECTORIO = DIRECTORIO.ID_DIRECTORIO 
        AND A.ID_ARCHIVO = CATALOGO.ID_ARCHIVO 
--        AND SUBSTRING(A.NOMBRE from (CHAR_LENGTH(A.NOMBRE)-3) for CHAR_LENGTH(A.NOMBRE)) = UPPER('PDF')
        AND (SELECT COUNT(*) from ARCHIVO B WHERE A.SHA1 = B.SHA1 and A.CRC32 = B.CRC32) > 1
-- ORDER by A.SHA1

-- para saber que archivos tienen mismo crc y sha1 pero no nombre
SELECT A.*
from archivo A
where (select count(*)
      from archivo B
      where A.sha1 = B.sha1
      and a.crc32 = b.crc32
      and a.nombre <> b.nombre) > 1
order by a.sha1

-- lo mismo que el anterior pero con resultados distintos
SELECT DIRECTORIO.NOMBRE "Directorio", A.NOMBRE "Archivo", A.TAMANIO, A.SHA1 "SHA1"
from CATALOGO, DIRECTORIO, ARCHIVO A
WHERE CATALOGO.ID_DIRECTORIO = DIRECTORIO.ID_DIRECTORIO
        AND A.ID_ARCHIVO = CATALOGO.ID_ARCHIVO
        AND (SELECT COUNT(*) from ARCHIVO B WHERE A.SHA1 = B.SHA1 and A.CRC = B.CRC and a.nombre <> b.nombre) > 1
ORDER by A.sha1 DESC

-- para buscar un determinado sha1 en un determinado catalogo
SELECT etiqueta.nombre, archivo.nombre, archivo.tamanio, archivo.sha1
from archivo, catalogo, etiqueta
WHERE sha1 = 'fd7c2de6c973f018df1d2f35cff3554145fbf08e'
and catalogo.id_archivo = archivo.id_archivo
and catalogo.id_etiqueta = etiqueta.id_etiqueta
and etiqueta.nombre = 'Libros-Casa'

-- sacar los archivos duplicados (crc y sha1 iguales, nombre distintos)
SELECT '['||EA.NOMBRE||']' "Etiqueta", DA.NOMBRE "Directorio", AA.NOMBRE "Archivo", AA.TAMANIO/1024 "Kb", AA.SHA1 "SHA1"
FROM DIRECTORIO DA, CATALOGO CA, ARCHIVO AA, ETIQUETA EA
WHERE CA.ID_DIRECTORIO = DA.ID_DIRECTORIO
      AND CA.ID_ARCHIVO    = AA.ID_ARCHIVO
      AND CA.ID_ETIQUETA   = EA.ID_ETIQUETA
      AND EA.NOMBRE        = 'Libros-Casa'
      AND EXISTS (SELECT AB.ID_ARCHIVO
          from ARCHIVO AB, CATALOGO CB, ETIQUETA EB
         WHERE CB.ID_ARCHIVO = AB.ID_ARCHIVO 
--          AND CB.ID_ARCHIVO <> AB.ID_ARCHIVO
          AND CB.ID_ETIQUETA = EB.ID_ETIQUETA
          AND AA.SHA1 = AB.SHA1
          AND AA.CRC = AB.CRC
          AND (AA.nombre LIKE '%pdf'
          or AA.nombre LIKE '%chm')
          AND AA.nombre <> AB.NOMBRE
          AND EB.NOMBRE = 'Libros-Casa'
          )
ORDER by AA.SHA1 DESC

-- sacar la fecha y hora actal
select current_date, current_timestamp
from rdb$database;
-- o
select current_timestamp from RDB$DATABASE;

-- cuantas veces aparece cada archivo en el catalogo.
SELECT A.NOMBRE, (SELECT COUNT(*) from CATALOGO B WHERE A.ID_ARCHIVO = B.ID_ARCHIVO)
from archivo A
WHERE a.NOMBRE LIKE '%pdf'
-- WHERE (SELECT COUNT(*) from CATALOGO B WHERE A.ID_ARCHIVO = B.ID_ARCHIVO) > 1
ORDER by 2 DESC

SELECT A.id_archivo, A.nombre, A.tamanio, A.crc32, A.sha1/*, A.sha512*/, A.fecha
FROM ARCHIVO A, CATALOGO B, ETIQUETA C
WHERE B.ID_ARCHIVO = A.ID_ARCHIVO
    AND B.ID_ETIQUETA = C.ID_ETIQUETA
    AND C.NOMBRE =  'DVD-Aria-Giovanni'

SELECT B.nombre, B.tamanio, B.sha1
FROM CATALOGO A, ARCHIVO B, ETIQUETA C
WHERE A.ID_ARCHIVO = B.ID_ARCHIVO
AND   A.ID_ETIQUETA = C.ID_ETIQUETA
AND   C.nombre = 'GuruChuchis-004'

SELECT C.nombre
FROM ETIQUETA C

SELECT SUM(B.tamanio)/1024, D.NOMBRE||'->'||C.NOMBRE
FROM CATALOGO A, ARCHIVO B, ETIQUETA C, DIRECTORIO D
WHERE A.ID_ARCHIVO = B.ID_ARCHIVO
AND   A.ID_ETIQUETA = C.ID_ETIQUETA
AND   A.ID_DIRECTORIO = D.ID_DIRECTORIO
AND   D.NOMBRE LIKE '%Kianna%'
GROUP by 2

-- archivos repetidos
SELECT COUNT(*), B.tamanio||' : '||B.crc32||' : '||B.sha1||' : '||B.nombre
FROM CATALOGO A, ARCHIVO B, ETIQUETA C, DIRECTORIO D
WHERE A.ID_ARCHIVO = B.ID_ARCHIVO
AND   A.ID_ETIQUETA = C.ID_ETIQUETA
AND   A.ID_DIRECTORIO = D.ID_DIRECTORIO
AND   B.nombre not like '%jpg'
GROUP by 2
HAVING COUNT(*) > 1
ORDER by 1
