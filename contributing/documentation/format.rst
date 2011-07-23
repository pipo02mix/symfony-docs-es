Formato de documentación
========================

La documentación Symfony2 utiliza `reStructuredText`_ como lenguaje de marcado y `Sphinx`_ para construir la documentación (en HTML, PDF, ...).

reStructuredText
----------------

reStructuredText "es un sistema analizador y sintaxis de marcado de texto, fácil de leer, lo que ves es lo que obtienes".

Puedes aprender más acerca de su sintaxis leyendo los `documentos`_ existentes de Symfony2 o leyendo el `Primer reStructuredText`_ en el sitio web de Sphinx.

Si estás familiarizado con Markdown, ten cuidado que las cosas a veces se ven muy similares, pero son diferentes:

* Las listas se inician al comienzo de una línea (no permite sangría);

* Los bloques de código en línea utilizan comillas dobles (````como estas````).

Sphinx
------

Sphinx es un sistema de construcción que añade algunas herramientas agradables para crear la documentación desde documentos reStructuredText. Como tal, agrega nuevas directivas e interpreta texto en distintos roles al `marcado`_ reST estándar.

Resaltado de sintaxis
~~~~~~~~~~~~~~~~~~~~~

Todo el código de los ejemplos utiliza PHP como resaltado del lenguaje por omisión. Puedes cambiarlo con la directiva ``code-block``:

.. code-block:: rst

    .. code-block:: yaml

        { foo: bar, bar: { foo: bar, bar: baz } }

Si el código PHP comienza con ``<?php``, entonces necesitas usar ``html+php`` como pseudolenguaje a resaltar:

.. code-block:: rst

    .. code-block:: html+php

        <?php echo $this->foobar(); ?>

.. note::

    Una lista de lenguajes apoyados está disponible en el `sitio web Pygments`_.

Bloques de configuración
~~~~~~~~~~~~~~~~~~~~~~~~

Cada vez que muestres una configuración, debes utilizar la directiva ``configuration-block`` para mostrar la configuración en todos los formatos de configuración compatibles (``YAML``, ``XML`` y ``PHP``)

.. code-block:: rst

    .. configuration-block::

        .. code-block:: yaml

            # Configuración en YAML

        .. code-block:: xml

            <!-- Configuración en XML //-->

        .. code-block:: php

            // Configuración en PHP

El fragmento reST anterior se reproduce de la siguiente manera:

.. configuration-block::

    .. code-block:: yaml

        # Configuración en YAML

    .. code-block:: xml

        <!-- Configuración en XML //-->

    .. code-block:: php

        // Configuración en PHP

La lista de formatos apoyados actualmente es la siguiente:

+----------------------+-------------+
| Formato de marcado   | Muestra     |
+======================+=============+
| html                 | HTML        |
+----------------------+-------------+
| xml                  | XML         |
+----------------------+-------------+
| php                  | PHP         |
+----------------------+-------------+
| yaml                 | YAML        |
+----------------------+-------------+
| jinja                | Twig        |
+----------------------+-------------+
| html+jinja           | Twig        |
+----------------------+-------------+
| jinja+html           | Twig        |
+----------------------+-------------+
| php+html             | PHP         |
+----------------------+-------------+
| html+php             | PHP         |
+----------------------+-------------+
| ini                  | INI         |
+----------------------+-------------+
| php-annotations      | Anotaciones |
+----------------------+-------------+

Probando la documentación
~~~~~~~~~~~~~~~~~~~~~~~~~

Para probar la documentación antes de consignarla:

 * Instala `Sphinx`_;

 * Ejecuta el programa de `instalación rápida de Sphinx`_;

 * Instala la extensión `configuration-block` de Sphinx (ver más abajo);

 * Ejecuta ``make html`` y revisa el código HTML generado en el directorio ``build``.

Instalando la extensión configuration-block de Sphinx
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 * Descarga la extensión desde el repositorio `fuente de configuration-block`_

* Copia el ``configurationblock.py`` al directorio ``_exts`` bajo tu directorio fuente (donde está ubicado ``conf.py``)

* Agrega lo siguiente al archivo ``conf.py``:

.. code-block:: py

    # ...
    sys.path.append(os.path.abspath('_exts'))

    # ...
    # agrega configurationblock a la lista de extensiones
    extensions = ['configurationblock']

.. _reStructuredText:                     http://docutils.sf.net/rst.html
.. _Sphinx:                               http://sphinx.pocoo.org/
.. _documentos:                           http://github.com/symfony/symfony-docs
.. _Primer reStructuredText:              http://sphinx.pocoo.org/rest.html
.. _marcado:                              http://sphinx.pocoo.org/markup/
.. _sitio web Pygments:                   http://pygments.org/languages/
.. _fuente de configuration-block:           https://github.com/fabpot/sphinx-php
.. _instalación rápida de Sphinx:         http://sphinx.pocoo.org/tutorial.html#setting-up-the-documentation-sources
