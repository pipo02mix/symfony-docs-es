Cómo utilizar ``Assetic`` para gestionar activos
================================================

``Assetic`` es una biblioteca para gestionar activos que se incluye con la distribución estándar de Symfony2 y se puede utilizar fácilmente en Symfony2 directamente desde las plantillas Twig o PHP.

``Assetic`` combina dos ideas principales: *activos* y *filtros*. Los *activos* son archivos tal como los archivos CSS, JavaScript e imágenes. Los *filtros* son cosas que se pueden aplicar a estos archivos antes de servirlos al navegador. Esto te permite una separación entre los archivos de activos almacenados en tu aplicación y los archivos presentados realmente al usuario.

Sin ``Assetic``, sólo sirves los archivos que están almacenados directamente en la aplicación:

.. configuration-block::

    .. code-block:: html+jinja

        <script src="{{ asset('js/script.js') }}" type="text/javascript" />

    .. code-block:: php

        <script src="<?php echo $view['assets']->getUrl('js/script.js') ?>"
                type="text/javascript" />

Sin embargo, *con* ``Assetic``, puedes manipular estos activos como quieras (o cargarlos desde cualquier lugar) antes de servirlos. Esto significa que puedes:

* Minimizarlos con ``minify`` y combinar todos tus archivos CSS y JS

* Ejecutar todos (o algunos) de tus archivos CSS o JS a través de algún tipo de compilador, como LESS, SASS o CoffeeScript

* Ejecutar optimizaciones de imagen en tus imágenes

Activos
-------

``Assetic`` ofrece muchas ventajas sobre los archivos que sirves directamente.
Los archivos no se tienen que almacenar dónde son servidos y se pueden obtener de diversas fuentes, tal como desde dentro de un paquete:

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/*'
        %}
        <script type="text/javascript" src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*')) as $url): ?>
        <script type="text/javascript" src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

En este ejemplo, todos los archivos en el directorio ``Resources/public/js/`` del ``AcmeFooBundle`` se cargan y sirven desde un lugar diferente.
En realidad la etiqueta reproducida simplemente podría ser::

    <script src="/js/abcd123.js"></script>

También puedes combinar varios archivos en uno solo. Esto ayuda a reducir el número de peticiones HTTP, lo cual es bueno para un rendimiento frontal extremo, ya que la mayoría de los navegadores sólo procesan un número limitado a la vez frenando la carga de tus páginas.
También te permite mantener los archivos con mayor facilidad dividiéndolos en partes manejables. Esto también te puede ayudar con la reutilización puesto que fácilmente puedes dividir los archivos de proyectos específicos de los que puedes utilizar en otras aplicaciones, pero aún los servirás como un solo archivo:

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/*'
                       '@AcmeBarBundle/Resources/public/js/form.js'
                       '@AcmeBarBundle/Resources/public/js/calendar.js'
        %}
        <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*',
                  '@AcmeBarBundle/Resources/public/js/form.js',
                  '@AcmeBarBundle/Resources/public/js/calendar.js')) as $url): ?>
        <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

Esto no sólo se aplica a tus propios archivos, también lo puede utilizar ``Assetic`` para combinar activos de terceros, tal como jQuery como un solo archivo en sí mismo:

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/thirdparty/jquery.js'
                       '@AcmeFooBundle/Resources/public/js/*'
        %}
        <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/thirdparty/jquery.js',
                  '@AcmeFooBundle/Resources/public/js/*')) as $url): ?>
        <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

*Filtros*
---------

Adicionalmente a esto ``Assetic``, puede aplicar *filtros* a tus *activos* antes de servirlos. Esto incluye tareas tales como compresión de la salida para reducir el tamaño de los archivos, el cual es otro valor de optimización frontal. Otros filtros incluyen la compilación de archivos JavaScript desde archivos CoffeeScript y SASS a CSS.

Muchos de los filtros no hacen el trabajo directamente, sino que utilizan otras bibliotecas para hacerlo, esto es por lo que a menudo tienes que instalar ese software también.
La gran ventaja de utilizar ``Assetic`` para invocar estas bibliotecas es que en lugar de tener que ejecutarlo manualmente cuando haz trabajado en los archivos, ``Assetic`` se hará cargo de esto por ti y elimina por completo este paso de tu desarrollo y proceso de despliegue.

Para usar un filtro debes especificarlo en la configuración de ``Assetic`` ya que no están habilitados por omisión. Por ejemplo, para utilizar el Compresor de JavaScript YUI hay que añadir la siguiente configuración:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            filters:
                yui_js:
                    jar: "%kernel.root_dir%/Resources/java/yuicompressor.jar"

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="yui_js"
                jar="%kernel.root_dir%/Resources/java/yuicompressor.jar" />
        </assetic:config>

    .. code-block:: php

        // app/config/config.php
        $contenedor->loadFromExtension('assetic', array(
            'filters' => array(
                'yui_js' => array(
                    'jar' => '%kernel.root_dir%/Resources/java/yuicompressor.jar',
                ),
            ),
        ));


A continuación, puedes especificar la utilización del filtro en la plantilla:

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


Puedes encontrar una guía más detallada de la configuración y uso de *filtros* ``Assetic`` así como detalles del modo de depuración de ``Assetic`` en :doc:`/cookbook/assetic/yuicompressor`.

Controlando la URL utilizada
----------------------------

Si quieres puedes controlar las URL que produce Assetic. Esto se hace desde la plantilla y es relativo a la raíz del documento público:

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/*'
           output='js/combined.js'
        %}
        <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*'),
            array(),
            array('output' => 'js/combined.js')
        ) as $url): ?>
        <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

Memorizando la salida
---------------------

El proceso de creación de los archivos servidos puede ser bastante lento, especialmente cuando utilizas algunos de los filtros que invocan software de terceros para el trabajo real. Incluso cuando trabajas en el entorno de desarrollo se desacelera la carga de páginas, si esto se fuera a hacer cada vez, rápidamente sería frustrante. Afortunadamente en el entorno de desarrollo ``Assetic`` memoriza la salida para que esto no suceda, en lugar de tener que borrar la caché manualmente, sin embargo, monitorea los cambios en los *activos* y regenera los archivos según sea necesario. Esto significa que puedes trabajar en los archivos de activos y ver los resultados al cargar la página, pero sin tener que sufrir continuas cargas lentas de la página.

Para producción, en la que no vas a realizar cambios a los archivos de los activos, el rendimiento puede ser mayor, evitando el paso de la comprobación de cambios.
Assetic te permite ir más allá y evitar el contacto con Symfony2 e incluso PHP del todo al servir los archivos. Esto se hace vertiendo todos los archivos de salida con una orden de consola. Estos los puede suministrar directamente el servidor web como archivos estáticos, aumentando el rendimiento y permitiendo que el servidor web haga frente a las cabeceras de caché. La orden de la consola para verter los archivos es:

.. code-block:: bash

    php app/console assetic:dump

.. note::

    Una vez que haz vertido la salida tendrás que ejecutar la orden de consola de nuevo para ver los nuevos cambios. Si estás corriendo el servidor de desarrollo tendrás que eliminar los archivos a fin de empezar a permitir que ``Assetic`` de nuevo procese los *activos* sobre la marcha.
