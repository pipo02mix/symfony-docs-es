Cómo minimizar JavaScript y hojas de estilo con YUI Compressor
==============================================================

¡Yahoo! proporciona una excelente utilidad para minimizar (minify) JavaScript y hojas de estilo para que viaje por la red más rápido, el `YUI Compressor`_. Gracias a ``Assetic``, puedes tomar ventaja de esta herramienta con mucha facilidad.

Descargando el JAR de YUI Compressor
------------------------------------

El YUI Compressor está escrito en Java y se distribuye como JAR. `Descarga el JAR`_ desde el sitio Yahoo! y guárdalo en ``app/Resources/java/yuicompressor.jar``.

Configurando los filtros de YUI
-------------------------------

Ahora debes configurar dos *filtros* ``Assetic`` en tu aplicación, uno para minimizar JavaScript con el compresor YUI y otro para minimizar hojas de estilo:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            filters:
                yui_css:
                    jar: "%kernel.root_dir%/Resources/java/yuicompressor.jar"
                yui_js:
                    jar: "%kernel.root_dir%/Resources/java/yuicompressor.jar"

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="yui_css"
                jar="%kernel.root_dir%/Resources/java/yuicompressor.jar" />
            <assetic:filter
                name="yui_js"
                jar="%kernel.root_dir%/Resources/java/yuicompressor.jar" />
        </assetic:config>

    .. code-block:: php

        // app/config/config.php
        $contenedor->loadFromExtension('assetic', array(
            'filters' => array(
                'yui_css' => array(
                    'jar' => '%kernel.root_dir%/Resources/java/yuicompressor.jar',
                ),
                'yui_js' => array(
                    'jar' => '%kernel.root_dir%/Resources/java/yuicompressor.jar',
                ),
            ),
        ));

Ahora tienes acceso a dos nuevos *filtros* ``Assetic`` en tu aplicación: ``yui_css`` y ``yui_js``. Estos utilizan el compresor de YUI para minimizar hojas de estilo y JavaScripts, respectivamente.

Minimizando tus activos
-----------------------

Ahora tienes configurado el compresor YUI, pero nada va a pasar hasta que apliques uno de estos filtros a un activo. Dado que tus activos son una parte de la capa de la vista, este trabajo se hace en tus plantillas:

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/*' filter='yui_js' %}
        <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*'),
            array('yui_js')) as $url): ?>
        <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

.. note::

    El ejemplo anterior asume que tienes un paquete llamado ``AcmeFooBundle`` y tus archivos JavaScript están bajo el directorio ``Resources/public/js`` de tu paquete. No obstante, esto no es importante - puedes incluir tus archivos Javascript sin importar donde se encuentren.

Con la incorporación del filtro ``yui_js`` a las etiquetas de los activos anteriores, ahora deberías ver llegar tus JavaScripts minimizados a través del cable mucho más rápido. Puedes repetir el mismo proceso para minimizar tus hojas de estilo.

.. configuration-block::

    .. code-block:: html+jinja

        {% stylesheets '@AcmeFooBundle/Resources/public/css/*' filter='yui_css' %}
        <link rel="stylesheet" type="text/css" media="screen" href="{{ asset_url }}" />
        {% endstylesheets %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->stylesheets(
            array('@AcmeFooBundle/Resources/public/css/*'),
            array('yui_css')) as $url): ?>
        <link rel="stylesheet" type="text/css" media="screen" href="<?php echo $view->escape($url) ?>" />
        <?php endforeach; ?>

Desactivando la minimización en modo de depuración
--------------------------------------------------

El JavaScript y las hojas de estilo minimizadas son muy difíciles de leer, y mucho más de depurar. Debido a esto, ``Assetic`` te permite desactivar un determinado *filtro* cuando la aplicación está en modo de depuración. Para ello, puedes prefijar el nombre del *filtro* en tu plantilla con un signo de interrogación: ``?``. Esto le dice a ``Assetic`` que aplique este filtro sólo cuando el modo de depuración está desactivado.

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/*' filter='?yui_js' %}
        <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*'),
            array('?yui_js')) as $url): ?>
        <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

.. _`YUI Compressor`: http://developer.yahoo.com/yui/compressor/
.. _`Descarga el JAR`: http://yuilibrary.com/downloads/#yuicompressor