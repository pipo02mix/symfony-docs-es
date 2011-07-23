Traduciendo
===========

La documentación de Symfony2 está escrita en Inglés y hay muchas personas involucradas en el proceso de traducción.

Colaborando
-----------

En primer lugar, familiarízate con el :doc:`lenguaje de marcado <format>` empleado en la documentación.

En segundo lugar, sigue las :doc:`buenas prácticas de la traducción profesional <tools_translate>`, utilizando las mejores herramientas libres disponibles hoy día.

A continuación, suscríbete a la `lista de correo docs de Symfony`_, debido a que la colaboración sucede allí.

Por último, busca el repositorio *maestro* para el idioma con el que deseas contribuir. Esta es la lista oficial de los repositorios *maestros*:

* *Inglés*:  http://github.com/symfony/symfony-docs
* *Ruso*:  http://github.com/avalanche123/symfony-docs-ru
* *Rumano*: http://github.com/sebio/symfony-docs-ro
* *Japonés*: https://github.com/symfony-japan/symfony-docs-ja

.. note::

    Si quieres contribuir traducciones para un nuevo lenguaje, lee la :ref:`sección dedicada <translations-adding-a-new-language>`.

Uniéndote al equipo de traducción
---------------------------------

Si quieres ayudar a traducir algunos documentos a tu idioma o corregir algunos errores, considera unirte, es un proceso muy sencillo:

* Preséntate en la `lista de correo docs de Symfony`_;
* *(opcional)* Solicita los documentos en que puedes trabajar;
* Bifurca el repositorio *principal* de tu idioma (haciendo clic en el botón "*Fork*" en la página GitHub);
* Traduce algunos documentos;
* Haz una petición de atracción (haciendo clic en la "petición de atracción" de tu página en GitHub);
* El director del equipo acepta tus modificaciones y las combina en el repositorio principal;
* El sitio web de documentación se actualiza todas las noches desde el repositorio principal.

.. _translations-adding-a-new-language:

Añadiendo un nuevo idioma
-------------------------

Esta sección ofrece algunas pautas para comenzar la traducción de la documentación de Symfony2 para un nuevo idioma.

Debido a que iniciar una traducción conlleva mucho trabajo, habla acerca de tu plan en la `lista de correo docs de Symfony`_ y trata de encontrar personas motivadas dispuestas a ayudar.

Cuando el equipo esté listo, nomina un administrador del equipo, quién será el responsable del repositorio *maestro*.

Crea el repositorio y copia los documentos en *Inglés*.

El equipo ahora puede iniciar el proceso de traducción.

Cuando el equipo confíe en que el repositorio está en un estado coherente y estable (se ha traducido todo, o los documentos sin traducir se han retirado de los toctrees -- archivos con el nombre ``index.rst`` y ``map.rst.inc``), el administrador del equipo puede pedir que el repositorio se añada a la lista de repositorios *maestro* oficiales enviando un correo electrónico a Fabien (fabien arroba symfony.com).

Manteniendo
-----------

La traducción no termina cuando se ha traducido todo. La documentación es un objeto en movimiento (se agregan nuevos documentos, se corrigen errores, se reorganizan párrafos, ...). El equipo de traducción tiene que seguir de cerca los cambios del repositorio en Inglés y aplicarlos a los documentos traducidos tan pronto como sea posible.

.. caution::

    Los idiomas sin mantenimiento se quitan de la lista oficial de repositorios de documentación puesto que la obsolescencia es peligrosa.

.. _`lista de correo docs de Symfony`: http://groups.google.com/group/symfony-docs
