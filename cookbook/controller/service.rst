.. index::
   single: Controlador; Como servicios

Cómo definir controladores como servicios
=========================================

En el libro, haz aprendido lo fácilmente que se puede utilizar un controlador cuando se extiende la clase base :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller`. Si bien esto funciona estupendamente, los controladores también se pueden especificar como servicios.

Para referir un controlador que se defina como servicio, utiliza la notación de dos puntos individuales (:). Por ejemplo, supongamos que hemos definido un servicio llamado ``mi_controlador`` y queremos que redirija a un método llamado ``indexAction()`` dentro del servicio::

    $this->forward('mi_controlador:indexAction', array('foo' => $bar));

Necesitas usar la misma notación para definir el valor ``_controller`` de la ruta:

.. code-block:: yaml

    mi_controlador:
        pattern:   /
        defaults:  { _controller: mi_controlador:indexAction }

Para utilizar un controlador de esta manera, este se debe definir en la configuración del contenedor de servicios. Para más información, consulta el capítulo :doc:`Contenedor de servicios </book/service_container>`.

Cuando se utiliza un controlador definido como servicio, lo más probable es no ampliar la clase base ``Controller``. En lugar de confiar en sus métodos de acceso directo, debes interactuar directamente con los servicios que necesitas. Afortunadamente, esto suele ser bastante fácil y la clase base ``Controller`` en sí es una gran fuente sobre la manera de realizar muchas tareas comunes.

.. note::

    Especificar un controlador como servicio requiere un poco más de trabajo. La principal ventaja es que el controlador completo o cualquier otro servicio pasado al controlador se puede modificar a través de la configuración del contenedor de servicios.
    Esto es útil especialmente cuando desarrollas un paquete de código abierto o cualquier paquete que se pueda utilizar en muchos proyectos diferentes. Así que, aunque no especifiques los controladores como servicios, es probable que veas hacer esto en algunos paquetes de código abierto de Symfony2.
