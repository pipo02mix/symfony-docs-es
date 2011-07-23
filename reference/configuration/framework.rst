.. index::
   single: Referencia de configuración; FrameworkBundle

Configurando el ``FrameworkBundle`` ("framework")
=================================================

Este documento de referencia es un trabajo en progreso. Este debe ser preciso, pero todavía no están totalmente cubiertas todas las opciones.

El ``FrameworkBundle`` contiene la mayor parte de la funcionalidad "base" de la plataforma y se puede configurar bajo la clave ``framework`` en la configuración de tu aplicación.
Esto incluye ajustes relacionados con sesiones, traducción, formularios, validación, enrutado y mucho más.

Configurando
------------

* `charset`_
* `secret`_
* `ide`_
* `test`_
* `form`_
    * :ref:`enabled<config-framework-form-enabled>`
* `csrf_protection`_
    * :ref:`enabled<config-framework-csrf-enabled>`
    * `field_name`

charset
.......

**tipo**: ``string`` **predeterminado**: ``UTF-8``

El conjunto de caracteres que se utiliza en toda la plataforma. Se convierte en el parámetro del contenedor de servicios llamado ``kernel.charset``.

secret
......

**tipo**: ``string`` **requerido**

Esta es una cadena que debe ser única para tu aplicación. En la práctica, se utiliza para generar las señales CSRF, pero esta se podría utilizar en cualquier otro contexto donde una cadena única sea útil. Se convierte en el parámetro llamado ``kernel.secret`` del contenedor de servicios.

ide
...

**tipo**: ``string`` **predeterminado**: ``null``

Si estás utilizando un IDE como TextMate o Vim Mac, Symfony puede convertir todas las rutas de archivo en un mensaje de excepción en un enlace, el cual abrirá el archivo en el IDE.

Si usas TextMate o Vim Mac, simplemente puedes utilizar uno de los siguientes valores integrados:

* ``textmate``
* ``macvim``

También puedes especificar una cadena personalizada como enlace al archivo. Si lo haces, debes duplicar todos los signos de porcentaje (``%``) para escapar ese carácter. Por ejemplo, la cadena completa de TextMate se vería así:

.. code-block:: yaml

    framework:
        ide:  "txmt://open?url=file://%%f&line=%%l"

Por supuesto, ya que cada desarrollador utiliza un IDE diferente, es mejor poner esto a nivel del sistema. Esto se puede hacer estableciendo en php.ini el valor de ``xdebug.file_link_format`` a la cadena de enlace al archivo. Si estableces este valor de configuración, entonces no es necesario determinar la opción ``ide``.

test
....

**tipo**: ``Boolean``

Si este parámetro de configuración está presente, se cargan los servicios relacionados con las pruebas de tu aplicación. Este valor debe estar presente en tu entorno ``test`` (por lo general a través de ``app/config/config_test.yml``). Para más información, consulta :doc:`/book/testing`.

form
....

csrf_protection
...............


Configuración predeterminada completa
-------------------------------------

.. configuration-block::

    .. code-block:: yaml

        framework:

            # Configuración general
            charset:              ~
            secret:               ~ # Required
            ide:                  ~
            test:                 ~

            # configuración de formulario
            form:
                enabled:              true
            csrf_protection:
                enabled:              true
                field_name:           _token

            # configuración esi
            esi:
                enabled:              true

            # configuración del generador de perfiles
            profiler:
                only_exceptions:      false
                only_master_requests:  false
                dsn:                  sqlite:%kernel.cache_dir%/profiler.db
                username:
                password:
                lifetime:             86400
                matcher:
                    ip:                   ~
                    path:                 ~
                    service:              ~

            # configuración de enrutado
            router:
                resource:             ~ # Required
                type:                 ~
                http_port:            80
                https_port:           443

            # configuración de sesión
            session:
                auto_start:           ~
                default_locale:       en
                storage_id:           session.storage.native
                name:                 ~
                lifetime:             ~
                path:                 ~
                domain:               ~
                secure:               ~
                httponly:             ~

            # configuración de plantillas
            templating:
                assets_version:       ~
                assets_version_format:  ~
                assets_base_urls:
                    http:                 []
                    ssl:                  []
                cache:                ~
                engines:              # Required
                form:
                    resources:        [FrameworkBundle:Form]

                    # Ejemplo:
                    - twig
                loaders:              []
                packages:

                    # Prototipo
                    name:
                        version:              ~
                        version_format:       ~
                        base_urls:
                            http:                 []
                            ssl:                  []

            # configuración de traducción
            translator:
                enabled:              true
                fallback:             en

            # configuración de validación
            validation:
                enabled:              true
                cache:                ~
                enable_annotations:   false

            # configuración de anotaciones
            annotations:
                cache:                file
                file_cache_dir:       %kernel.cache_dir%/annotations
                debug:                true



