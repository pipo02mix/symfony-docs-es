.. index::
   single: Instalando

Instalando y configurando Symfony
=================================

El objetivo de este capítulo es el de empezar a trabajar con una aplicación funcionando incorporada en lo alto de Symfony. Afortunadamente, Symfony dispone de "distribuciones", que son proyectos Symfony funcionales desde el "arranque", los cuales puedes descargar y comenzar a desarrollar inmediatamente.

Descargando una distribución de Symfony2
----------------------------------------

.. tip::

    En primer lugar, comprueba que tienes instalado y configurado un servidor web (como Apache) con PHP 5.3.2 o superior. Para más información sobre los requisitos de Symfony2, consulta la :doc:`referencia de requisitos </reference/requirements>`.

Los paquetes de las "distribuciones" de Symfony2, son aplicaciones totalmente funcionales que incluyen las bibliotecas del núcleo de Symfony2, una selección de útiles paquetes, una sensible estructura de directorios y alguna configuración predeterminada. Al descargar una distribución Symfony2, estás descargando el esqueleto de una aplicación funcional que puedes utilizar inmediatamente para comenzar a desarrollar tu aplicación.

Empieza por visitar la página de descarga de Symfony2 en `http://symfony.com/download`_.
En esta página, puedes encontrar la *Edición estándar de Symfony*, que es la distribución principal de Symfony2. En este caso, necesitas hacer dos elecciones:

* Descargar o bien un archivo ``.tgz`` o ``.zip`` - ambos son equivalentes, descarga aquel con el que te sientas más cómodo;

* Descarga la distribución con o sin ``vendors``. Si tienes instalado `Git`_ en tu ordenador, debes descargar Symfony2 "sin ``vendors``", debido a que esto añade un poco más de flexibilidad cuando incluyas bibliotecas de terceros.

