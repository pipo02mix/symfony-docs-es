.. index::
   single: Requisitos

Requisitos para que funcione Symfony2
=====================================

Para ejecutar Symfony2, el sistema debe cumplir con una lista de requisitos. Fácilmente puedes ver si el sistema pasa todos los requisitos ejecutando ``web/config.php`` en tu distribución de Symfony. Debido a que el CLI a menudo utiliza un archivo de configuración ``php.ini`` diferente, también es una buena idea revisar tus requisitos desde la línea de ordenes por medio de:

.. code-block:: bash

    php app/check.php

A continuación mostramos la lista de requisitos obligatorios y opcionales.

Obligatorio
-----------

* PHP debe ser una versión mínima de PHP 5.3.2
* Tu php.ini debe tener configurado el valor date.timezone

Opcional
--------

* Necesitas tener instalado el módulo PHP-XML
* Debes tener por lo menos la versión 2.6.21 de libxml
* Debe estar habilitado el tokenizer de PHP
* Las funciones mbstring deben estar habilitadas
* iconv debe estar habilitada
* POSIX debe estar habilitado
* Intl tiene que estar instalado
* APC (u otro opcode de caché debe estar instalado)
* php.ini con la configuración recomendada

    * short_open_tags: off
    * magic_quotes_gpc: off
    * register_globals: off
    * session.autostart: off

Doctrine
--------

Si deseas utilizar Doctrine, necesitarás tener instalado PDO. Además, es necesario tener instalado el controlador de PDO para el servidor de base de datos que desees utilizar.