.. index::
   single: Perfiles; Colector de datos

Cómo crear un colector de datos personalizado
=============================================

El :doc:`Generador de perfiles </book/internals/profiler>` de Symfony2 delega la recolección de datos a los colectores de datos. Symfony2 incluye algunos colectores, pero puedes crear tu propio diseño fácilmente.

Creando un colector de datos personalizado
------------------------------------------

Crear un colector de datos personalizado es tan simple como implementar la clase :class:`Symfony\\Component\\HttpKernel\\DataCollector\\DataCollectorInterface`::

    interface DataCollectorInterface
    {
        /**
         * Recoge los datos de las instancias de Request y Response.
         *
         * @param Request    $peticion   Una instancia de Request
         * @param Response   $respuesta  Una instancia de Response
         * @param \Exception $exception Una instancia de Exception
         */
        function collect(Request $peticion, Response $respuesta, \Exception $exception = null);

        /**
         * Devuelve el nombre del colector.
         *
         * @return string El nombre del colector
         */
        function getNombre();
    }

El método ``getNombre()`` debe devolver un nombre único. Este se utiliza para acceder a la información más adelante en (consulta :doc:`/cookbook/testing/profiling` para ver un ejemplo).

El método ``collect()`` se encarga de almacenar los datos de las propiedades locales a las que quieres dar acceso.

.. caution::

    Puesto que el generador de perfiles serializa instancias del colector de datos, no debes almacenar objetos que no se puedan serializar (como objetos PDO), o tendrás que proporcionar tu propio método ``serialize()``.

La mayoría de las veces, es conveniente extender :class:`Symfony\\Component\\HttpKernel\\DataCollector\\DataCollector` y rellenar los datos de la propiedad ``$this->data`` (que se encarga de serializar la propiedad ``$this->data``)::

    class MemoryDataCollector extends DataCollector
    {
        public function collect(Request $peticion, Response $respuesta, \Exception $exception = null)
        {
            $this->data = array(
                'memory' => memory_get_peak_usage(true),
            );
        }

        public function getMemory()
        {
            return $this->data['memory'];
        }

        public function getNombre()
        {
            return 'memory';
        }
    }

.. _data_collector_tag:

Habilitando colectores de datos personalizados
----------------------------------------------

Para habilitar un colector de datos, lo tienes que añadir como un servicio regular a tu configuración, y etiquetarlo con ``data_collector``:

.. configuration-block::

    .. code-block:: yaml

        services:
            data_collector.el_nombre_de_tu_colector:
                class: Fully\Qualified\Collector\Class\Name
                tags:
                    - { name: data_collector }

    .. code-block:: xml

        <service id="data_collector.el_nombre_de_tu_colector" class="Fully\Qualified\Collector\Class\Name">
            <tag name="data_collector" />
        </service>

    .. code-block:: php

        $contenedor
            ->register('data_collector.el_nombre_de_tu_colector', 'Fully\Qualified\Collector\Class\Name')
            ->addTag('data_collector')
        ;

Añadiendo el generador de perfiles web en plantillas
----------------------------------------------------

Cuando desees mostrar los datos recogidos por el colector de datos en la barra de depuración o el generador de perfiles web, crea una plantilla Twig siguiendo este esqueleto:

.. code-block:: jinja

    {% extends 'WebProfilerBundle:Profiler:base.html.twig' %}

    {% block toolbar %}
        {# contenido de la barra de herramientas de depuración web #}
    {% endblock %}

    {% block head %}
        {# si el panel del generador de perfiles necesita algunos archivos JS o CSS específicos #}
    {% endblock %}

    {% block menu %}
        {# el contenido del menú #}
    {% endblock %}

    {% block panel %}
        {# el contenido del panel #}
    {% endblock %}

Cada bloque es opcional. El bloque ``toolbar`` se utiliza para la barra de herramientas de depuración web ``menu`` y ``panel`` se utilizan para agregar un grupo especial al generador de perfiles web.

Todos los bloques tienen acceso al objeto ``collector``.

.. tip::

    Las plantillas incorporadas utilizan una imagen codificada en base64 para la barra de herramientas (``<img src="src="data:image/png;base64,..."``). Puedes calcular el valor base64 para una imagen con este pequeño guión: ``echo
    base64_encode(file_get_contents($_SERVER['argv'][1]));``.

Para habilitar la plantilla, agrega un atributo ``template`` a la etiqueta ``data_collector`` en tu configuración. Por ejemplo, asumiendo que tu plantilla está en algún ``AcmeDebugBundle``:

.. configuration-block::

    .. code-block:: yaml

        services:
            data_collector.el_nombre_de_tu_colector:
                class: Acme\DebugBundle\Collector\Class\Name
                tags:
                    - { name: data_collector, template: "AcmeDebug:Collector:templatename", id: "el_nombre_de_tu_colector" }

    .. code-block:: xml

        <service id="data_collector.el_nombre_de_tu_colector" class="Acme\DebugBundle\Collector\Class\Name">
            <tag name="data_collector" template="AcmeDebug:Collector:templatename" id="el_nombre_de_tu_colector" />
        </service>

    .. code-block:: php

        $contenedor
            ->register('data_collector.el_nombre_de_tu_colector', 'Acme\DebugBundle\Collector\Class\Name')
            ->addTag('data_collector', array('template' => 'AcmeDebugBundle:Collector:templatename', 'id' => 'el_nombre_de_tu_colector'))
        ;