Descarga uno de los archivos en algún lugar bajo el directorio raíz de tu servidor web local y descomprímelo. Desde una línea de ordenes de UNIX, esto se puede hacer con una de las siguientes ordenes (sustituye ``###`` con el nombre de archivo real):

.. code-block:: bash

    # para un archivo .tgz
    tar zxvf Symfony_Standard_Vendors_2.0.###.tgz

    # para un archivo .zip
    unzip Symfony_Standard_Vendors_2.0.###.zip

Cuando hayas terminado, debes tener un directorio ``Symfony/`` que se ve algo como esto:

.. code-block:: text

    www/ <- tu directorio raíz del servidor web
        Symfony/ <- el archivo extraído
            app/
                cache/
                config/
                logs/
            src/
                ...
            vendor/
                ...
            web/
                app.php
                ...

Actualizando ``vendors``
~~~~~~~~~~~~~~~~~~~~~~~~

Por último, si descargaste el archivo "sin ``vendors``", instala tus proveedores ejecutando el siguiente método desde la línea de ordenes:

.. code-block:: bash

    php bin/vendors install

Esta orden descarga todas las bibliotecas de terceros necesarias - incluyendo al mismo Symfony - en el directorio ``vendor/``.

Instalando y configurando
~~~~~~~~~~~~~~~~~~~~~~~~~

En este punto, todas las bibliotecas de terceros necesarias ahora viven en el directorio ``vendor/``. También tienes una instalación predeterminada de la aplicación en ``app/`` y algunos ejemplos de código dentro de ``src/``.

Symfony2 viene con una interfaz visual para probar la configuración del servidor, muy útil para ayudarte a solucionar problemas relacionados con la configuración de tu servidor web y PHP para utilizar Symfony. Usa la siguiente URL para examinar tu configuración:

.. code-block:: text

    http://localhost/Symfony/web/config.php

Si hay algún problema, corrígelo antes de continuar.

.. sidebar:: Configurando permisos

    Un problema común es que ambos directorios ``app/cache`` y ``app/logs`` deben tener permiso de escritura, tanto para el servidor web cómo para la línea de ordenes del usuario. En un sistema UNIX, si el usuario del servidor web es diferente de tu usuario de línea de ordenes, puedes ejecutar las siguientes ordenes una sola vez en el proyecto para garantizar que los permisos se configuran correctamente. Cambia ``www-data`` al usuario del servidor web y ``tuNombre`` a tu usuario de la línea de ordenes:

    **1. Usando ACL en un sistema que admite chmod +a**

    .. code-block:: bash

        Muchos sistemas te permiten utilizar la orden chmod +a. Intenta esto primero, y si se produce un error - prueba el método siguiente:

        rm -rf app/cache/*
        rm -rf app/logs/*

        sudo chmod +a "www-data allow delete,write,append,file_inherit,directory_inherit" app/cache app/logs
        sudo chmod +a "tuNombre allow delete,write,append,file_inherit,directory_inherit" app/cache app/logs

    **2. Usando ACL en un sistema que no es compatible con un chmod +a**

    Algunos sistemas, como Ubuntu, no son compatibles con ``chmod +a``, pero son compatibles con otra utilidad llamada ``setfacl``. En algunos sistemas, este se tendrá que instalar antes de usarlo:

    .. code-block:: bash

        sudo setfacl -R -m u:www-data:rwx -m u:tuNombre:rwx app/cache app/logs
        sudo setfacl -dR -m u:www-data:rwx -m u:tuNombre:rwx app/cache app/logs

    **3. Sin usar ACL**

    Si no tienes acceso para modificar los directorios ACL, tendrás que cambiar la ``umask`` para que los directorios ``cache/`` y ``logs/`` se puedan escribir por el grupo o por cualquiera (dependiendo de si el usuario del servidor web y el usuario de la línea de ordenes están en el mismo grupo o no). Para ello, pon la siguiente línea al comienzo de los archivos ``app/console``, ``web/app.php`` y ``web/app_dev.php``:

    .. code-block:: php

        umask(0002); // Esto permitirá que los permisos sean 0775

        // o

        umask(0000); // Esto permitirá que los permisos sean 0777

    Ten en cuenta que el uso de ACL se recomienda cuando tienes acceso a ellos en el servidor porque cambiar la ``umask`` no es seguro en subprocesos.

Cuando todo esté listo, haz clic en el enlace "Visita la página de Bienvenida" para ver tu primer aplicación "real" en Symfony2:

.. code-block:: text

    http://localhost/Symfony/web/app_dev.php/

¡Symfony2 debería felicitarte por tu arduo trabajo hasta el momento!

.. image:: /images/quick_tour/bienvenida.jpg

Empezando a desarrollar
-----------------------

Ahora que tienes una aplicación Symfony2 completamente funcional, ¡puedes comenzar el desarrollo! Tu distribución puede contener algún código de ejemplo - comprueba el archivo ``README.rst`` incluido con la distribución (ábrelo como un archivo de texto) para saber qué código de ejemplo incluye tu distribución y cómo lo puedes eliminar más tarde.

Si eres nuevo en Symfony, alcánzanos en ":doc:`page_creation`", donde aprenderás a crear páginas, cambiar la configuración, y todo lo demás que necesitas en tu nueva aplicación.

Usando control de código fuente
-------------------------------

Si estás utilizando un sistema de control de versiones como ``Git`` o ``Subversion``, puedes configurar tu sistema de control de versiones y empezar a consignar el proyecto de manera normal. Para ``Git``, se puede hacer fácilmente con la siguiente orden:

.. code-block:: bash

    git init

Para más información sobre la configuración y uso de Git, echa un vistazo a las guías del `Campamento de inicio GitHub`_.

Ignorando el directorio ``vendor/``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si descargaste el archivo "sin ``vendors``", puedes ignorar con seguridad todo el directorio ``vendor/`` y no consignarlo al control de código fuente. Con ``Git``, esto se logra creando un archivo ``.gitignore`` y añadiendo lo siguiente:

.. code-block:: text

    vendor/

Ahora, el directorio de proveedores no será consignado al control de código fuente. Esto está muy bien (en realidad, ¡es genial!) porque cuando alguien más clone o coteje el proyecto, él/ella simplemente puede ejecutar el archivo ``php bin/vendors.php`` para descargar todas las bibliotecas de proveedores necesarias.

.. _`http://symfony.com/download`: http://symfony.com/download
.. _`Git`: http://git-scm.com/
.. _`Campamento de inicio GitHub`: http://help.github.com/set-up-git-redirect
